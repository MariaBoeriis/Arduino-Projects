#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Library for LCD
#include <Adafruit_VL53L0X.h>   // Library for IR Sensor
#include <DS3231.h> // Library for clock module

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to Clock Module
DS3231 rtc(SDA, SCL);

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to VL53L0X
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  lcd.init();       // Initialize LCD Display
  lcd.backlight();  // Turn on LCD Backlight

  // Initialize on IR Distance Sensor
  Serial.println("Turning on IR Distance Sensor");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot IR Distance Sensor"));
  }
  Serial.println(F("IR Distance Sensor is functional"));

  // Initialize the Clock Module
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(13, 25, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(7, 2, 2023);   // Set the date to January 1st, 2014
}

void loop() {
  // Measure distance
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  
  delay(1000);

  // Show data on lcd screen
  lcd.setCursor(2, 0);        // Set the cursor on the third column and first row.
  lcd.print(measure.RangeMilliMeter);  // Print data
  lcd.setCursor(14, 0);
  lcd.print("mm");  // Print mm unit at the end of line 1
  lcd.setCursor(2, 1);        // Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print(rtc.getTimeStr());  // Print timestamp
}
