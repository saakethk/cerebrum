""" Get necessary relevant data """
from openbb import obb
import pandas as pd
import sqlite3
import sqlite_utils

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
        df = pd.read_sql_query(f"SELECT * from {table_name}", connection)
        df.index = pd.to_datetime(df["date"], format='mixed')
        connection.close()
        return df

class Data:
    
    @staticmethod
    def cache_price_data(symbol: str) -> None:
        # Ref: https://docs.openbb.co/python/reference
        output = obb.equity.price.historical(symbol=symbol, start_date="1901-12-31", end_date="2026-01-01", interval="1d", provider="yfinance")
        output_df = output.to_dataframe()
        output_df["pct_change"] = output_df["close"].pct_change()
        output_df["encoded_direction"] = output_df["pct_change"] > 0
        output_df["encoded_strength"] = ((output_df["pct_change"].abs() - output_df["pct_change"].mean()) * 100).round()
        output_df["encoded_vector_val"] = output_df["encoded_strength"] * output_df["encoded_direction"].map({True: 1, False: -1})
        output_df["encoded_buy"] = output_df["encoded_vector_val"] > 1
        output_df["encoded_sell"] = output_df["encoded_vector_val"] < -1
        output_df["encoded_hold"] = (output_df["encoded_buy"] == 0) & (output_df["encoded_sell"] == 0)
        Database.create_table(dataframe=output_df, table_name=symbol)

    @staticmethod
    def get_price_data(symbol: str) -> pd.DataFrame:
        tables = Database.get_tables()
        if symbol not in tables:
            Data.cache_price_data(symbol=symbol)
        return Database.get_table(table_name=symbol)

if __name__ == "__main__":
    interest_stocks = ["NVDA", "GOOGL", "AAPL", "MSFT", "AMZN", "META"]
    for stock in interest_stocks:
        Data.get_price_data(symbol=stock)
    # Analysis.visualize_data("META")

