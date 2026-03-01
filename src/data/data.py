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

  def check_cached(self, symbol: str, frequency: str) -> tuple[datetime, datetime]:
    table: str = f"{symbol}_{frequency}"
    start_data: dict = self.db_client.retrieve_first(table_name=table)
    last_data: dict = self.db_client.retrieve_last(table_name=table)
    start_date = start_data["date"]
    end_date = last_data["date"]
    return (start_date.astype(datetime), end_date.astype(datetime))
  
  def get_relevant_date(self, date: datetime, frequency: str):
    if ("min" in frequency):
    elif ("d" in frequency):
    elif ("m" in frequency):
    elif ("y" in frequency):
  
  def check_pre_cache(self, start_date: datetime, cached_date: datetime, frequency: str) -> bool:
    
    return True
  
  def check_post_cache(self, end_date: datetime, cached_date: datetime, frequency: str) -> bool:
    return True

  def get_data(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=(365 * 100))), 
               end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
    table: str = f"{symbol}_{frequency}"
    if (self.is_cached(symbol=symbol, frequency=frequency) == True):
      # nothing is cached (or one point is cached)
      data: pl.DataFrame = self.data_client.get_tickers(
        symbol=symbol, start_date=start_date,
        end_date=end_date, frequency=frequency)
      self.db_client.create_table(table_name=table, data=data)
      return data
    else:
      # there is a range of dates
      cached_start, cached_end = self.check_cached(symbol=symbol, frequency=frequency)
      data: pl.DataFrame = pl.DataFrame()
      if (start_date < cached_start) and (end_date > cached_end):
        # cache before and after period
        pre_start: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=start_date,
          end_date=cached_start, frequency=frequency)
      elif (start_date < cached_start):
        # cache before

      elif (end_date > cached_end):
        # cache after

      if (start_date < cached_start):
        pre_start: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=start_date,
          end_date=cached_start, frequency=frequency) 
        if (data.is_empty()):
          data = pre_start
        elif (pre_start.is_empty() == False):
          data = pl.concat([data, pre_start], how="vertical")
      if (end_date > cached_end): #type: ignore
        post_end: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=cached_end, #type: ignore
          end_date=end_date, frequency=frequency)
        if (data.is_empty()):
          data = post_end
        elif (post_end.is_empty() == False):
          data = pl.concat([data, post_end], how="vertical")
    if (data.is_empty() == False):
      # enforces uniqueness in table
      data = data.unique(subset=["date"], keep="first")
      data = data.sort("date")
      # caches data in table
      self.db_client.insert_all(table_name=table, all_data=data.to_dicts())
    return self.db_client.retrieve_all(table_name=table)
  
if __name__ == "__main__":
  print(Data().get_data(symbol="HLT", start_date=datetime.now() - timedelta(days=365), end_date=datetime.now(), frequency="1d"))
  # print(Data().get_data(symbol="HLT"))
