""" All the code necessary to get stock data """
from datetime import datetime, timedelta
from openbb import obb
import polars as pl
from typing import Literal
import pandas as pd
import pandas_market_calendars as mcal

class Source:

  def __init__(self):
    self.cached_start = None
    self.cached_end = None
    
  def get_source_tickers(self, symbol: str, start_date: datetime, 
                      end_date: datetime, frequency: str, 
                      source: Literal['fmp', 'intrinio', 'polygon', 'tiingo', 'yfinance']) -> pl.DataFrame:
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
  
  def get_valid_day(self, date: datetime) -> bool:
    # TODO: https://pandas-market-calendars.readthedocs.io/en/latest/usage.html
    start: datetime = date
    end: datetime = date
    if self.cached_start != None:
      start = self.cached_start
    if self.cached_end != None:
      end = self.cached_end

    nyse = mcal.get_calendar('NYSE')
    early = nyse.schedule(start_date='2012-07-01', end_date='2012-07-10')
    nyse.open_at_time(early, pd.Timestamp('2012-07-03 12:00', tz='America/New_York'))
    return False
  
class YFinance(Source):
    
  def get_tickers(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=(365 * 100))), 
                  end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
    return super().get_source_tickers(
      symbol=symbol, start_date=start_date, end_date=end_date, 
      frequency=frequency, source="yfinance")["date", "open", "high", "low", "close", "volume"]
