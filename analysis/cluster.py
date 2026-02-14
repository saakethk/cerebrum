""" Model for time-series clustering with rolling window """
import pandas as pd
import numpy as np
from tslearn.preprocessing import TimeSeriesScalerMeanVariance
from tslearn.clustering import TimeSeriesKMeans
from typing import Any

from data import Data

class Cluster:
    
    @staticmethod
    def scale_data(data):
        scaler = TimeSeriesScalerMeanVariance()
        data_scaled = scaler.fit_transform(data)
        return data_scaled
    
    @staticmethod
    def run_model(scaled_data, num_clusters: int, window_size: int) -> tuple[TimeSeriesKMeans, Any]:
        model = TimeSeriesKMeans(
            n_clusters=num_clusters,
            metric="dtw",
            max_iter=25,
            metric_params={"sakoe_chiba_radius": int(window_size * 0.1)},  # speed constraint
            random_state=0,
            n_jobs=-1
        )
        labels = model.fit_predict(scaled_data)
        return model, labels

    @staticmethod
    def perform_static_analysis(pivot: pd.DataFrame, cluster_size: int) -> tuple[Any, TimeSeriesKMeans]:
        # pivot - wide format table with stock, data, and prices
        model_inputs = pivot.values[:, :, np.newaxis] # Converts to tslearn format (# stocks, # dates, # features)
        scaled_inputs = Cluster.scale_data(data=model_inputs)
        model, labels = Cluster.run_model(scaled_data=scaled_inputs, num_clusters=cluster_size, window_size=int(model_inputs.shape[1]))
        return labels, model
  
    @staticmethod
    def compute_cross_sectional_entropy(labels):
        # Shannon entropy
        _, counts = np.unique(labels, return_counts=True)
        p = counts / counts.sum()
        return -(p * np.log(p)).sum()
    
    @staticmethod
    def perform_rolling_analysis(pivot: pd.DataFrame, window_size: int, step_size: int, cluster_size: int) -> tuple[pd.DataFrame, pd.DataFrame]:
        # Performs static analysis on rolling window
        dates = pivot.columns
        num_dates = len(dates)
        results = []
        for start_index in range(0, num_dates - window_size, step_size):
            end_index = start_index + window_size
            relevant_pivot = pivot.iloc[:, start_index:end_index]
            labels, model = Cluster.perform_static_analysis(pivot=relevant_pivot, cluster_size=cluster_size)
            results.append({
                "date": dates[end_index],
                "labels": labels,
                "model": model
            })
        # Convert results to pandas dataframe
        rolling_analysis = pd.DataFrame(
            {r["date"]: r["labels"] for r in results},
            index=pivot.index
        )
        entropy_series = rolling_analysis.apply(Cluster.compute_cross_sectional_entropy, axis=0)
        return rolling_analysis, entropy_series
    
if __name__ == "__main__":
    interest_stocks = Data.get_relevant_symbols(filename="cache/SP500.csv")
    pivot_data = Data.get_pivot_data(symbols=interest_stocks, length=250)
    # labels, model = Cluster.perform_static_analysis(pivot=pivot_data)
    data = Cluster.perform_rolling_analysis(pivot=pivot_data, window_size=60, step_size=5, cluster_size=50)
    print(data)
    

        



        
        



