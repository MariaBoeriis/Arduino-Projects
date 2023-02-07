void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Set Serial read to 9600 baud
  pinMode(8, OUTPUT); // Set pin 8 as OUTPUT
}

void loop() {
  // put your main code here, to run repeatedly:
  float potentiometerValue = analogRead(A0);  // Read value from potentiometer
  float voltage = potentiometerValue * 5.0 / 1023.0;  // Convert to voltage
  Serial.println(voltage);  // Print value

  if (voltage >= 2.0) {
    digitalWrite(8, HIGH);  // Turn on diode
  } else {
    digitalWrite(8, LOW);   // Turn off diode 
  }
    delay(1000);
}

