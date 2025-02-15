import sys
import time
import serial

def usage():
	print("Usage: python bootload.py COMx")
	sys.exit()

if len(sys.argv) != 2:
	usage()

port = sys.argv[1]

if not port.startswith("COM"):
	usage()

print("Opening {}".format(port), flush=True)

try:
	s = serial.Serial(port, baudrate=1200)
except:
	print("Unable to open {}".format(port), flush=True)
	sys.exit()

while not s.is_open:
	pass

s.close()

time.sleep(1)
