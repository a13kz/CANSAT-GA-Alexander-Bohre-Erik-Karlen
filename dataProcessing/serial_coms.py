import serial
import serial.tools.list_ports
import datetime
import os.path

active_port = ""
ports = serial.tools.list_ports.grep('USB')
path = 'C:/Users/alexander.bohre/Documents/Arduino/gymnasie_arbete/collected_data/'
today = datetime.date.today()
current_time = datetime.datetime.now().strftime("%H:%M:%S")

for p in ports:
    active_port = p.device
    print(os.path.isfile(path+str(today)+'.txt'))
    ser = serial.Serial(active_port, 115200)
while True:     
    line=str(ser.readline())
    arr = line[2:][:-5].split()
    print(arr)
    with open(path+'/temperatures/'+str(today)+'temperature'+'.txt', "a") as f:
        f.write(f'{arr[0]} {current_time} \n')
    with open(path+'/pressures/'+str(today)+'pressure'+'.txt', "a") as f:
        f.write(f'{arr[1]} {current_time} \n')
    with open(path+'/altitudes/'+str(today)+'altitude'+'.txt', "a") as f:
        f.write(f'{arr[2]} {current_time} \n')
