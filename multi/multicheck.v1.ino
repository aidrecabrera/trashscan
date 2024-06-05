#include

#define NUM_CANS 4

const int TRIG_PINS[_NUMSENSOR] = {3, 5, 7, 9};
const int ECHO_PINS[_NUMSENSOR] = {4, 6, 8, 10};

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

void loop()
{
    float distances[_NUMSENSOR];
    bool isFull[_NUMSENSOR];

    readAllDistances(distances);

    delay(30);
}
