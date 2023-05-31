from flask import Flask, jsonify, request
from flask_socketio import SocketIO, emit
from flask_cors import CORS

app = Flask(__name__)
cors = CORS(app)
socketio = SocketIO(app, cors_allowed_origins='*', async_mode='threading')

carProgress = {
    "speed": 0,
    "isRunning": False,
    "distance": 0
}

# Receive from the client the event to start the car
@socketio.on('startCar')
def handle_start_car():
    print("starCar")
    start_car()

def start_car():
    if not carProgress["isRunning"]:
        carProgress["isRunning"] = True

        socketio.start_background_task(emit_car_progress, carProgress)

# Emit car progress every second
def emit_car_progress(carProgress):
    while carProgress["isRunning"]:
        carProgress["speed"] = 2
        carProgress["distance"] += 0.55

        # show only the first 2 decimal places
        carProgress["distance"] = float("{:.2f}".format(carProgress["distance"]))

        socketio.emit('carProgress', carProgress)
        socketio.sleep(1)

@socketio.on('stopCar')
def handle_stop_car():
    print("stopCar")
    stop_car()

def stop_car():
    if carProgress["isRunning"]:
        carProgress["isRunning"] = False
        socketio.emit('carProgress', carProgress)

@socketio.on('resetCar')
def handle_reset_car():
    print("resetCar")
    reset_car()

def reset_car():
    carProgress["speed"] = 0
    carProgress["isRunning"] = False
    carProgress["distance"] = 0

    socketio.emit('carProgress', carProgress)

# Simulating object detection and emitting event
@app.route('/object/found', methods=['POST'])
def object_found():
    carProgress["isRunning"] = False
    socketio.emit('objectFound', carProgress)
    return '', 200

# Simulating object removed
@app.route('/object/removed', methods=['POST'])
def object_removed():
    socketio.emit('objectRemoved', carProgress)
    return '', 200

@socketio.on('connect')
def handle_connect():
    print('A client connected')

    socketio.emit('carProgress', carProgress)

    @socketio.on('disconnect')
    def handle_disconnect():
        print('A client disconnected')

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=4000)
