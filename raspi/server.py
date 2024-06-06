import os
import sys
import serial
import threading
import time
from flask import Flask, jsonify
from flask_cors import CORS
from flask_socketio import SocketIO, emit
from data.sensor_data import HCSR04

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

app = Flask(__name__)
CORS(app)
socketio = SocketIO(app, cors_allowed_origins="*")

latest_data = {
    "SENSOR_1": 13,
    "SENSOR_2": 0,
    "SENSOR_3": 0,
    "SENSOR_4": 0,
}

@app.route('/sensor_data', methods=['GET'])
def get_sensor_data():
    return jsonify(latest_data)

@socketio.on('connect')
def handle_connect():
    emit('sensor_update', latest_data)

def update_sensor_data():
    global latest_data
    sensor = HCSR04()
    try:
        if sensor.check_transmission(serial_port='COM10'):
            sensor.get_bin_data()
            latest_data = {
                "SENSOR_1": sensor.sensor_1,
                "SENSOR_2": sensor.sensor_2,
                "SENSOR_3": sensor.sensor_3,
                "SENSOR_4": sensor.sensor_4
            }
            socketio.emit('sensor_update', latest_data)
    except serial.SerialException as e:
        pass
    except Exception as e:
        pass
    finally:
        socketio.emit('sensor_update', latest_data)

def sensor_data_updater():
    while True:
        update_sensor_data()
        time.sleep(0.5)


if __name__ == "__main__":
    updater_thread = threading.Thread(target=sensor_data_updater)
    updater_thread.daemon = True
    updater_thread.start()
    socketio.run(app, debug=True, host='0.0.0.0', port=5000)
