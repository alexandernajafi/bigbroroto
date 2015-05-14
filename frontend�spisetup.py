import wiringpi2 as wpi
'''


class Spi:

	def __init__(self, chl = 1, speed = 15200):
		wpi.wiringPiSPISetup(chl, speed)

	def read(data = 255):
		wiringpi2.wiringPiSPIDataRW(data,'\n')
		print data
'''
class Spi:
	def __init__(self, chl = 0, speed = 500000):
		wpi.wiringPiSPISetup(chl, speed)
		self.chl = chl
		#self.read(255, plane=True)
		#self.read(255, plane=True)
		print "Spi setup" 	
	
	def read(self, data = 255, plane = False):
		if not plane: print "Send: ", data
		data = chr(data)
		result = wpi.wiringPiSPIDataRW(self.chl, data)
		if not plane: print "Result from api: ", result
		hexdata = ''.join('%02x' % ord(byte) for byte in data)
		if not plane: print "Hex back: ", hexdata
		return int(hexdata, 16)

if __name__ == "__main__":
	spi = Spi()
	for i in range(25):
		spi.read(i)
