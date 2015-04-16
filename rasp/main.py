from camera import Camera
from spisetup import Spi
import time

def main():
	spi = Spi()
	cam = Camera("192.168.10.101", "root", "bigbroroto")
	oldAngle = 0
	cam.rotateTo(0)
	cam.tiltTo(0)
	print "Will start listen for angles from dsp" 
	while True:
		newAngle = spi.read(250, plane=True)
		if newAngle == 0:
			continue
		# Compute 0-255 to 0-360
		newAngle = int(newAngle * 1.4117647059)
		# Calculate real angle
		if newAngle < 180:
			camAngle = -180+newAngle
		else: 
			camAngle = newAngle-180
		if oldAngle != camAngle:
			cam.rotateTo(camAngle);
			oldAngle = camAngle
			print "Rotate to: ", camAngle
		time.sleep(0.1)

if __name__ == "__main__":
	main()
