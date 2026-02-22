# Regime detection
## Notes on how regime detection works for stocks and ideas for implementation
- Principal component analysis and feature extraction to capture variation in time series.
- Cluster analysis on the principal components to identify distinct groupings or regimes
- Unsupervised regime discovery on pca data to discover regimes without labels
- Treat discovered clusters as ground truth
  - Train classifier to assign new stocks to regimes
  - Avoids recomputing clusters
- k-means clustering with euclidean distance for classification
  - Average silhouette width for possible values of k is used
  - Metrics
    - Cumulative Return (%)
    - Annualized Expected Return (%)
    - Annualized Volatility (%)
    - Daily Return Skewness
    - Daily Return Kurtosis
    - Maximum Drawdown (%)

## Sources
1. https://medium.com/@rachita.pateria/an-introduction-to-markov-switching-model-for-time-series-b279e9e26125
2. https://arxiv.org/pdf/2108.05801