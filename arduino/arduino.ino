const int transmitterPin = 9; // Transmitter Pin (TRIG)
const int receiverPin = 10;   // Receiver Pin (ECHO)

// Speed of sound constant (cm/us)
const float speedOfSound = 0.034;

// State variables
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
  // Send pulse to start the measurement
  sendPulse();

  // Get duration of the pulse from the receiverPin
  pulseDuration = measurePulseDuration();

  // Calculate the distance based on the duration
  if (pulseDuration > 0)
  {
    distance = calculateDistance(pulseDuration);
  }
  else
  {
    distance = -1; // Invalid measurement
  }

  // Print the distance
  printDistance(distance);

  // Add a short delay to prevent flooding the serial monitor
  delay(100);
}

/**
 * Sends a pulse to the transmitterPin to start the measurement
 */
void sendPulse()
{
  digitalWrite(transmitterPin, LOW);
  delayMicroseconds(2);
  digitalWrite(transmitterPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(transmitterPin, LOW);
}

/**
 * Measures the duration of the pulse from the receiverPin
 * @return pulse duration in microseconds
 */
long measurePulseDuration()
{
  return pulseIn(receiverPin, HIGH, 30000); // Timeout of 30ms to avoid hanging
}

/**
 * Calculates the distance based on the pulse duration
 * @param duration pulse duration in microseconds
 * @return distance in centimeters
 */
float calculateDistance(long duration)
{
  // Speed of sound wave divided by 2 (go and back), 0.034 cm/us
  return (duration * speedOfSound) / 2;
}

/**
 * Prints the distance to the Serial Monitor
 * @param distance distance in centimeters
 */
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
