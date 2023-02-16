#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Library for LCD
#include <VL53L0X.h> // Library for IR Sensor
#include <DS3231.h> // Library for clock module

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to Clock Module
DS3231 rtc(SDA, SCL);

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to VL53L0X
VL53L0X range;

void setup() {
  Serial.begin(9600);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  lcd.init();       // Initialize LCD Display
  lcd.backlight();  // Turn on LCD Backlight

  // Initialize on IR Distance Sensor
  if (!range.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  range.startContinuous();

  // Initialize the Clock Module
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(13, 25, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(7, 2, 2023);   // Set the date to January 1st, 2014
}

void loop() {
  // Measure distance
  Serial.println(range.readRangeContinuousMillimeters());

  delay(1000);

  // Show data on lcd screen
  lcd.setCursor(2, 0);        // Set the cursor on the third column and first row.
  lcd.print(range.readRangeContinuousMillimeters());  // Print data
  lcd.setCursor(14, 0);
  lcd.print("mm");  // Print mm unit at the end of line 1
  lcd.setCursor(2, 1);        // Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print(rtc.getTimeStr());  // Print timestamp
}
