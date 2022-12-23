import plotly.express as px
df = px.data.tips()
fig = px.treemap(df, path=[px.Constant("all"), 'day',
                 'time', 'sex'], values='total_bill')
fig.update_traces(root_color="lightgrey")
fig.update_layout(margin=dict(t=50, l=25, r=25, b=25))
fig.write_html("./treemap0.html")
