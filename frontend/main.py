import sys
import time
from daemon import Daemon

class BigBroRoto(Daemon):
	def run(self):
		i = 0
		while True:
			i += 1
			sys.stdout.write("test: "+str(i)+"\n")
			sys.stdout.flush()
			time.sleep(1)

if __name__ == "__main__":
	roto = BigBroRoto('/tmp/bigbroroto-daemon.pid', stdout="/home/pi/logs.txt");
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
