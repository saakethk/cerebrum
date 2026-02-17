""" All the code necessary to get stock data """
from abc import ABC, abstractmethod
from datetime import datetime

class Source(ABC):
    
    @abstractmethod
    def get_ticker_data(self, start_date: datetime, end_date: datetime, frequency: int):
        """"""
