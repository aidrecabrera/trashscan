import os
import sys
import serial
import serial.tools.list_ports
from proto import trashscan_protocol_pb2

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))


class HCSR04:
    def __init__(self):
        self.sensor_1 = 0
        self.sensor_2 = 0
        self.sensor_3 = 0
        self.sensor_4 = 0
        
    def get_bin_data(self):
        ser = serial.Serial('COM10', 19200)
        encoded_message = ser.read(20)
        bin_status = trashscan_protocol_pb2.BIN_STATUS()
        bin_status.ParseFromString(encoded_message)
        self.sensor_1 = bin_status.SENSOR_1
        self.sensor_2 = bin_status.SENSOR_2
        self.sensor_3 = bin_status.SENSOR_3
        self.sensor_4 = bin_status.SENSOR_4

    def check_transmission(self, serial_port='COM10'):
        ports = [port.device for port in serial.tools.list_ports.comports()]
        return serial_port in ports