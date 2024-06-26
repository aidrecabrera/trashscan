import serial
import trashscan_protocol_pb2

ser = serial.Serial('/dev/ttyUSB0', 9600)

while True:
    encoded_message = ser.read(20)

    bin_status = trashscan_protocol_pb2.BIN_STATUS()
    bin_status.ParseFromString(encoded_message)

    # Print the sensor values
    print(f"SENSOR_1: {bin_status.SENSOR_1}")
    print(f"SENSOR_2: {bin_status.SENSOR_2}")
    print(f"SENSOR_3: {bin_status.SENSOR_3}")
    print(f"SENSOR_4: {bin_status.SENSOR_4}")
