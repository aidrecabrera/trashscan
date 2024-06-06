#include "pb_encode.h"
#include <SoftwareSerial.h>
#include "trashscan_protocol.pb.h"

#define TRIG_PIN1 3
#define ECHO_PIN1 4

#define TRIG_PIN2 5
#define ECHO_PIN2 6

#define TRIG_PIN3 7
#define ECHO_PIN3 8

#define TRIG_PIN4 9
#define ECHO_PIN4 10

SoftwareSerial SIM900(2, 11);

String number = "09483572088";

String textForSensor1 = "Biodegradable Bin is Full.";
String textForSensor2 = "Non-Biodegradable Bin is Full.";
String textForSensor3 = "Recyclable Bin is Full.";
String textForSensor4 = "Hazardous Bin is Full.";

const int triggerCM = 13;
const unsigned long sensorInterval = 5000; // 5 seconds
unsigned long lastSensorRead = 0;

enum State {
    READ_SENSORS,
    CHECK_SENSORS,
    SEND_SMS,
    SEND_PROTOBUF,
    WAIT
};

State currentState = READ_SENSORS;
float distances[4] = {0, 0, 0, 0};

void setup()
{
    Serial.begin(115200); // Protobuf transmission data
    SIM900.begin(9600);  // GSM module

    pinMode(TRIG_PIN1, OUTPUT);
    pinMode(ECHO_PIN1, INPUT);
    pinMode(TRIG_PIN2, OUTPUT);
    pinMode(ECHO_PIN2, INPUT);
    pinMode(TRIG_PIN3, OUTPUT);
    pinMode(ECHO_PIN3, INPUT);
    pinMode(TRIG_PIN4, OUTPUT);
    pinMode(ECHO_PIN4, INPUT);
}

float readDistanceCM(int trigPin, int echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    if (duration == 0) {
        return -1; // Indicate a timeout
    }
    return duration * 0.034 / 2;
}

void loop()
{
    unsigned long currentMillis = millis();

    switch (currentState)
    {
        case READ_SENSORS:
            if (currentMillis - lastSensorRead >= sensorInterval)
            {
                lastSensorRead = currentMillis;
                distances[0] = readDistanceCM(TRIG_PIN1, ECHO_PIN1);
                distances[1] = readDistanceCM(TRIG_PIN2, ECHO_PIN2);
                distances[2] = readDistanceCM(TRIG_PIN3, ECHO_PIN3);
                distances[3] = readDistanceCM(TRIG_PIN4, ECHO_PIN4);
                currentState = CHECK_SENSORS;
            }
            break;

        case CHECK_SENSORS:
            checkAndSendSMS();
            currentState = SEND_PROTOBUF;
            break;

        case SEND_SMS:
            currentState = WAIT;
            break;

        case SEND_PROTOBUF:
            sendProtobufData();
            currentState = WAIT;
            break;

        case WAIT:
            if (currentMillis - lastSensorRead >= sensorInterval)
            {
                currentState = READ_SENSORS;
            }
            break;
    }
}

void checkAndSendSMS()
{
    if (distances[0] != -1 && distances[0] < triggerCM)
    {
        sendNonBlockingSMS(textForSensor1, number);
    }
    if (distances[1] != -1 && distances[1] < triggerCM)
    {
        sendNonBlockingSMS(textForSensor2, number);
    }
    if (distances[2] != -1 && distances[2] < triggerCM)
    {
        sendNonBlockingSMS(textForSensor3, number);
    }
    if (distances[3] != -1 && distances[3] < triggerCM)
    {
        sendNonBlockingSMS(textForSensor4, number);
    }
}

void sendNonBlockingSMS(String message, String number)
{
    SIM900.listen();
    SIM900.print("AT+CMGF=1\r");
    delay(100);
    SIM900.print("AT+CMGS=\"");
    SIM900.print(number);
    SIM900.print("\"\r");
    delay(100);
    SIM900.print(message);
    delay(100);
    SIM900.write(26);
    delay(1000);
    Serial.listen();
}

void sendProtobufData()
{
    BIN_STATUS bin_statusdata = BIN_STATUS_init_zero;
    bin_statusdata.SENSOR_1 = (distances[0] != -1) ? (distances[0] < 40 ? distances[0] : 40) : -1;
    bin_statusdata.SENSOR_2 = (distances[1] != -1) ? (distances[1] < 40 ? distances[1] : 40) : -1;
    bin_statusdata.SENSOR_3 = (distances[2] != -1) ? (distances[2] < 40 ? distances[2] : 40) : -1;
    bin_statusdata.SENSOR_4 = (distances[3] != -1) ? (distances[3] < 40 ? distances[3] : 40) : -1;

    uint8_t buffer[BIN_STATUS_size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    bool status = pb_encode(&stream, BIN_STATUS_fields, &bin_statusdata);

    if (status)
    {
        Serial.write(buffer, stream.bytes_written);
    }
    else
    {
        Serial.println("Failed to encode protobuf");
    }
}
