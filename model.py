""" Testing different model types """
import numpy as np
import pandas as pd
import tensorflow as tf
# from tensorflow.keras.models import Sequential, load_model
# from tensorflow.keras.layers import Dense, LSTM
from sklearn.preprocessing import MinMaxScaler
print(tf.__version__)

# # Load the data
# data = pd.read_csv('time_series_data.csv', index_col='Date', parse_dates=True)
# series = data['Value'].values

# # Preprocess the data
# scaler = MinMaxScaler(feature_range=(0, 1))
# scaled_series = scaler.fit_transform(series.reshape(-1, 1))

# # Prepare the data for LSTM
# def create_dataset(series, time_step=1):
# X, y = [], []
# for i in range(len(series) - time_step - 1):
# X.append(series[i:(i + time_step), 0])
# y.append(series[i + time_step, 0])
# return np.array(X), np.array(y)

# time_step = 10
# X, y = create_dataset(scaled_series, time_step)
# X = X.reshape(X.shape[0], X.shape[1], 1)

# # Build the LSTM model
# model = Sequential()
# model.add(LSTM(units=50, return_sequences=True, input_shape=(time_step, 1)))
# model.add(LSTM(units=50))
# model.add(Dense(1))
# model.compile(optimizer='adam', loss='mean_squared_error')

# # Train the model
# model.fit(X, y, epochs=20, batch_size=32)

# # Forecast future values
# forecast = model.predict(X[-10:])

# # Inverse transform to original scale
# forecast = scaler.inverse_transform(forecast)

# print("Forecasted values:", forecast)
