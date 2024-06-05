#include <pb_encode.h>
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

void setup()
{
    Serial.begin(115200); // protobuf transmission data
    Serial.begin(9600); // gsm module
    SIM900.begin(9600); // gsm module
    delay(5000);
    sendsms(textForSMS, number);
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

    delay(30);
}

void sendsms(String message, String number)
{
    String mnumber = "AT + CMGS = \"" + number + "\"";
    SIM900.print("AT+CMGF=1\r");
    delay(1000);
    SIM900.println(mnumber);
    delay(1000);
    SIM900.println(message);
    delay(1000);
    SIM900.println((char)26);
    delay(1000);
    SIM900.println();
    delay(100);
}
