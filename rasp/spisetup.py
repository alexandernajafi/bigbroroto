import wiringpi2 as wpi


class Spi:

	def __init__(self, chl = 1, speed = 15200):
		wpi.wiringPiSPISetup(chl, speed)

	def read(data = 255):
		wiringpi2.wiringPiSPIDataRW(data,'\n')
		print data