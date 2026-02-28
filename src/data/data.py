""" Outward facing class that can actually be used for data retrieval """
import polars as pl
from datetime import datetime, timedelta

from connector import Database
from retrieval import YFinance

class Data():
  data_client = YFinance()
  db_client = Database()

  def check_cached(self, symbol: str, start_date: datetime, 
                   end_time: datetime, frequency: str):
    start_data: dict = self.db_client.retrieve_first(table_name=symbol)
    last_data: dict = self.db_client.retrieve_last(table_name=symbol)

  def get_data(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=(365 * 100))), 
               end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
    data: pl.DataFrame = self.data_client.get_tickers(
      symbol=symbol, start_date=start_date,
      end_date=end_date, frequency=frequency)
    return data
