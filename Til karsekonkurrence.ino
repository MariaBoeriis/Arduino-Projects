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


//File myFile; // Initialize file

const int chipSelect = 10;

DS3231 rtc(SDA, SCL);

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


  // Initialize SD reader
  Serial.print("Initializing SD card...");

  


}

void loop() {
  // Measure distance
  VL53L0X_RangingMeasurementData_t measure;

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
  DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(temperature);
  delay(1000);

  // Humidity & Temperature & Range
  lcd.setCursor(0, 0);
  lcd.print("H: ");// humidity);//, " T: ", temperature, " L: ", range );
  lcd.setCursor(4,0);
}
