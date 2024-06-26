import os
import sys
import serial
import threading
import time

from flask import Flask, jsonify
from flask_cors import CORS
from flask_socketio import SocketIO, emit
from data.sensor_data import HCSR04
from gsm.sms import BinNotificationSystem

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

app = Flask(__name__)
CORS(app)
socketio = SocketIO(app, cors_allowed_origins="*")

latest_data = {
    "SENSOR_1": 0,
    "SENSOR_2": 0,
    "SENSOR_3": 0,
    "SENSOR_4": 0,
}

bin_system = BinNotificationSystem(port='/dev/ttyUSB0')

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
        if sensor.check_transmission(serial_port='/dev/ttyACM0'):
            sensor.get_bin_data()
            latest_data = {
                "SENSOR_1": sensor.sensor_1,
                "SENSOR_2": sensor.sensor_2,
                "SENSOR_3": sensor.sensor_3,
                "SENSOR_4": sensor.sensor_4
            }
            socketio.emit('sensor_update', latest_data)
            if sensor.sensor_1 >= 10:
                print("Bio is full")
                bin_system.send_notification('bio')
            if sensor.sensor_2 >= 10:
                print("Non is full")
                bin_system.send_notification('non')
            if sensor.sensor_3 >= 10:
                print("Rec is full")
                bin_system.send_notification('rec')
            if sensor.sensor_4 >= 10:
                print("Haz is full")
                bin_system.send_notification('haz')
    except serial.SerialException:
        pass
    except Exception as e:
        print(f"Error: {e}")
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
