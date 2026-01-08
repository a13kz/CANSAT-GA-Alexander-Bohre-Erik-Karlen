import plotly.express as px
file = open("collected_data/data.txt", "r")
data = file.read()
#print(data)
file.close

def make_arr(arr):
    arr = arr.split("\n")
    arr[] = arr.split("=")
    return arr

print(make_arr(data)[1])
#
#fig = px.line(x=["a","b","c"], y=[1,3,2], title="sample figure")
#fig.show()