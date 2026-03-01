""" All the code necessary to get stock data """
from datetime import datetime, timedelta
from openbb import obb
import polars as pl
from typing import Literal

class Source:
    
  def get_source_tickers(self, symbol: str, start_date: datetime, 
                      end_date: datetime, frequency: str, 
                      source: Literal['fmp', 'intrinio', 'polygon', 'tiingo', 'yfinance']) -> pl.DataFrame:
    output = obb.equity.price.historical( # type: ignore
        symbol=symbol, 
        start_date=start_date.strftime("%Y-%m-%d"), 
        end_date=end_date.strftime("%Y-%m-%d"), 
        interval=frequency, 
        provider=source,
        actions=False)
    return output.to_polars()
  
class YFinance(Source):
    
  def get_tickers(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=(365 * 100))), 
                  end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
    return super().get_source_tickers(
      symbol=symbol, start_date=start_date, end_date=end_date, 
      frequency=frequency, source="yfinance")["date", "open", "high", "low", "close", "volume"]
