""" All code necessary to connect to database and perform basic read, write, and retrieval commands """
import duckdb
import os
from abc import ABC
import polars as pl

class Database(ABC):

  def __init__(self, print_logs = True):
    self.__print_logs = print_logs

  def log(self, msg: str):
    # for logging caching status
    if (self.__print_logs):
      print(msg)

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
  
  def delete_all(self, table_name: str) -> None:
    return None

class DuckDB(Database):

  def __init__(self, path: str = "data.duckdb"):
    super().__init__()
    self.__path = self.check_path(path)
    self.__conn = duckdb.connect(database=self.__path)

  def get_tables(self) -> pl.DataFrame:
    query: str = f"SHOW TABLES"
    return self.__conn.execute(query).pl()

  def create_table(self, table_name: str, data: pl.DataFrame) -> None:
    if (table_name in self.get_tables()):
      self.log(f"[db] {table_name} already exists in cache")
      return
    query: str = f"CREATE TABLE IF NOT EXISTS {table_name} AS SELECT * from data"
    self.__conn.execute(query)
    self.__conn.execute(f"ALTER TABLE {table_name} ADD PRIMARY KEY (date)")

  def insert(self, table_name: str, data: dict) -> None:
    struct: list = []
    value: list = []
    update_set_str: list[str] = []
    for key, item in data.items():
      # Finds structure of data
      struct.append(key)
      if type(item) not in [int, float]:
        val = f"'{str(item)}'"
      else:
        val = f"{str(item)}"
      # Excluded just means you are updating with new values rather than keeping old
      update_set_str.append(f"{key} = EXCLUDED.{key}")
      value.append(val)
    query: str = f"""
    INSERT INTO {table_name} ({",".join(struct)}) VALUES ({",".join(value)}) ON CONFLICT (date)
    DO UPDATE SET
      {",".join(update_set_str)};"""
    self.__conn.execute(query)
    
  def insert_all(self, table_name: str, all_data: list[dict]) -> None:
    for data in all_data:
      self.insert(table_name=table_name, data=data)
  
  def retrieve(self, table_name: str, key: str, val: str) -> pl.DataFrame:
    if type(val) == str:
      val = f"\'{val}\'"
    query: str = f"SELECT * FROM {table_name} WHERE {key} = {val}" # Has to be single quotes
    try:
      return self.__conn.sql(query).pl()
    except Exception as error:
      print(error)
      self.log(f"[db] {val} not found in column \"{key}\"")
      return pl.DataFrame()
  
  def retrieve_last(self, table_name: str) -> dict:
    query: str = f"SELECT * FROM {table_name} ORDER BY date DESC LIMIT 1"
    result: dict = self.__conn.sql(query).fetchnumpy()
    last_item: dict = {}
    for key, value in result.items():
      last_item[key] = value[-1]
    return last_item
  
  def retrieve_first(self, table_name: str) -> dict:
    query: str = f"SELECT * FROM {table_name} ORDER BY date LIMIT 1"
    result: dict = self.__conn.sql(query).fetchnumpy()
    last_item: dict = {}
    for key, value in result.items():
      last_item[key] = value[-1]
    return last_item
  
  def retrieve_all(self, table_name: str) -> pl.DataFrame:
    query: str = f"SELECT * FROM {table_name} ORDER BY date"
    return self.__conn.sql(query).pl()
  
  def delete_all(self, table_name: str) -> None:
    query: str = f"DROP TABLE IF EXISTS {table_name}"
    self.__conn.execute(query)

  