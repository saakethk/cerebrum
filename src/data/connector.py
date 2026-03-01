""" All code necessary to connect to database and perform basic read, write, and retrieval commands """
import duckdb
import os
from abc import ABC
import polars as pl
import numpy as np
from numpy.typing import NDArray

class Database(ABC):

  def check_path(self, path: str):
    dir = os.path.dirname(path)
    if dir == None:
      # Makes directory
      os.makedirs(dir, exist_ok=True)
    return path
  
  def get_tables(self) -> pl.DataFrame:
    return pl.DataFrame()
  
  def create_table(self, table_name: str, data: pl.DataFrame) -> None:
    return None

  def insert(self, table_name: str, data: dict) -> None:
    return None

  def insert_all(self, table_name: str, all_data: list[dict]) -> None:
    return None

  def retrieve(self, table_name: str, key: str, val: str) -> pl.DataFrame:
    return pl.DataFrame()

  def retrieve_last(self, table_name: str) -> dict:
    return {}
  
  def retrieve_first(self, table_name: str) -> dict:
    return {}

  def retrieve_all(self, table_name: str) -> pl.DataFrame:
    return pl.DataFrame()

class DuckDB(Database):

  def __init__(self, path: str = "data.duckdb"):
    self.path = self.check_path(path)
    self.conn = duckdb.connect(database=self.path)

  def get_tables(self) -> pl.DataFrame:
    query: str = f"SHOW TABLES"
    return self.conn.execute(query).pl()

  def create_table(self, table_name: str, data: pl.DataFrame) -> None:
    query: str = f"CREATE TABLE IF NOT EXISTS {table_name} AS SELECT * from data"
    self.conn.execute(query)
    self.conn.execute(f"ALTER TABLE {table_name} ADD PRIMARY KEY (date)")

  def insert(self, table_name: str, data: dict) -> None:
    struct: list = []
    value: list = []
    for key, item in data.items():
      # Finds structure of data
      struct.append(key)
      if type(item) not in [int, float]:
        value.append(f"'{str(item)}'")
      else:
        value.append(str(item))
    query: str = f"""
    INSERT INTO {table_name} ({",".join(struct)}) VALUES ({",".join(value)}) ON CONFLICT (date)
    DO UPDATE SET
      date = EXCLUDED.date,
      open = EXCLUDED.open,
      high = EXCLUDED.high,
      low = EXCLUDED.low,
      close = EXCLUDED.close,
      volume = EXCLUDED.volume;"""
    self.conn.execute(query)
    
  def insert_all(self, table_name: str, all_data: list[dict]) -> None:
    for data in all_data:
      self.insert(table_name=table_name, data=data)
  
  def retrieve(self, table_name: str, key: str, val: str) -> pl.DataFrame:
    query: str = f"SELECT * FROM {table_name} WHERE {key} = {val}"
    return self.conn.sql(query).pl()
  
  def retrieve_last(self, table_name: str) -> dict:
    query: str = f"SELECT * FROM {table_name} ORDER BY date DESC LIMIT 1"
    result: dict = self.conn.sql(query).fetchnumpy()
    last_item: dict = {}
    for key, value in result.items():
      last_item[key] = value[-1]
    return last_item
  
  def retrieve_first(self, table_name: str) -> dict:
    query: str = f"SELECT * FROM {table_name} ORDER BY date LIMIT 1"
    result: dict = self.conn.sql(query).fetchnumpy()
    last_item: dict = {}
    for key, value in result.items():
      last_item[key] = value[-1]
    return last_item
  
  def retrieve_all(self, table_name: str) -> pl.DataFrame:
    query: str = f"SELECT * FROM {table_name}"
    return self.conn.sql(query).pl()

  