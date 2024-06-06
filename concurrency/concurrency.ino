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
const unsigned long smsInterval = 10000; // 10 seconds
unsigned long lastSensorRead = 0;
unsigned long lastSMS = 0;

enum State { IDLE, READ_SENSORS, SEND_PROTOBUF, CHECK_SMS, SEND_SMS };
State currentState = IDLE;

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
    return duration * 0.034 / 2;
}

void loop()
{
    unsigned long currentMillis = millis();

    switch (currentState)
    {
        case IDLE:
            if (currentMillis - lastSensorRead >= sensorInterval)
            {
                lastSensorRead = currentMillis;
                currentState = READ_SENSORS;
            }
            break;

        case READ_SENSORS:
        {
            float distance1 = readDistanceCM(TRIG_PIN1, ECHO_PIN1);
            float distance2 = readDistanceCM(TRIG_PIN2, ECHO_PIN2);
            float distance3 = readDistanceCM(TRIG_PIN3, ECHO_PIN3);
            float distance4 = readDistanceCM(TRIG_PIN4, ECHO_PIN4);

            static BIN_STATUS bin_statusdata = BIN_STATUS_init_zero;
            bin_statusdata.SENSOR_1 = distance1 < 40 ? distance1 : 40;
            bin_statusdata.SENSOR_2 = distance2 < 40 ? distance2 : 40;
            bin_statusdata.SENSOR_3 = distance3 < 40 ? distance3 : 40;
            bin_statusdata.SENSOR_4 = distance4 < 40 ? distance4 : 40;

            currentState = SEND_PROTOBUF;
        }
        break;

        case SEND_PROTOBUF:
        {
            uint8_t buffer[BIN_STATUS_size];
            pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
            bool status = pb_encode(&stream, BIN_STATUS_fields, &bin_statusdata);

            if (status)
            {
                Serial.write(buffer, stream.bytes_written);
            }

            currentState = CHECK_SMS;
        }
        break;

        case CHECK_SMS:
        {
            static int sensorIndex = 0;
            float distances[] = { bin_statusdata.SENSOR_1, bin_statusdata.SENSOR_2, bin_statusdata.SENSOR_3, bin_statusdata.SENSOR_4 };
            String messages[] = { textForSensor1, textForSensor2, textForSensor3, textForSensor4 };

            if (sensorIndex < 4)
            {
                checkAndSendSMS(distances[sensorIndex], triggerCM, messages[sensorIndex]);
                sensorIndex++;
            }
            else
            {
                sensorIndex = 0;
                currentState = IDLE;
            }
        }
        break;

        case SEND_SMS:
            break;
    }
}

void checkAndSendSMS(float distance, int threshold, String message)
{
    if (distance < threshold)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - lastSMS >= smsInterval)
        {
            lastSMS = currentMillis;
            sendsms(message, number);
        }
    }
}

void sendsms(String message, String number)
{
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
}
