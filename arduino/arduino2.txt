#include <pb_encode.h>
#include "trashscan_protocol.pb.h"

#define NUM_SENSORS 4
#define MAX_DISTANCE_CM 40      // trashcan height
#define TRIGGER_THRESHOLD_CM 13 // threshold for triggering "full" status
#define TRIG_DURATION_US 10     // duration for triggering the ultrasonic pulse (in microseconds)

const uint8_t TRIG_PINS[NUM_SENSORS] = {3, 5, 7, 9};
const uint8_t ECHO_PINS[NUM_SENSORS] = {4, 6, 8, 10};
float distances[NUM_SENSORS];

void setup()
{
    Serial.begin(9600);
    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
        pinMode(TRIG_PINS[i], OUTPUT);
        pinMode(ECHO_PINS[i], INPUT);
        digitalWrite(TRIG_PINS[i], LOW);
    }
}

float getDistanceCM(uint8_t trigPin, uint8_t echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(TRIG_DURATION_US);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH, MAX_DISTANCE_CM * 58); // avoid blocking
    if (duration == 0)
    {
        return MAX_DISTANCE_CM; // return max distance
    }
    return duration * 0.034 / 2;
}

void readAllDistances()
{
    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
        distances[i] = getDistanceCM(TRIG_PINS[i], ECHO_PINS[i]);
    }
}

void serialComms(bool status, uint8_t buffer[], size_t length)
{
    if (status)
    {
        Serial.write(buffer, length);
    }
    else
    {
        Serial.println("Encoding failed");
    }
}

void loop()
{
    readAllDistances();

    // protobuf transmission data
    BIN_STATUS bin_statusdata = BIN_STATUS_init_zero;
    bin_statusdata.SENSOR_1 = distances[0];
    bin_statusdata.SENSOR_2 = distances[1];
    bin_statusdata.SENSOR_3 = distances[2];
    bin_statusdata.SENSOR_4 = distances[3];

    // protobuf encoder
    uint8_t buffer[BIN_STATUS_size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    bool status = pb_encode(&stream, BIN_STATUS_fields, &bin_statusdata);

    serialComms(status, buffer, stream.bytes_written);

    delay(100);
}
