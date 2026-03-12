""" All the code necessary to get stock data """
from datetime import datetime, timedelta
from openbb import obb
import polars as pl
from typing import Literal
import pandas as pd
import pandas_market_calendars as mcal

class Source:

  def __init__(self, print_logs = True):
    self.cached_start = None
    self.cached_end = None
    self.cached_calendar = mcal.get_calendar('NYSE')
    self.print_logs = print_logs

  def log(self, msg: str):
    # for logging caching status
    if (self.print_logs):
      print(msg)

  def filter_core(self, data: pd.DataFrame):
    # gets columns considered core for quantitative trading
    return data["date", "open", "high", "low", "close", "volume"]
    
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
    return output.to_polars()
  
  def check_valid_timestamp(self, timestamp: datetime) -> bool:
    # TODO: https://pandas-market-calendars.readthedocs.io/en/latest/usage.html
    # for a day to be valid it must be present already and have a date object
    schedule = self.cached_calendar.schedule( # Can make this more efficient
      start_date=timestamp.strftime("%Y-%m-%d"), 
      end_date=timestamp.strftime("%Y-%m-%d"))
    ts = pd.Timestamp(timestamp, tz="America/New_York")
    try:
      return self.cached_calendar.open_at_time(schedule, ts)
    except Exception as error:
      self.log("[source] day outside of available period encountered")
      return False
  
class YFinance(Source):
    
  def get_tickers(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=(365 * 100))), 
                  end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
    data = super().get_source_tickers(
      symbol=symbol, start_date=start_date, end_date=end_date, 
      frequency=frequency, source="yfinance")
    return self.filter_core(data=data) #type: ignore
  
if __name__ == "__main__":
  test = YFinance()
  print(test.get_tickers(symbol="AAPL", frequency="1h"))
  test_date = datetime.now() - timedelta(days=1, hours=6)
  print(test_date)
  print(test.check_valid_timestamp(timestamp=test_date))
