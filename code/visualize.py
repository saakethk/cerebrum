""" Visualize the environment for reinforcement learning """
from dash import Dash, html, dcc, Output, Input, ctx, State
import dash_ag_grid as dag
import plotly.express as px
import pandas as pd
from random import randint

from data import Data

app = Dash()

def get_rand_dataset(data: pd.DataFrame, seq_size: int) -> tuple[pd.DataFrame, pd.Series]:
    # Returns the random sample from the data and the actual percent change the next day 
    total_len: int = len(data)
    start_index = randint(0, total_len - seq_size - 2)
    seq = data.iloc[start_index : start_index + seq_size + 1]
    return seq[:seq_size], seq.iloc[-1]

stock_data = Data.get_price_data("AAPL")
data_subset, expected_val = get_rand_dataset(data=stock_data, seq_size=7)

app.layout = [
    html.H1(children='Data Visualization', style={"text-align": "center"}),
    html.Hr(),
    dcc.Store(id="num_correct", data=0),
    dcc.Graph(id="price_graph"),
    dcc.Graph(id="percent_graph"),
    html.Div(children=[
        html.Div(children=[
            dcc.Graph(id="price_graph")
        ],
        style={"width": "100%"}),
        html.Div(children=[
            dcc.Graph(id="percent_graph")
        ],
        style={"width": "100%"})],
        style={"display": "inline-flex"}
    ),
    html.Button('Buy', id='buy_btn', n_clicks=0),
    html.Button('Sell', id='sell_btn', n_clicks=0),
    html.Button('Hold', id='hold_btn', n_clicks=0),
    # dag.AgGrid(
    #     rowData=data_subset.to_dict('records'),
    #     columnDefs=[{"field": i} for i in data_subset.columns]
    # ),
    html.Div(id="status")
]

@app.callback(
    Output('status', 'children'),
    Output('num_correct', 'data'),
    Output('price_graph', 'figure'),
    Output('percent_graph', 'figure'),
    State('num_correct', 'data'),
    Input('buy_btn', 'n_clicks'),
    Input('sell_btn', 'n_clicks'),
    Input('hold_btn', 'n_clicks')
)
def update_graph(num_correct, b_clicks, s_clicks, h_clicks):
    stock_data = Data.get_price_data("AAPL")
    data_subset, expected_val = get_rand_dataset(data=stock_data, seq_size=7)
    price_fig = px.line(data_subset, x="date", y="close", title="Price Graph")
    percent_fig = px.line(data_subset, x="date", y="pct_change", title="% Change Graph")
    new_num_correct = num_correct
    total_clicks = b_clicks + s_clicks + h_clicks
    if ("buy_btn" == ctx.triggered_id) and (expected_val["encoded_buy"] == 1):
        # Buy the stock
        new_num_correct += 1
    if ("sell_btn" == ctx.triggered_id) and (expected_val["encoded_sell"] == 1):
        # Sell the stock
        new_num_correct += 1
    if ("hold_btn" == ctx.triggered_id) and (expected_val["encoded_hold"] == 1):
        # Hold the stock
        new_num_correct += 1
    return f"Num_Correct: {new_num_correct} / {total_clicks}", new_num_correct, price_fig, percent_fig

if __name__ == '__main__':
    app.run(debug=True)