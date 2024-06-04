const int transmitterPin = 9;
const int receiverPin = 10;

const float speedOfSound = 0.034;

long pulseDuration;
float distance;

void setup()
{
  pinMode(transmitterPin, OUTPUT);
  pinMode(receiverPin, INPUT);
  Serial.begin(9600);
}

void loop()
{
  sendPulse();
  pulseDuration = measurePulseDuration();
  if (pulseDuration > 0)
  {
    distance = calculateDistance(pulseDuration);
  }
  else
  {
    distance = -1;
  }
  printDistance(distance);
  delay(100);
}

void sendPulse()
{
  digitalWrite(transmitterPin, LOW);
  delayMicroseconds(2);
  digitalWrite(transmitterPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(transmitterPin, LOW);
}

long measurePulseDuration()
{
  return pulseIn(receiverPin, HIGH, 30000);
}

float calculateDistance(long duration)
{
  return (duration * speedOfSound) / 2;
}

void printDistance(float distance)
{
  if (distance >= 0)
  {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  else
  {
    Serial.println("Invalid measurement");
  }
}
