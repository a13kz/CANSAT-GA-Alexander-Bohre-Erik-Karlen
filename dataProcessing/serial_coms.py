import serial
import serial.tools.list_ports
import datetime
import os.path


active_port = ""
ports = serial.tools.list_ports.grep('USB')
#change to current directory
path = 'C:/Users/alexander.bohre/Documents/Arduino/gymnasie_arbete/collected_data/'
today = datetime.date.today()


for p in ports:
    active_port = p.device
    break
ser = serial.Serial(active_port, 115200)

print("Location for measure?")
location = input()

while True:     
    line=ser.readline().decode('latin1').strip()
    print(line)
    current_time = datetime.datetime.now().strftime("%H:%M:%S")
    arr = line.split("|")
    print(arr)
    if len(arr) == 5:
        with open(f"{path}/temperatures/{str(today)}_{location}_temperature.txt", "a") as f:
            f.write(f'{arr[0]} time: {current_time} \n')
        with open(f"{path}/pressures/{str(today)}_{location}_pressure.txt", "a") as f:
            f.write(f'{arr[1]} time: {current_time} \n')
        with open(f"{path}/altitudes/{str(today)}_{location}_altitude.txt", "a") as f:
            f.write(f'{arr[2]} time: {current_time} \n')
        with open(f"{path}/accelerations/{str(today)}_{location}_acceleration.txt", "a") as f:
            f.write(f'{arr[3]} time: {current_time} \n')
        with open(f"{path}/rotations/{str(today)}_{location}_rotation.txt", "a") as f:
            f.write(f'{arr[4]} time: {current_time} \n')


