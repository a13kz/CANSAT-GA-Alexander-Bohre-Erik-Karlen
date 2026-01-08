import serial
import serial.tools.list_ports
import datetime
import os.path


active_port = ""
ports = serial.tools.list_ports.grep('USB')
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
    if len(arr) == 2:
        with open(f"{path}/temperatures/{str(today)}_Coords.txt", "a") as f:
            f.write(f'{arr[0]} time: {current_time} \n')