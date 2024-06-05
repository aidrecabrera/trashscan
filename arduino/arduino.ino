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
String textForSMS = "Test Message.";
String number = "09483572088";

String textForSensor1 = "Biodegradable Bin is Full.";
String textForSensor2 = "Non-Biodegradable Bin is Full.";
String textForSensor3 = "Recycable Bin is Full.";
String textForSensor4 = "Hazardous Bin is Full.";

const int triggerCM = 13;

void setup()
{
    Serial.begin(115200); // protobuf transmission data
    SIM900.begin(9600);  // gsm module

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
    float distance1 = readDistanceCM(TRIG_PIN1, ECHO_PIN1);
    float distance2 = readDistanceCM(TRIG_PIN2, ECHO_PIN2);
    float distance3 = readDistanceCM(TRIG_PIN3, ECHO_PIN3);
    float distance4 = readDistanceCM(TRIG_PIN4, ECHO_PIN4);

    // protobuf transmission data
    BIN_STATUS bin_statusdata = BIN_STATUS_init_zero;
    bin_statusdata.SENSOR_1 = distance1 < 40 ? distance1 : 40;
    bin_statusdata.SENSOR_2 = distance2 < 40 ? distance2 : 40;
    bin_statusdata.SENSOR_3 = distance3 < 40 ? distance3 : 40;
    bin_statusdata.SENSOR_4 = distance4 < 40 ? distance4 : 40;

    // protobuf encoder
    uint8_t buffer[BIN_STATUS_size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    bool status = pb_encode(&stream, BIN_STATUS_fields, &bin_statusdata);

    if (status)
    {
        Serial.write(buffer, stream.bytes_written);
    }

    if (distance1 < triggerCM)
    {
        sendsms(textForSensor1, number);
    }
    if (distance2 < triggerCM)
    {
        sendsms(textForSensor2, number);
    }
    if (distance3 < triggerCM)
    {
        sendsms(textForSensor3, number);
    }
    if (distance4 < triggerCM)
    {
        sendsms(textForSensor4, number);
    }
    
    delay(5000);
}

void sendsms(String message, String number)
{
    String mnumber = "AT+CMGS=\"" + number + "\"";
    SIM900.print("AT+CMGF=1\r");
    SIM900.println(mnumber);
    SIM900.println(message);
    SIM900.write(26);
}
