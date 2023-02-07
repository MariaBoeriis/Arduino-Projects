#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Library for LCD
#include <DS3231.h> // Library for clock module
#include <SparkFun_VL53L1X.h> // IR Sensor

DS3231 rtc(SDA, SCL);

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

void setup()
{
  
  Wire.begin();

  Serial.begin(9600);
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.init() == false)
    Serial.println("Sensor online!");

  	// put your setup code here, to run once:
  Serial.begin(9600);
  
  // Initialize the rtc object
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(13, 25, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(7, 2, 2023);   // Set the date to January 1st, 2014
  
 // put your setup code here, to run once:
  lcd.init();       // Initialize LCD Display
  lcd.backlight();  // Turn on LCD Backlight

}

void loop()
{
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.stopRanging();

  Serial.print("Distance(mm): ");
  Serial.print(distance);

  //float distanceMeter = distance/1000;

  //Serial.print("\tDistance(m): ");
  //Serial.print(distanceMeter, 2);

  Serial.println();
  delay(3000);

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

   // put your main code here, to run repeatedly:
  // Print 'Hello World!' on the first line of the LCD:
  lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
  lcd.print(distance); // Print the string "Hello World!"
  lcd.setCursor(8, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print(rtc.getDOWStr());
}
