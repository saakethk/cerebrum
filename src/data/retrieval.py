""" All the code necessary to get stock data """
from datetime import datetime, timedelta
from openbb import obb
import polars as pl
from typing import Literal
import pandas as pd
import pandas_market_calendars as mcal

class Source:

  def __init__(self, print_logs = True):
    self.__cached_start = None
    self.__cached_end = None
    self.__cached_calendar = mcal.get_calendar('NYSE')
    self.__print_logs = print_logs

  def __log(self, msg: str):
    # for logging caching status
    if (self.__print_logs):
      print(msg)

  def __filter_core(self, data: pl.DataFrame) -> pl.DataFrame:
    # gets columns considered core for quantitative trading
    return data[["date", "open", "high", "low", "close", "volume"]]
    
  def get_source_tickers(self, symbol: str, start_date: datetime, 
                      end_date: datetime, frequency: str, 
                      source: Literal['fmp', 'intrinio', 'polygon', 'tiingo', 'yfinance']) -> pl.DataFrame:
    # gets data using openbb (wrapper for its interface)
    self.cached_start = start_date
    self.cached_end = end_date
    output = obb.equity.price.historical( # type: ignore
        symbol=symbol, 
        start_date=start_date.strftime("%Y-%m-%d"), 
        end_date=end_date.strftime("%Y-%m-%d"), 
        interval=frequency, 
        provider=source,
        actions=False)
    return self.__filter_core(output.to_polars())
  
  def check_valid_timestamp(self, timestamp: datetime) -> bool:
    # Src: https://pandas-market-calendars.readthedocs.io/en/latest/usage.html
    # for a day to be valid it must be present already and have a date object
    
    if timestamp > datetime.now():
      # Checks that date is not after today
      return False
    
    schedule = self.__cached_calendar.schedule( # Can make this more efficient
      start_date=timestamp.strftime("%Y-%m-%d"), 
      end_date=timestamp.strftime("%Y-%m-%d"))
    ts = pd.Timestamp(timestamp, tz="America/New_York")
    try:
      return self.__cached_calendar.open_at_time(schedule, ts)
    except Exception as error:
      self.__log("[source] day outside of available period encountered")
      return False
  
class YFinance(Source):

  def __init__(self):
    super().__init__()
    
  def get_tickers(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=365)), 
                  end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
    # Gets last year of data by default with 1d frequency
    data = super().get_source_tickers(
      symbol=symbol, start_date=start_date, end_date=end_date, 
      frequency=frequency, source="yfinance")
    return data
  
if __name__ == "__main__":
  test = YFinance()
  print(test.get_tickers(symbol="AAPL", frequency="1h"))
  test_date = datetime.now() - timedelta(days=1, hours=6)
  print(test_date)
  print(test.check_valid_timestamp(timestamp=test_date))
