#!/usr/bin/env python

from flask import Flask, render_template, Response
from paste.translogger import TransLogger
import itertools
import gevent
#from gevent.queue import Queue
import Queue
import time
from gevent.wsgi import WSGIServer
import socket
from threading import Thread

app = Flask(__name__)
app.debug = True

class AlgorithmRunner(Thread):
    logsubscriptions = []

    def registerlogsubscriptor(self, q):
        self.logsubscriptions.append(q)

    def getlogsubscriptions(self):
        for sub in self.logsubscriptions:
            yield sub

    def removelogsubscriptions(self, q):
        self.logsubscriptions.remove(q)

    def log(self, message):
        print "Will log"
        date = time.strftime("%H:%M:%S")
        msg = str(date)+"\t "+message
        for subscriptor in self.logsubscriptions:
            subscriptor.put(msg)
        if len(self.logsubscriptions) == 0:
            print "Logged but no subscriptors"
        else:
            print "number of subs: "+str(len(self.logsubscriptions))

    def run(self):
        print "Starting algorithm"
        #for i in range(10):
        i = 0
        try:
            #while True:
            for i in range(10):
                self.log(str(i)+" log test")
                i += 1
                time.sleep(0.5)
        except Exception:
            print "close"

algo = AlgorithmRunner()

'''def log(message):
    print "Will"
    date = time.strftime("%H:%M:%S")
    msg = str(date)+"\t "+message
    def notify():
        for subscriptor in algo.getlogsubscriptions():
            subscriptor.put(msg)
    gevent.spawn(notify)'''

def _getSSEEvent(message):
    return "data: "+str(message)+"\n\n"

@app.before_first_request
def start_algo():
    algo.start()

@app.route("/")
def index():
    ip = socket.gethostbyname(socket.gethostname())
    algo.log("Client connected to frontend at "+str(ip))
    #gevent.spawn(algo.start)
    #algo.start()
    return render_template("layout.html")

@app.route("/logsstream")
def logsStream():
    def gen():
        q = Queue()
        algo.registerlogsubscriptor(q)
        try:
            while True:
                result = q.get()
                yield _getSSEEvent(result)
        except GeneratorExit:
            algo.removelogsubscriptions(q)
    return Response(gen(), content_type='text/event-stream')

def run_server():
    server = WSGIServer(("", 5000), app)
    server.serve_forever()

def runAlgorithm():
    rotoThread = AlgorithmRunner()
    rotoThread.start()

if __name__ == "__main__":
    #runAlgorithm()
    run_server()