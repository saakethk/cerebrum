""" Tests all data functions """
import unittest

from data.connector import *
from data.retrieval import *
from data.data import *

class TestRetrieval(unittest.TestCase):
  retrieve_obj = YFinance()
  
  def test_get_tickers(self):
    tickers = self.retrieve_obj.get_tickers(symbol="AAPL")
    self.assertNotEqual(tickers.shape[0], 0)

  def test_valid_timestamp(self):
    invalid_timestamp = datetime(year=2026, month=3, day=22, hour=0)
    valid_timestamp = datetime(year=2026, month=3, day=20, hour=10)

    is_valid = self.retrieve_obj.check_valid_timestamp(invalid_timestamp)
    self.assertEqual(is_valid, False)

    is_valid = self.retrieve_obj.check_valid_timestamp(valid_timestamp)
    self.assertEqual(is_valid, True)

if __name__ == '__main__':
  unittest.main()