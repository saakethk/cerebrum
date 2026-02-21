""" All the code necessary to get stock data """
from datetime import datetime, timedelta
from openbb import obb
import polars as pl
from typing import Literal

class Source:
    
    def get_source_tickers(self, symbol: str, start_date: datetime, 
                        end_date: datetime, frequency: str, 
                        source: Literal['fmp', 'intrinio', 'polygon', 'tiingo', 'yfinance']) -> pl.DataFrame:
        """
        Docstring for get_source_tickers
        
        :param symbol: Stock ticker to get data for. (Ex. AAPL)
        :type symbol: str
        :param start_date: Start date of when to start getting data as python datetime object.
        :type start_date: datetime
        :param end_date: End date of when to stop getting data as python datetime object.
        :type end_date: datetime
        :param frequency: Interval by which to get stock ticker data. (Ex. 1m, 5m, ect.)
        :type frequency: str
        :param source: The source from which to get data from. Limited number supported currently due to openbb backend.
        :type source: Literal['fmp', 'intrinio', 'polygon', 'tiingo', 'yfinance']
        :return: Returns a polar dataframe (Different from pandas dataframe).
        :rtype: Polar DataFrame
        """
        output = obb.equity.price.historical( # type: ignore
            symbol=symbol, 
            start_date=start_date.strftime("%Y-%m-%d"), 
            end_date=end_date.strftime("%Y-%m-%d"), 
            interval=frequency, 
            provider=source)
        return output.to_polars()
    
class YFinance(Source):
    
    def get_tickers(self, symbol: str, start_date: datetime = (datetime.now() - timedelta(days=(365 * 100))), 
                    end_date: datetime = datetime.now(), frequency: str = "1d") -> pl.DataFrame:
        """
        Docstring for get_tickers
        
        :param symbol: Stock ticker to get data for. (Ex. AAPL)
        :type symbol: str
        :param start_date: Start date of when to start getting data as python datetime object.
        :type start_date: datetime
        :param end_date: End date of when to stop getting data as python datetime object.
        :type end_date: datetime
        :param frequency: Interval by which to get stock ticker data. (Ex. 1m, 5m, ect.)
        :type frequency: str
        :return: Returns a polar dataframe (Different from pandas dataframe).
        :rtype: Polar DataFrame
        """
        return super().get_source_tickers(symbol=symbol, start_date=start_date, 
                                       end_date=end_date, frequency=frequency, source="yfinance")
    
if __name__ == "__main__":
    end_date = datetime.now()
    start_date = end_date - timedelta(days=365)
    print(YFinance().get_tickers(symbol="AAPL"))






