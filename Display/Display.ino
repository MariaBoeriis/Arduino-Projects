#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Library for LCD
#include <DS3231.h> // Library for clock module
#include <SparkFun_VL53L1X.h> // IR Sensor

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

void setup() {
  // put your setup code here, to run once:
  lcd.init();       // Initialize LCD Display
  lcd.backlight();  // Turn on LCD Backlight
}

void loop() {
  // put your main code here, to run repeatedly:
  // Print 'Hello World!' on the first line of the LCD:
  lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
  lcd.print("Hello World!"); // Print the string "Hello World!"
  lcd.setCursor(2, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print("Maria <3 ;D");
}
