// protobuf & sensor data lib
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "trashscan_protocol.pb.h"

#define _NUMSENSOR 4

const int TRIG_PINS[_NUMSENSOR] = {3, 5, 7, 9};
const int ECHO_PINS[_NUMSENSOR] = {4, 6, 8, 10};

void serialComms(bool status, uint8_t buffer[20], pb_ostream_t &stream);

void setup()
{
    Serial.begin(115200);
    for (int i = 0; i < _NUMSENSOR; i++)
    {
        pinMode(TRIG_PINS[i], OUTPUT);
        pinMode(ECHO_PINS[i], INPUT);
    }
}

float getDistanceCM(int trigPin, int echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    int duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;
}

void readAllDistances(float distances[])
{
    for (int i = 0; i < _NUMSENSOR; i++)
    {
        distances[i] = getDistanceCM(TRIG_PINS[i], ECHO_PINS[i]);
    }
}

void serialComms(bool status, uint8_t buffer[20], pb_ostream_t &stream)
{
    if (status)
    {
        Serial.write(buffer, stream.bytes_written);
    }
    else
    {
        Serial.println("Encoding failed");
    }
}

void loop()
{
    readAllDistances(distances);

    BIN_STATUS bin_statusdata = BIN_STATUS_init_zero;
    bin_statusdata.SENSOR_1 = 1.0;
    bin_statusdata.SENSOR_2 = 2.0;
    bin_statusdata.SENSOR_3 = 3.0;
    bin_statusdata.SENSOR_4 = 4.0;

    // protobuf encoder
    uint8_t buffer[BIN_STATUS_size];
    size_t buffer_len = BIN_STATUS_size;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_len);
    bool status = pb_encode(&stream, BIN_STATUS_fields, &bin_statusdata);

    float distances[_NUMSENSOR];
    bool isFull[_NUMSENSOR];

    serialComms(status, buffer, stream);
    delay(30);
}
