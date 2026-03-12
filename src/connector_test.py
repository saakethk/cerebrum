""" Tests all data functions """
import unittest

from data.connector import *
from data.retrieval import *
from data.data import *

class TestConnector(unittest.TestCase):
  db_file: str = "data/duck.db"
  conn = DuckDB(db_file)
  test_table_name = "test"
  test_data = {
    "date": ["2020-10-20", "2024-11-19"],
    "stocks": ["AAPL", "META"],
    "recommendation": ["buy", "sell"]
  }
  test_insert_obj_1 = {
    "date": "2028-09-13",
    "stocks": "GOOGL",
    "recommendation": "sell"
  }
  test_insert_obj_2 = {
    "date": "2026-02-28",
    "stocks": "NVDA",
    "recommendation": "buy"
  }
  
  def test_creation(self):
    self.test_empty()
    num_tables_before = len(self.conn.get_tables())
    test_data = pl.DataFrame(self.test_data)
    self.conn.create_table(table_name=self.test_table_name, data=test_data)
    tables_after = self.conn.get_tables()
    num_tables_after = len(tables_after)
    self.assertEqual(num_tables_after, num_tables_before + 1)
    self.assertFalse(tables_after.filter(pl.col("name").str.contains(self.test_table_name)).is_empty())

  def test_retrieval(self):
    self.test_creation()
    results = self.conn.retrieve(table_name=self.test_table_name, key="stocks", val="GOOGL")
    self.assertEqual(len(results), 0)
    results = self.conn.retrieve(table_name=self.test_table_name, key="stocks", val="AAPL")
    self.assertEqual(len(results), 1)
    self.test_empty()

  def test_insert(self):
    self.test_creation()
    self.conn.insert(self.test_table_name, self.test_insert_obj_1)
    self.assertEqual(len(self.conn.retrieve_all(self.test_table_name)), 3)
    self.test_empty()

  def test_insert_multiple(self):
    self.test_creation()
    self.conn.insert_all(self.test_table_name, 
                         [self.test_insert_obj_1, self.test_insert_obj_2])
    self.assertEqual(len(self.conn.retrieve_all(self.test_table_name)), 4)
    self.test_empty()

  def test_retrieve_all(self):
    self.test_creation()
    result = self.conn.retrieve_all(table_name=self.test_table_name)
    self.assertEqual(len(result), 2)
    self.test_empty()

  def test_retrieve_first(self):
    self.test_creation()
    all_results = self.conn.retrieve_all(table_name=self.test_table_name)
    result = self.conn.retrieve_first(table_name=self.test_table_name)
    self.assertEqual(len(result), len(self.test_insert_obj_1))
    self.assertEqual(result, all_results.to_dicts()[0])
    self.test_empty()

  def test_retrieve_last(self):
    self.test_creation()
    all_results = self.conn.retrieve_all(table_name=self.test_table_name)
    result = self.conn.retrieve_last(table_name=self.test_table_name)
    self.assertEqual(len(result), len(self.test_insert_obj_1))
    self.assertEqual(result, all_results.to_dicts()[-1])
    self.test_empty()

  def test_empty(self):
    self.conn.delete_all(table_name=self.test_table_name)
    self.assertFalse(self.test_table_name in self.conn.get_tables())

if __name__ == '__main__':
  unittest.main()