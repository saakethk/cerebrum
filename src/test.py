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

  def test_empty(self):
    self.conn.delete_all(table_name=self.test_table_name)
    self.assertFalse(self.test_table_name in self.conn.get_tables())
  
  def test_creation(self):
    self.test_empty() # To empty before creation
    num_tables_before = len(self.conn.get_tables())
    test_data = pl.DataFrame(self.test_data)
    self.conn.create_table(table_name=self.test_table_name, data=test_data)
    tables_after = self.conn.get_tables()
    num_tables_after = len(tables_after)
    self.assertEqual(num_tables_after, num_tables_before + 1)
    self.assertFalse(tables_after.filter(pl.col("name").str.contains(self.test_table_name)).is_empty())

  def test_retrieval(self):
    pass

  def test_insert(self):
    pass

  def test_insert_multiple(self):
    pass

  def test_retrieve(self):
    pass

  def test_retrieve_first(self):
    pass

  def test_retrieve_all(self):
    pass


if __name__ == '__main__':
  unittest.main()