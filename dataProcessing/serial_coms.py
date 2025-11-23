import serial
import serial.tools.list_ports
import datetime
import os.path

active_port = ""
ports = serial.tools.list_ports.grep('USB')
path = 'C:/Users/alexander.bohre/Documents/Arduino/gymnasie_arbete/collected_data/'
today = datetime.date.today()
print(today)
for p in ports:
    active_port = p.device
    print(os.path.isfile(path+str(today)+'.txt'))
    now = datetime.datetime.now()
    ser = serial.Serial(active_port, 115200)
while True:     
    line=str(ser.readline())
    arr = line[2:][:-5].split()
    print(arr)
    with open(path+'/temps/'+str(today)+'temp'+'.txt', "a") as f:
        f.write(arr[0]+'\n')
    with open(path+'/pressures/'+str(today)+'pressure'+'.txt', "a") as f:
        f.write(arr[1]+'\n')



