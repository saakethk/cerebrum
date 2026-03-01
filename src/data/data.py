""" Outward facing class that can actually be used for data retrieval """
import polars as pl
from datetime import datetime, timedelta

from connector import DuckDB
from retrieval import YFinance

class Data():

  def __init__(self):
    self.data_client = YFinance()
    self.db_client = DuckDB(path="data/duck.db")

  def is_cached(self, symbol: str, frequency: str) -> bool:
    table: str = f"{symbol}_{frequency}"
    table_names = [table["name"] for table in self.db_client.get_tables().to_dicts()]
    if (table in table_names):
      return True
    return False

  def check_cached(self, symbol: str, frequency: str) -> tuple[datetime | None, datetime | None]:
    table: str = f"{symbol}_{frequency}"
    if (self.is_cached(symbol=symbol, frequency=frequency) == False):
      return (None, None)
    start_data: dict = self.db_client.retrieve_first(table_name=table)
    last_data: dict = self.db_client.retrieve_last(table_name=table)
    start_date = start_data["date"]
    end_date = last_data["date"]
    return (start_date.astype(datetime), end_date.astype(datetime))

  def get_data(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=(365 * 100))), 
               end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
    cached_start, cached_end = self.check_cached(symbol=symbol, frequency=frequency)
    print(cached_start)
    print(cached_end)
    if (cached_start == None) and (cached_end == None):
      # nothing is cached
      data: pl.DataFrame = self.data_client.get_tickers(
        symbol=symbol, start_date=start_date,
        end_date=end_date, frequency=frequency)
    elif (cached_start == cached_end):
      # one single date is cached
      data: pl.DataFrame = self.data_client.get_tickers(
        symbol=symbol, start_date=start_date,
        end_date=end_date, frequency=frequency)
    else:
      # there is a range of dates
      data: pl.DataFrame = pl.DataFrame()
      if (start_date < cached_start): #type: ignore
        pre_start: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=start_date,
          end_date=cached_start, frequency=frequency) #type: ignore
        print(pre_start)
        if (data.is_empty()):
          data = pre_start
        elif (pre_start.is_empty() == False):
          data = pl.concat([data, pre_start], how="vertical")
      if (end_date > cached_end): #type: ignore
        post_end: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=cached_end, #type: ignore
          end_date=end_date, frequency=frequency)
        print(post_end)
        if (data.is_empty()):
          data = post_end
        elif (post_end.is_empty() == False):
          data = pl.concat([data, post_end], how="vertical")
    # enforces uniqueness in table
    data = data.unique(subset=["date"], keep="first")
    data = data.sort("date")
    print(data)
    # creates table if not exists
    if (self.is_cached(symbol=symbol, frequency=frequency)):
      
    # caches data in table
    self.db_client.create_table(table_name=f"{symbol}_{frequency}", data=data)
    return data
  
if __name__ == "__main__":
  print(Data().get_data(symbol="GE", start_date=datetime.now() - timedelta(days=365), end_date=datetime.now(), frequency="1d"))
  print(Data().get_data(symbol="GE"))
