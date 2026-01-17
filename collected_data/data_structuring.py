import matplotlib
import os
import sys

un_dir = os.path.dirname(os.path.abspath(sys.argv[0]))+'/unstructured_data'
s_dir = os.path.dirname(os.path.abspath(sys.argv[0]))+'/structured_data'

#import plotly.express as px
#file = open(c_dir+file_title, "r")
#data = file.read()

for x in os.listdir(un_dir):
    if x.endswith(".txt"):
        file = open(un_dir+"/"+x, "r")
        data = file.readlines()
        x = x.split(".")
        
        #with open(f"{s_dir}/{x[0]}.xlsx","w") as new_file:
        #    new_file.write(data)
        #    print(data[0])
        #os.remove(un_dir+"/"+x)
        #print(data)


#structured_file = open("C:\Users\nilsa\Documents\GitHub\gymnasie_arbete\collected_data\structured_data/.xlsx", "x")
##print(data)
#file.close
#
#def make_arr(arr):
#    arr = arr.split("\n")
#    arr[] = arr.split("=")
#    return arr
#
#print(make_arr(data)[1])
#
#fig = px.line(x=["a","b","c"], y=[1,3,2], title="sample figure")
#fig.show()