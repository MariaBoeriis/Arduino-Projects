#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Library for LCD
#include <Adafruit_VL53L0X.h>   // Library for IR Sensor
#include <DS3231.h> // Library for clock module
#include <dht.h>    // Library for DHT11 Humidity Sensor
#include <SPI.h>    // Library for SD Card Reader
#include <SD.h>     // Library for SD Card Reader


#define DHT11_PIN 7

float humidity;
int temperature;
int range;
//char date;
//char time;


File myFile; // Initialize file

// change this to match your SD shield or module;
const int chipSelect = 10;

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to Clock Module
DS3231 rtc(SDA, SCL);

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 20x4 LCD.

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to VL53L0X
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Wiring: Serial pin is connected to D7.
// Connect to DHT-11
dht DHT;

void setup() {
  Serial.begin(9600);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  lcd.init();       // Initialize LCD Display
  lcd.backlight();  // Turn on LCD Backlight

  // Initialize on IR Distance Sensor
  Serial.println("Initializing IR Sensor");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot IR Sensor"));
    while (1);
  }
  Serial.println(F("Booted IR Sensor"));

  // Initialize the Clock Module
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(13, 25, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(7, 2, 2023);   // Set the date to January 1st, 2014

  // Initialize SD reader
  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");


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

  

  // ------ Show data on lcd screen ------
  humidity = DHT.humidity;
  temperature = DHT.temperature;
  range = measure.RangeMilliMeter;
  //date = rtc.getDateStr();
  //time = rtc.getTimeStr();

  // Measure humidity and temperature
  DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(temperature);
  Serial.print("Humidity = ");
  Serial.println(humidity);

  delay(1000);

  // Humidity & Temperature & Range
  lcd.setCursor(0, 0);
  lcd.print("H: ");// humidity);//, " T: ", temperature, " L: ", range );
  lcd.setCursor(4,0);
  //lcd.print(humidity, 0);
  // Date & Time
  //lcd.setCursor(0, 1);
  //lcd.print("D: ", date, " ", time);

}
