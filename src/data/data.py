""" Outward facing class that can actually be used for data retrieval """
import polars as pl
from datetime import datetime, timedelta

from data.connector import DuckDB
from data.retrieval import YFinance

class Data():

  def __init__(self):
    self.data_client = YFinance()
    self.db_client = DuckDB(path="data/duck.db")
    self.print_logs = True

  def log(self, msg: str):
    # for logging caching status
    if (self.print_logs):
      print(msg)

  def get_cached(self) -> list[str]:
    # returns list of already cached data
    table_names = [table["name"] for table in self.db_client.get_tables().to_dicts()]
    return table_names

  def is_cached(self, symbol: str, frequency: str) -> bool:
    # returns if data is cached at all
    table: str = f"{symbol}_{frequency}"
    table_names = self.get_cached()
    if (table in table_names):
      return True
    return False

  def check_cached(self, symbol: str, frequency: str) -> tuple[datetime, datetime]:
    # checks bounds of cached data (requires checking before)
    table: str = f"{symbol}_{frequency}"
    start_data: dict = self.db_client.retrieve_first(table_name=table)
    last_data: dict = self.db_client.retrieve_last(table_name=table)
    start_date = start_data["date"]
    end_date = last_data["date"]
    return (start_date.astype(datetime), end_date.astype(datetime))
  
  def empty_cache(self, symbol: str, frequency: str) -> bool:
    # checks bounds of cached data (requires checking before)
    table: str = f"{symbol}_{frequency}"
    table_names = self.get_cached()
    if table not in table_names:
      return False
    self.db_client.delete_all(table_name=table)
    return True
  
  def empty_full_cache(self) -> bool:
    # emptys full cache
    status = True
    table_names = self.get_cached()
    for table in table_names:
      symbol, frequency = table.split("_")
      status = status and self.empty_cache(symbol=symbol, frequency=frequency)
    return status
  
  def get_relevant_date(self, date: datetime, frequency: str):
    # gets relevant part of date
    if ("min" in frequency):
      date = date.replace(microsecond=0, second=0)
    elif ("d" in frequency) or ("m" in frequency) or ("y" in frequency):
      date = date.replace(microsecond=0, second=0, hour=0, minute=0)
    return date
  
  def check_pre_cache(self, start_date: datetime, cached_date: datetime, frequency: str) -> bool:
    relevant_start_date: datetime = self.get_relevant_date(date=start_date, frequency=frequency)
    relevant_compare_date: datetime = self.get_relevant_date(date=cached_date, frequency=frequency)
    if (relevant_start_date < relevant_compare_date):
      # data needs to be cached
      return True
    # data is already cached
    return False
  
  def check_post_cache(self, end_date: datetime, cached_date: datetime, frequency: str) -> bool:
    relevant_end_date: datetime = self.get_relevant_date(date=end_date, frequency=frequency)
    relevant_compare_date: datetime = self.get_relevant_date(date=cached_date, frequency=frequency)
    if (relevant_end_date > relevant_compare_date):
      # data needs to be cached
      return True
    # data is already cached
    return False

  def get_data(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=(365 * 100))), 
               end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
    table: str = f"{symbol}_{frequency}"
    if (self.is_cached(symbol=symbol, frequency=frequency) == False):
      self.log(f"[cache] data retrived from {start_date} to {end_date}")
      # nothing is cached (or one point is cached)
      data: pl.DataFrame = self.data_client.get_tickers(
        symbol=symbol, start_date=start_date,
        end_date=end_date, frequency=frequency)
      self.db_client.create_table(table_name=table, data=data)
    else:
      # there is a range of dates
      cached_start, cached_end = self.check_cached(symbol=symbol, frequency=frequency)
      self.log(f"[cache] data cached from {cached_start} to {cached_end}")
      pre_cache_necessary: bool = self.check_pre_cache(
        start_date=start_date, cached_date=cached_start, frequency=frequency)
      post_cache_necessary: bool = self.check_post_cache(
        end_date=end_date, cached_date=cached_end, frequency=frequency)
      if (pre_cache_necessary and post_cache_necessary):
        self.log(f"[cache] retrieved data before from {start_date} to {cached_start}")
        self.log(f"[cache] retrieved data after from {cached_end} to {end_date}")
        pre_start: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=start_date,
          end_date=cached_start, frequency=frequency)
        post_end: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=cached_end,
          end_date=end_date, frequency=frequency)
        all_data = pl.concat([pre_start, post_end], how="vertical").unique(subset=["date"], keep="first").sort("date")
        self.db_client.insert_all(table_name=table, all_data=all_data.to_dicts())
      elif (pre_cache_necessary):
        self.log(f"[cache] retrieved data before from {cached_start} to {start_date}")
        pre_start: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=start_date,
          end_date=cached_start, frequency=frequency)
        self.db_client.insert_all(table_name=table, all_data=pre_start.to_dicts())
      elif (post_cache_necessary):
        self.log(f"[cache] retrieved data before from {cached_end} to {end_date}")
        post_end: pl.DataFrame = self.data_client.get_tickers(
          symbol=symbol, start_date=cached_end,
          end_date=end_date, frequency=frequency)
        self.db_client.insert_all(table_name=table, all_data=post_end.to_dicts())
    return self.db_client.retrieve_all(table_name=table)
  
if __name__ == "__main__":
  # print(Data().get_data(symbol="SBUX", start_date=datetime.now() - timedelta(days=365), end_date=datetime.now(), frequency="1d"))
  print(Data().get_data(symbol="SBUX"))
  # Deal with edge case where start is after start and end is after end
  # print(Data().get_relevant_date(datetime.now(), "1d"))
  # print(Data().get_data(symbol="HLT"))
