#!/usr/bin/env python

from flask import Flask, render_template
import cherrypy
from paste.translogger import TransLogger

app = Flask(__name__)
app.debug = True

@app.route("/")
def hello():
    return render_template("layout.html")

def run_server():
    app_logged = TransLogger(app)
    cherrypy.tree.graft(app_logged, '/')
    cherrypy.config.update({
        'engine.autoreload_on': True,
        'log.screen': True,
        'server.socket_port': 5000,
        'server.socket_host': 'localhost'
    })
    cherrypy.engine.start()
    cherrypy.engine.block()

if __name__ == "__main__":
    run_server()