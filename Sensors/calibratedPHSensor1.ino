const int SensorPin = 0;
float buf[10];
int temp;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Initialize the serial communication for debugging (optional)
}


void loop() {
  // put your main code here, to run repeatedly:
  // Read the pH value and perform any desired actions
  float phValue = getPh();
  // Do something with the pH value
  Serial.print("pH value: ");
  Serial.println(phValue); // Print the pH value to the serial monitor (optional)
  delay(1000); // Delay for 1 second before taking another reading
}


float getPh() {
  for (int i = 0; i < 10; i++) { // Get 10 sample values from the sensor to smooth the value
    float analogVal = analogRead(SensorPin);
    buf[i] = -0.02165*(analogVal) + 16.91709;
    delay(10);
  }
  for (int i = 0; i < 9; i++) { // Sort the analog values from small to large
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  
  float medianPH = buf[4];
  delay(500);
  return medianPH;
}
