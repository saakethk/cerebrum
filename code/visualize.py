""" Visualize the environment for reinforcement learning """
from dash import Dash, html, dcc, Output, Input, ctx, State
import dash_ag_grid as dag
import plotly.express as px
import pandas as pd
from random import randint

from data import Data

app = Dash()

data_subset, expected_val = Data.get_subset(symbol="AAPL", seq_size=7)

app.layout = [
    dcc.Store(id="points", data=0),
    dcc.Store(id="num_correct", data=0),
    html.Div(
        children=[
            html.Div(id="status", style={
                "padding": "1em", 
                "background": "black", 
                "color": "white", 
                "width": "fit-content",
                "margin": "auto",
                "margin-bottom": "0.5em"}),
            dcc.Graph(id="price_graph", style={"height": "300px", "padding": "0px"}),
            dcc.Graph(id="percent_graph", style={"height": "300px", "padding": "0px"}),
            dcc.Graph(id="volume_graph", style={"height": "300px", "padding": "0px"}),
            html.Div(
                children=[
                    html.Div(
                        children=[
                            html.Button('Buy', id='buy_btn', n_clicks=0, style={"padding": "1em", "background": "green", "color": "black"}),
                            html.Button('Hold', id='hold_btn', n_clicks=0, style={"padding": "1em", "background": "black", "color": "white"}),
                            html.Button('Sell', id='sell_btn', n_clicks=0, style={"padding": "1em", "background": "red", "color": "black"})
                        ]
                    ),
                    html.Div(
                        id="prev_status_check",
                        style={"padding": "0.5em"}
                    )
                ],
                style={"margin": "auto", "padding": "1em", "border": "1px solid black", "display": "flex", "justify-content": "space-between"}
            )
        ],
        style={"width": "100%", "max-width": "700px", "margin": "auto"}
    )
]

@app.callback(
    Output('prev_status_check', 'children'),
    Output('status', 'children'),
    Output('num_correct', 'data'),
    Output('price_graph', 'figure'),
    Output('percent_graph', 'figure'),
    Output('volume_graph', 'figure'),
    State('num_correct', 'data'),
    Input('buy_btn', 'n_clicks'),
    Input('sell_btn', 'n_clicks'),
    Input('hold_btn', 'n_clicks')
)
def update_graph(num_correct, b_clicks, s_clicks, h_clicks):
    data_subset, expected_val = Data.get_subset(symbol="AAPL", seq_size=7)
    price_fig = px.line(data_subset, x="date", y="close", title="Price Graph", template="plotly_dark")
    percent_fig = px.line(data_subset, x="date", y="pct_change", title="% Change Graph", template="plotly_dark")
    volume_fig = px.bar(data_subset, x="date", y="volume", title="Volume", template="plotly_dark")
    new_num_correct = num_correct
    total_clicks = b_clicks + s_clicks + h_clicks
    message = "You were wrong."
    if ("buy_btn" == ctx.triggered_id) and (expected_val["encoded_buy"] == 1):
        # Buy the stock
        message = "Buy was correct."
        new_num_correct += 1
    if ("sell_btn" == ctx.triggered_id) and (expected_val["encoded_sell"] == 1):
        # Sell the stock
        message = "Sell was correct."
        new_num_correct += 1
    if ("hold_btn" == ctx.triggered_id) and (expected_val["encoded_hold"] == 1):
        # Hold the stock
        message = "Hold was correct."
        new_num_correct += 1
    return message, f"Num_Correct: {new_num_correct} / {total_clicks} (Accuracy: {(new_num_correct / total_clicks if total_clicks != 0 else 0) * 100}%)", new_num_correct, price_fig, percent_fig, volume_fig

if __name__ == '__main__':
    app.run(debug=True)