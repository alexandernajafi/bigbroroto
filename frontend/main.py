import sys
import time
from daemon import Daemon
from threading import Thread
import socket
from thread import *
import Queue
from camera import Camera
from spisetup import Spi

class SocketServerSSE(Thread):
	def __init__(self):
		Thread.__init__(self)
		self.dataQueue = Queue.Queue()

	def run(self):
		port = 8008
		soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		soc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		sys.stdout.write("Created socket")
		sys.stdout.flush()
		try:
			soc.bind(('', port))
		except socket.error as error:
			sys.stderr.write("Error: Could not bind to port... "+str(error))
			sys.stderr.flush()
			sys.exit()
		soc.listen(10)
		connections = []
		notRunning = True

		def clientHandler(conn):
			notRunning = False
			sys.stdout.write("Started client handler")
			sys.stdout.flush()
			while len(connections) > 0:
				data = self.dataQueue.get()
				sys.stdout.write("Got data, will send to clients: "+str(data))
				sys.stdout.flush()
				for connection in connections:
					try:
						connection.send(str(data))
					except socket.error:
						connections.remove(connection)
					time.sleep(1)
					#pass # Send data somehow
			notRunning = True

		while True:
			sys.stdout.write("Now listening for clients")
			sys.stdout.flush()
			conn, adr = soc.accept()
			sys.stdout.write("Accepted socket from "+str(adr))
			sys.stdout.flush()
			connections.append(conn)
			if len(connections) > 0 and notRunning:
				start_new_thread(clientHandler, (conn,))

class BigBroRoto(Daemon):
	def run(self):
		i = 0
		th = SocketServerSSE()
		th.start()
		sys.stdout.write("hejsan")
		sys.stdout.flush()
		while True:
			i += 1
			sys.stdout.write("test: "+str(i)+"\n")
			sys.stdout.flush()
			time.sleep(1)

if __name__ == "__main__":
	roto = BigBroRoto('/tmp/bigbroroto-daemon.pid', stderr="/home/pi/logs.txt", stdout="/home/pi/logs.txt");
	if len(sys.argv) == 2:
                if 'start' == sys.argv[1]:
                        roto.start()
                elif 'stop' == sys.argv[1]:
                        roto.stop()
                elif 'restart' == sys.argv[1]:
                        roto.restart()
                else:
                        print "Unknown command"
                        sys.exit(2)
                sys.exit(0)
