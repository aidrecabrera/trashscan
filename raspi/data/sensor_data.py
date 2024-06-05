import os
import sys

import serial
from proto import trashscan_protocol_pb2

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

class HCSR04:
    def __init__(self):
        self.sensor_1 = 0
        self.sensor_2 = 0
        self.sensor_3 = 0
        self.sensor_4 = 0
        
    def get_bin_data(self, serial_port='/dev/ttyUSB0', baud_rate=9600):
        with serial.Serial(serial_port, baud_rate, timeout=1) as ser:
            while True:
                incoming_data = ser.read(ser.inWaiting())

                if incoming_data:
                    sensor_data_packet = trashscan_protocol_pb2.BIN_STATUS()
                    sensor_data_packet.ParseFromString(incoming_data)

                    self.sensor_1 = sensor_data_packet.SENSOR_1
                    self.sensor_2 = sensor_data_packet.SENSOR_2
                    self.sensor_3 = sensor_data_packet.SENSOR_3
                    self.sensor_4 = sensor_data_packet.SENSOR_4
                    
                    yield self
    
    def check_transmission(self, serial_port="COM11"):
        return os.path.exists(serial_port)