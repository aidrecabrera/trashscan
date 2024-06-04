#define TRIG_PIN1 3
#define ECHO_PIN1 4

#define TRIG_PIN2 5
#define ECHO_PIN2 6

#define TRIG_PIN3 7
#define ECHO_PIN3 8

#define TRIG_PIN4 9
#define ECHO_PIN4 10

const float DISTANCE_THRESHOLD = 100.0; // Adjustable threshold

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

void executeSensorAction(int sensorNumber, bool isNearby)
{
    if (isNearby)
    {
        Serial.print("Custom Action: Sensor ");
        Serial.print(sensorNumber);
        Serial.println(" detected an object nearby!");
        // Implement GSM communication
        // Implement Protocol Buffer serialization
    }
}

void printSensorInfo(int sensorNumber, float distance, bool isNearby)
{
    Serial.print("Sensor ");
    Serial.print(sensorNumber);
    Serial.print(" distance: ");
    Serial.print(distance);
    Serial.print(" cm, Nearby: ");
    Serial.println(isNearby ? "Yes" : "No");
}

void loop()
{
    float distance1 = readDistanceCM(TRIG_PIN1, ECHO_PIN1);
    float distance2 = readDistanceCM(TRIG_PIN2, ECHO_PIN2);
    float distance3 = readDistanceCM(TRIG_PIN3, ECHO_PIN3);
    float distance4 = readDistanceCM(TRIG_PIN4, ECHO_PIN4);

    bool isNearby1 = distance1 < DISTANCE_THRESHOLD;
    bool isNearby2 = distance2 < DISTANCE_THRESHOLD;
    bool isNearby3 = distance3 < DISTANCE_THRESHOLD;
    bool isNearby4 = distance4 < DISTANCE_THRESHOLD;

    printSensorInfo(1, distance1, isNearby1);
    executeSensorAction(1, isNearby1);

    printSensorInfo(2, distance2, isNearby2);
    executeSensorAction(2, isNearby2);

    printSensorInfo(3, distance3, isNearby3);
    executeSensorAction(3, isNearby3);

    printSensorInfo(4, distance4, isNearby4);
    executeSensorAction(4, isNearby4);

    delay(30);
}
