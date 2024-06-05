from raspi.data.sensor_data import HCSR04


if __name__ == "__main__":
    sensor = HCSR04()
    
    if sensor.check_transmission(serial_port='COM11'):
        for data in sensor.get_bin_data(serial_port='COM11', baud_rate=9600):
            print(f"Sensor 1: {data.sensor_1}")
            print(f"Sensor 2: {data.sensor_2}")
            print(f"Sensor 3: {data.sensor_3}")
            print(f"Sensor 4: {data.sensor_4}")
    else:
        print("Serial port not found.")
