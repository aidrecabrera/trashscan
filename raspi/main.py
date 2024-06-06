import os
import sys
from data.sensor_data import HCSR04

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

if __name__ == "__main__":
    sensor = HCSR04()
    
    if sensor.check_transmission(serial_port='/dev/ttyACM0'):
        while True:
            sensor.get_bin_data()
            print(sensor.sensor_1)
            print(sensor.sensor_2)
            print(sensor.sensor_3)
            print(sensor.sensor_4)
    else:
        print("Serial port not found.")
