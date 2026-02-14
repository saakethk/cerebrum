# Key Findings
This document is for me to list key finding I come across during exploratory analysis of the data.

- Stock: META
  - This stock appears to have a right-skewed volume
  - Applying a log transformation helped to make it more normal
    - Applying a inverse and sqrt did not help as much
    - Can apply standard scaler on it after to make it cleaner
  - The pct_change distirbution is pretty normal and it appears that it can be fit to a standard scaling of normal distribution
    - This means we can create a label for anomalies (statistically significant price movements) - we can label them based on distribution
    - standard scaler converts everything to a z-score (# of standard deviation above or below median)
  - Idea is to first predict days on which significant price changes will occur
    - Create model to classify whether conditions on these significant price days will lead to a buy or sell
    - Can also have model which just predicts if price will go up or down tommorow

- For data processing tslearn has a more efficient implementation of what I manually implemented
  - Time Series Scaler Mean Variance
    - Z-score based scaler
      - Default: mean = 0, standard deviation = 1
  - For clustering there are two types for long term clustering
    - Static (Set window) - What I did in clustering.ipynb
    - Rolling window (Tests multiple windows and shows changes in grouping over time)
  - Metric to measure similarity: https://www.geeksforgeeks.org/machine-learning/dynamic-time-warping-dtw-in-time-series/

- Regime detection is process of identifying distinct states in market
  - Regime Detection: https://medium.com/lseg-developer-community/market-regime-detection-using-statistical-and-ml-based-approaches-b4c27e7efc8b
  - Computing cross sectional entropy can help us to distinguish whether market is ordered or unordered
    - Indicates whether clusters are evenly distributed or unevenely distributed
      - Even distribution indicates unordered distribution
      - Uneven distribution indicates ordered distribution
  