void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Set Serial read to 9600 baud
  pinMode(8, OUTPUT); // Set pin 8 as OUTPUT
}

void loop() {
  // put your main code here, to run repeatedly:
  float sensorValue = analogRead(A0);  // Read value from sensor
  float V = sensorValue * 5.0 / 1024.0; // 1024 = 2^10 bit
  float T = (V-1.375)/(0.0225);

  
  Serial.println(T);  // Print value 

  if (T >= 28.0) {
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
  }
  delay(1000);
}
