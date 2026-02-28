""" Tests all data functions """
import unittest

from data.connector import *
from data.retrieval import *

class TestData(unittest.TestCase):
  db_file: str = "data/duck.db"
  
  def test_yfinance_retrieval(self):
    source = YFinance()
    data = source.get_tickers(symbol="AAPL")
    self.assertNotEqual(len(data), 0)

  def test_retrieve_data(self):
    conn = DuckDB(path=self.db_file)
    source = YFinance()
    data = source.get_tickers(symbol="AAPL")
    self.assertNotEqual(len(data), 0)
    conn.create_table(table_name="AAPL", data=data)
    results = conn.retrieve_all("AAPL")
    self.assertNotEqual(len(results), 0)

  def test_db_existence(self):
    conn = DuckDB(path=self.db_file)
    self.assertNotEqual(len(conn.get_tables().to_dicts()), 0)
  
  def test_connector(self):
    conn = DuckDB(path=self.db_file)
    source = YFinance()
    data = source.get_tickers(symbol="AAPL")
    all_results = conn.retrieve_all("AAPL")
    self.assertNotEqual(len(all_results), 0)
    results = conn.retrieve_last("AAPL")
    self.assertEqual(len(results), 8)
    before_results = conn.retrieve_all("AAPL")
    conn.insert("AAPL", data.to_dicts()[0])
    after_results = conn.retrieve_all("AAPL")
    self.assertEqual(len(after_results), len(before_results) + 1)
    before_results = conn.retrieve_all("AAPL")
    conn.insert_all("AAPL", data.to_dicts()[0:5])
    after_results = conn.retrieve_all("AAPL")
    self.assertEqual(len(after_results), len(before_results) + 5)


if __name__ == '__main__':
  unittest.main()