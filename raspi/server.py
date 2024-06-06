import os
import sys
import serial
from flask import Flask, jsonify
from flask_cors import CORS
from data.sensor_data import HCSR04
from proto import trashscan_protocol_pb2

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

app = Flask(__name__)
CORS(app)

latest_data = {
    "SENSOR_1": 100.0,
    "SENSOR_2": 1.0,
    "SENSOR_3": 0.0,
    "SENSOR_4": 0.0
}

@app.route('/sensor_data', methods=['GET'])
def get_sensor_data():
    global latest_data
    sensor = HCSR04()
    if sensor.check_transmission(serial_port='COM10'):
        try:
            sensor.get_bin_data()
            print(sensor.sensor_1)
            print(sensor.sensor_2)
            print(sensor.sensor_3)
            print(sensor.sensor_4)
            latest_data = {
                "SENSOR_1": sensor.sensor_1,
                "SENSOR_2": sensor.sensor_2,
                "SENSOR_3": sensor.sensor_3,
                "SENSOR_4": sensor.sensor_4
            }
        except StopIteration:
            pass
    return jsonify(latest_data)

if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0', port=5000)