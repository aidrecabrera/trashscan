#define TRIG_PIN1 3
#define ECHO_PIN1 4

#define TRIG_PIN2 5
#define ECHO_PIN2 6

#define TRIG_PIN3 7
#define ECHO_PIN3 8

#define TRIG_PIN4 9
#define ECHO_PIN4 10

void setup()
{
    Serial.begin(115200);
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
    int duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;
}

void loop()
{
    float distance1 = readDistanceCM(TRIG_PIN1, ECHO_PIN1);
    float distance2 = readDistanceCM(TRIG_PIN2, ECHO_PIN2);
    float distance3 = readDistanceCM(TRIG_PIN3, ECHO_PIN3);
    float distance4 = readDistanceCM(TRIG_PIN4, ECHO_PIN4);

    bool isNearby = (distance1 < 100) || (distance2 < 100) || (distance3 < 100) || (distance4 < 100);

    Serial.print("Sensor 1 distance: ");
    Serial.println(distance1);
    Serial.print("Sensor 2 distance: ");
    Serial.println(distance2);
    Serial.print("Sensor 3 distance: ");
    Serial.println(distance3);
    Serial.print("Sensor 4 distance: ");
    Serial.println(distance4);

    delay(30);
}
