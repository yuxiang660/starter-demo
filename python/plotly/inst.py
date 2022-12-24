import plotly.express as px
import pandas as pd
data = {
    "cell_weight": [1, 1, 1, 1, 1, 1,  # mux weight == 3
                    3, 3, 3, 3,  # cnt weight == 6
                    ],
    "L0": ["U0(not)", "V0(inv)", "O0(or)",  # mux0
           "U1(not)", "V1(inv)", "O1(or)",  # mux1
           "X0(xor)", "B0(BUF)",  # cnt0
           "X1(xor)", "B1(BUF)",  # cnt1
           ],
    "L1": ["mux0", "mux0", "mux0", "mux1", "mux1", "mux1",  # dut_mux
           "cnt0", "cnt0", "cnt1", "cnt1",  # dut_cnt
           ],
    "L2": ["dut_mux", "dut_mux", "dut_mux", "dut_mux", "dut_mux", "dut_mux",
           "dut_cnt", "dut_cnt", "dut_cnt", "dut_cnt",
           ],
}
df = pd.DataFrame(data)
fig = px.treemap(df, path=[px.Constant("top"), 'L2',
                 'L1', 'L0'], values='cell_weight')
fig.update_traces(root_color="lightgrey")
fig.update_layout(margin=dict(t=50, l=25, r=25, b=25))
fig.write_image("./inst.svg")
fig.write_html("./inst.html")
