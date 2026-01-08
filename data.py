""" Get necessary relevant data """
from openbb import obb
import pandas as pd
import matplotlib.pyplot as plt
import sqlite3
import sqlite_utils

class Database:
    # Ref: https://docs.python.org/3/library/sqlite3.html
    DATABASE_LOC: str = "data/cerebrum.sqlite"

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
        dataframe = pd.read_sql_query(f"SELECT * from {table_name}", connection)
        connection.close()
        return dataframe

class Data:
    
    @staticmethod
    def get_price_data(symbol: str) -> None:
        # Ref: https://docs.openbb.co/python/reference
        output = obb.equity.price.historical(symbol=symbol, start_date="1901-12-31", end_date="2026-01-01", interval="1d", provider="yfinance")
        output_df = output.to_dataframe()
        output_df["pct_change"] = output_df["close"].pct_change()
        output_df["forecasted_pct_change"] = output_df["pct_change"].shift(-1)
        output_df["encoded_pct_change"] = output_df["pct_change"] > 0
        Database.create_table(dataframe=output_df, table_name=symbol)
    
class Analysis:
    
    @staticmethod
    def visualize_data(symbol: str) -> None:
        df = Database.get_table(table_name=symbol)
        df.index = pd.to_datetime(df["date"], format='mixed')
        plt.plot(df.index, df["encoded_pct_change"])
        plt.show()
        

if __name__ == "__main__":
    # Data.get_price_data(symbol="META")
    Analysis.visualize_data("META")

