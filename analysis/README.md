# Key Findings
This document is for me to list key finding I come across during exploratory analysis of the data.

- Stock: META
  - This stock appears to have a right-skewed volume
  - Applying a log transformation helped to make it more normal
    - Applying a inverse and sqrt did not help as much
  - The pct_change distirbution is pretty normal and it appears that it can be fit to a standard scaling of normal distribution
    - This means we can create a label for anomalies (statistically significant price movements) - we can label them based on distribution
    - standard scaler converts everything to a z-score (# of standard deviation above or below median)