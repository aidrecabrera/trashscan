#include "pb_encode.h"
#include "trashscan_protocol.pb.h"

#define TRIG_PIN1 3
#define ECHO_PIN1 4
#define TRIG_PIN2 5
#define ECHO_PIN2 6
#define TRIG_PIN3 7
#define ECHO_PIN3 8
#define TRIG_PIN4 9
#define ECHO_PIN4 10

const int triggerDistance = 13;
const unsigned long sensorInterval = 1000; // 5 seconds

enum State {
    READ_SENSORS,
    CHECK_SENSORS,
    SEND_PROTOBUF,
    WAIT
};

State currentState = READ_SENSORS;
unsigned long lastSensorRead = 0;
float distances[4] = {0, 0, 0, 0};

float readDistanceCM(int trigPin, int echoPin);
void checkAndSendSMS();
void sendProtobufData();

void setup() {
    Serial.begin(19200); // Protobuf transmission data

    pinMode(TRIG_PIN1, OUTPUT);
    pinMode(ECHO_PIN1, INPUT);
    pinMode(TRIG_PIN2, OUTPUT);
    pinMode(ECHO_PIN2, INPUT);
    pinMode(TRIG_PIN3, OUTPUT);
    pinMode(ECHO_PIN3, INPUT);
    pinMode(TRIG_PIN4, OUTPUT);
    pinMode(ECHO_PIN4, INPUT);
}

void loop() {
    unsigned long currentMillis = millis();

    switch (currentState) {
        case READ_SENSORS:
            if (currentMillis - lastSensorRead >= sensorInterval) {
                lastSensorRead = currentMillis;
                distances[0] = readDistanceCM(TRIG_PIN1, ECHO_PIN1);
                distances[1] = readDistanceCM(TRIG_PIN2, ECHO_PIN2);
                distances[2] = readDistanceCM(TRIG_PIN3, ECHO_PIN3);
                distances[3] = readDistanceCM(TRIG_PIN4, ECHO_PIN4);
                currentState = CHECK_SENSORS;
            }
            break;

        case CHECK_SENSORS:
            currentState = SEND_PROTOBUF;
            break;

        case SEND_PROTOBUF:
            sendProtobufData();
            currentState = WAIT;
            break;

        case WAIT:
            if (currentMillis - lastSensorRead >= sensorInterval) {
                currentState = READ_SENSORS;
            }
            break;
    }
}

float readDistanceCM(int trigPin, int echoPin) {
    float totalDistance = 0;
    int validReadings = 0;

    for (int i = 0; i < 5; i++) {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH);
        if (duration > 0) {
            totalDistance += duration * 0.034 / 2;
            validReadings++;
        }
    }
    return (validReadings > 0) ? totalDistance / validReadings : -1;
}

void sendProtobufData() {
    BIN_STATUS binStatus = BIN_STATUS_init_zero;

    binStatus.SENSOR_1 = (distances[0] != -1) ? (distances[0] < 40 ? distances[0] : 40) : -1;
    binStatus.SENSOR_2 = (distances[1] != -1) ? (distances[1] < 40 ? distances[1] : 40) : -1;
    binStatus.SENSOR_3 = (distances[2] != -1) ? (distances[2] < 40 ? distances[2] : 40) : -1;
    binStatus.SENSOR_4 = (distances[3] != -1) ? (distances[3] < 40 ? distances[3] : 40) : -1;

    uint8_t buffer[BIN_STATUS_size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    bool status = pb_encode(&stream, BIN_STATUS_fields, &binStatus);

    if (status) {
        Serial.write(buffer, stream.bytes_written);
    } else {
        Serial.print("Failed to encode protobuf: ");
        Serial.println(PB_GET_ERROR(&stream));
    }
}
