import plotly.express as px
import pandas as pd
data = {
    "total_bill": [16.99, 10.34, 21.01, 23.68, 24.59],
    "day": ["Sun", "Sat", "Fri", "Thur", "Fri"],
    "time": ["Dinner", "Dinner", "Lunch", "Lunch", "Dinner"],
    "sex": ["Male", "Female", "Male", "Male", "Female"],
}
df = pd.DataFrame(data)
fig = px.treemap(df, path=[px.Constant("all"), 'day',
                 'time', 'sex'], values='total_bill')
fig.update_traces(root_color="lightgrey")
fig.update_layout(margin=dict(t=50, l=25, r=25, b=25))
fig.write_image("./treemap1.svg")
fig.write_html("./treemap1.html")
