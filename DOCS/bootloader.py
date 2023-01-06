import serial
import os

# Change the following variable before running the script
fileName = "STM32F446_UserApplication_Update.bin"
COM = "COM11"

fileSize = os.path.getsize(fileName)
print('Bootloader PC program')
print(f'Binary file is {fileSize} bytes')

# Open the serial link on port COM
s = serial.Serial(COM,115200,timeout=1)
f = open(fileName,"rb")

# Send bytes of the binary file one by one
for x in range(fileSize):
    s.write(f.read(1))
    print(s.read(1))
