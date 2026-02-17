""" Model for time-series clustering with rolling window """
import pandas as pd
import numpy as np
from tslearn.preprocessing import TimeSeriesScalerMeanVariance
from tslearn.clustering import TimeSeriesKMeans
from typing import Any
from tqdm import tqdm
import multiprocessing
import time

from analysis.data import Data

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
    def perform_individual_rolling_analysis(data):
        # pivot: pd.DataFrame, start_index: int, end_index: int, cluster_size: int
        # Performs individual rolling analysis
        relevant_pivot = data[0].iloc[:, data[1]:data[2]]
        labels, model = Cluster.perform_static_analysis(pivot=relevant_pivot, cluster_size=data[3])
        return labels
    
    @staticmethod
    def perform_rolling_analysis(pivot: pd.DataFrame, window_size: int, step_size: int, cluster_size: int):
        # Performs static analysis on rolling window
        start_time = time.perf_counter()
        dates = pivot.columns
        num_dates = len(dates)
        date_inputs = []
        inputs = []
        for start_index in tqdm(range(0, num_dates - window_size, step_size), desc="Performing Rolling Analysis"):
            end_index = start_index + window_size
            inputs.append((pivot, start_index, end_index, cluster_size))
            date_inputs.append(dates[end_index])
            
        pool = multiprocessing.Pool()
        outputs = pool.map(Cluster.perform_individual_rolling_analysis, inputs)
        results = []
        for i in range(len(date_inputs)):
            results.append({
                "date": date_inputs[i],
                "labels": outputs[i]
            })
        # Convert results to pandas dataframe
        rolling_analysis = pd.DataFrame(
            {r["date"]: r["labels"] for r in results},
            index=pivot.index
        )
        print(rolling_analysis)
        rolling_analysis.to_csv("output.csv")
        end_time = time.perf_counter()
        print(end_time - start_time)
        # entropy_series = rolling_analysis.apply(Cluster.compute_cross_sectional_entropy, axis=0)
        # return rolling_analysis, entropy_series
    
if __name__ == "__main__":
    interest_stocks = Data.get_cached_symbols()
    pivot_data = Data.get_pivot_data(symbols=interest_stocks, length=250)
    print(pivot_data)
    # labels, model = Cluster.perform_static_analysis(pivot=pivot_data)
    data = Cluster.perform_rolling_analysis(pivot=pivot_data, window_size=60, step_size=5, cluster_size=50)
    print(data)
    

        



        
        



