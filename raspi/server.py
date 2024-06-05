import os
import sys
from flask import Flask, jsonify
from data.sensor_data import HCSR04

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

app = Flask(__name__)
latest_data = {
    "SENSOR_1": 0.0,
    "SENSOR_2": 0.0,
    "SENSOR_3": 0.0,
    "SENSOR_4": 0.0
}

@app.route('/sensor_data', methods=['GET'])
def get_sensor_data():
    global latest_data
    sensor = HCSR04()
    if sensor.check_transmission(serial_port='COM11'):
        try:
            sensor_data = next(sensor.get_bin_data(serial_port='COM11', baud_rate=9600))
            latest_data = {
                "SENSOR_1": sensor_data.sensor_1,
                "SENSOR_2": sensor_data.sensor_2,
                "SENSOR_3": sensor_data.sensor_3,
                "SENSOR_4": sensor_data.sensor_4
            }
        except StopIteration:
            pass
    return jsonify(latest_data)

if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0', port=5000)
