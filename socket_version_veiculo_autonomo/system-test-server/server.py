from flask import Flask, jsonify, request
from flask_socketio import SocketIO, emit
from flask_cors import CORS
from fastai.vision.all import *
import pickle
import cv2
import time
from picamera2 import Picamera2
import libcamera

app = Flask(__name__)
cors = CORS(app)
socketio = SocketIO(app, cors_allowed_origins='*', async_mode='threading')

carProgress = {
    "progress": 0,
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

@socketio.on('startPredict'):
def handle_start_predict():
    my_thread.start()

# Emit car progress every second
def emit_car_progress(carProgress):
    while carProgress["isRunning"]:
        carProgress["speed"] = 2
        carProgress["distance"] += 0.55

        # show only the first 2 decimal places
        carProgress["distance"] = float("{:.2f}".format(carProgress["distance"]))

        socketio.emit('carProgress', carProgress)
        socketio.sleep(1)


def my_predict(labels):
    def predict(img):
        img = PILImage.create(img)
        pred,pred_idx,probs = learn.predict(img)
        return {labels[i]: float(probs[i]) for i in range(len(labels))}

    while True:
        picam2.capture_file("test.png")
        image = "./test.png"
        result = predict(image) 
        result
        time.sleep(3)

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
    carProgress["progress"] = 0
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

def object_detected():
    objectDetected = request.get_json()["objectDetected"]
    socketio.emit('objectDetected', objectDetected)

    return '', 200

@socketio.on('connect')
def handle_connect():
    print('A client connected')

    socketio.emit('carProgress', carProgress)

@socketio.on('disconnect')
def handle_disconnect():
    print('A client disconnected')


labels = []
if __name__ == '__main__':
    cv2.startWindowThread()


    picam2 = Picamera2()
    preview_config = picam2.create_preview_configuration()
    preview_config["transform"] = libcamera.Transform(hflip=1, vflip=1)
    picam2.configure(preview_config)
    picam2.start()

    with open('learn_pkl', 'rb') as file:
          learn = pickle.load(file)

    labels = learn.dls.vocab

    socketio.run(app, host='192.168.0.101', port=4000)


# Grab images as numpy arrays and leave everything else to OpenCV.

