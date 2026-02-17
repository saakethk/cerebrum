""" Get necessary relevant data """
from openbb import obb
import pandas as pd
import sqlite3
import sqlite_utils
from random import randint
from tqdm import tqdm

class Database:
    # Ref: https://docs.python.org/3/library/sqlite3.html
    DATABASE_LOC: str = "cache/cerebrum.sqlite"

    @staticmethod
    def create_table(dataframe: pd.DataFrame, table_name: str) -> None:
        connection = sqlite3.connect(Database.DATABASE_LOC)
        dataframe.to_sql(table_name, connection, if_exists="replace")
        connection.close()
      
    @staticmethod
    def get_tables() -> list[str]:
        db = sqlite_utils.Database(Database.DATABASE_LOC)
        return db.table_names()
    
    @staticmethod
    def get_table(table_name: str) -> pd.DataFrame:
        # Ref: https://datacarpentry.github.io/python-ecology-lesson/instructor/09-working-with-sql.html
        connection = sqlite3.connect(Database.DATABASE_LOC)
        df = pd.read_sql_query(f"SELECT * from '{table_name}'", connection)
        df.index = pd.to_datetime(df["date"], format='mixed')
        connection.close()
        return df

class Data:
    
    @staticmethod
    def cache_price_data(symbol: str) -> None:
        # Ref: https://docs.openbb.co/python/reference
        output = obb.equity.price.historical(symbol=symbol, start_date="1901-12-31", end_date="2026-01-01", interval="1d", provider="yfinance") # type: ignore
        output_df = output.to_dataframe()
        Database.create_table(dataframe=output_df, table_name=symbol)

    @staticmethod
    def get_cached_symbols() -> list[str]:
        return Database.get_tables()

    @staticmethod
    def get_relevant_symbols(filename: str) -> list[str]:
        # Reads file and extracts relevant symbols
        with open(filename, "r") as file:
            symbols = []
            not_done = True
            while not_done:
                content = file.readline()
                if (content == ""):
                    not_done = False
                    continue
                symbols.append(content.split(",")[0])
            return symbols[1:]
        return []

    @staticmethod
    def get_symbol_data(symbol: str) -> pd.DataFrame:
        tables = Database.get_tables()
        if symbol not in tables:
            Data.cache_price_data(symbol=symbol)
        return Database.get_table(table_name=symbol)
    
    @staticmethod
    def get_symbol_returns(symbol: str) -> pd.DataFrame:
        symbol_data = Data.get_symbol_data(symbol=symbol)
        symbol_data["symbol"] = symbol
        symbol_data["day_returns"] = symbol_data["close"].pct_change()
        symbol_data = symbol_data.dropna()
        return symbol_data[["date", "symbol", "day_returns"]]
    
    @staticmethod
    def get_symbols_returns(symbols: list[str], length: int) -> pd.DataFrame:
        # Length is the number of past days to get for each stock
        all_data: pd.DataFrame = pd.DataFrame()
        for symbol in tqdm(symbols, desc="Downloading Data"):
            try:
                stock_data = Data.get_symbol_returns(symbol=symbol)[-length:]
                all_data = pd.concat([all_data, stock_data], ignore_index=True)
            except Exception as error:
                print(error)
                continue
        return all_data
    
    @staticmethod
    def get_pivot_data(symbols: list[str], length: int) -> pd.DataFrame:
        # Returns wide version of get_prices_data
        data = Data.get_symbols_returns(symbols=symbols, length=length)
        pivot = data.pivot(index="symbol", columns="date", values="day_returns")
        pivot = pivot.ffill()
        pivot = pivot.dropna(axis=1)
        return pivot
        
    @staticmethod
    def get_subset(symbol: str, seq_size: int) -> tuple[pd.DataFrame, pd.Series]:
        # Returns the random sample from the data and the actual percent change the next day
        data: pd.DataFrame = Data.get_symbol_data(symbol)
        total_len: int = len(data)
        start_index = randint(0, total_len - seq_size - 2)
        seq = data.iloc[start_index : start_index + seq_size + 1]
        return seq[:seq_size], seq.iloc[-1]

if __name__ == "__main__":
    interest_stocks = ["NVDA", "GOOGL", "AAPL", "MSFT", "AMZN", "META"]
    # for stock in interest_stocks:
    #     print(Data.get_price_data(symbol=stock))
    # print(Data.get_pivot_data(symbols=interest_stocks, length=250))
    print(Data.get_relevant_symbols(filename="cache/SP500.csv"))

