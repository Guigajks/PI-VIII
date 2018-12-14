from gevent import monkey
monkey.patch_all()

from flask import Flask, render_template
from flask_socketio import SocketIO

app = Flask(__name__)
# db = redis.StrictRedis('localhost', 6379, 0)

socketio = SocketIO(app)

@app.route('/')
def main():
    return render_template('index.html')

@socketio.on('connect', namespace='/')
def ws_conn():
    socketio.emit('msg', namespace='/')

@socketio.on('disconnect', namespace='/')
def ws_disconn():
    socketio.emit('msg', namespace='/')

@socketio.on('message', namespace='/')
def ws_send(message):
    print(message)
    socketio.emit('message',  message, namespace='/')


if __name__ == '__main__':
        socketio.run(app) 