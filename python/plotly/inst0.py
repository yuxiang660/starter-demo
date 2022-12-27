import plotly.express as px
import pandas as pd
import numpy as np

data = [
    {
        "Level0": "dut_mux",
        "Level1": "mux0",
        "LevelQT": [
            {"QtName": "U0(not)", "PowerW": 1, "PinCapW": 12.1},
            {"QtName": "V0(inv)", "PowerW": 1, "PinCapW": 12.1},
            {"QtName": "O0(xor)", "PowerW": 1, "PinCapW": 12.1},
        ]
    },
    {
        "Level0": "dut_mux",
        "Level1": "mux1",
        "LevelQT": [
            {"QtName": "U1(not)", "PowerW": 1, "PinCapW": 12.1},
            {"QtName": "V1(inv)", "PowerW": 1, "PinCapW": 12.1},
            {"QtName": "O1(xor)", "PowerW": 1, "PinCapW": 12.1},
        ]
    },
    {
        "Level0": "dut_cnt",
        "Level1": "cnt0",
        "LevelQT": [
            {"QtName": "X0(xor)", "PowerW": 3, "PinCapW": 2.11},
            {"QtName": "B0(buf)", "PowerW": 3, "PinCapW": 2.11},
        ]
    },
    {
        "Level0": "dut_cnt",
        "Level1": "cnt1",
        "LevelQT": [
            {"QtName": "X1(xor)", "PowerW": 3, "PinCapW": 2.1},
            {"QtName": "B1(buf)", "PowerW": 3, "PinCapW": 2.1},
        ]
    },
]


data_n = pd.json_normalize(data, "LevelQT", ["Level1", "Level0"])
print(data_n)

df = pd.DataFrame(data_n)
fig = px.treemap(df, path=[px.Constant("top"), 'Level0', 'Level1', 'QtName'],
                 values='PowerW')
fig.update_traces(root_color="lightgrey")
fig.update_layout(margin=dict(t=50, l=25, r=25, b=25))
fig.write_image("./inst0.powerW.svg")
fig.write_html("./inst0.powerW.html")

fig2 = px.treemap(df, path=[px.Constant("top"), 'Level0', 'Level1', 'QtName'],
                 values='PinCapW')
fig2.update_traces(root_color="lightgrey")
fig2.update_layout(margin=dict(t=50, l=25, r=25, b=25))
fig2.write_image("./inst0.pinCapW.svg")
fig2.write_html("./inst0.pinCapW.html")
