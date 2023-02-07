#include <Wire.h>
#include <L
#include <DS3231.h> // Library for clock module

DS3231 rtc(SDA, SCL);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initialize the rtc object
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(13, 25, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(7, 2, 2023);   // Set the date to January 1st, 2014
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());
  
  // Wait one second before repeating :)
  delay (1000);
}
