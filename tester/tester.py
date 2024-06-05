import serial
import trashscan_protocol_pb2

ser = serial.Serial('COM10', 19200, timeout=1)

def read_protobuf_message(ser, message_length):
    buffer = bytearray()
    while len(buffer) < message_length:
        chunk = ser.read(message_length - len(buffer))
        if not chunk:
            break
        buffer.extend(chunk)
    return buffer

while True:
    expected_length = 20
    encoded_message = read_protobuf_message(ser, expected_length)

    if len(encoded_message) == expected_length:
        bin_status = trashscan_protocol_pb2.BIN_STATUS()
        try:
            bin_status.ParseFromString(encoded_message)
            print(f"SENSOR_1: {bin_status.SENSOR_1}")
            print(f"SENSOR_2: {bin_status.SENSOR_2}")
            print(f"SENSOR_3: {bin_status.SENSOR_3}")
            print(f"SENSOR_4: {bin_status.SENSOR_4}")
        except Exception as e:
            print(f"Failed to parse message: {e}")
    else:
        print("Incomplete message received")
