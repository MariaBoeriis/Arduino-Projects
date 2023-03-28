//include libraries
#include <DS3231.h>             // Library for Time Clock module
#include <SPI.h>                // Library for SD Card reader
#include <SD.h>                 // Library for SD Card reader
#include <LiquidCrystal_I2C.h>  // Library for LCD screen
#include <SparkFun_VL53L1X.h>   // Library for distance sensor
#include <dht11.h>              // Library for DHT11 Humidity Sensor

File myFile;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231 rtc(SDA, SCL);                //SDA and SCL from both the clockmodul and display is connected to the SDA and SCL input by making common channels on the breadboard

dht11 DHT11;                // set the variable
SFEVL53L1X distanceSensor;  // set the variable

// define optional interrupt and shutdown pins.
const int SHUTDOWN_PIN = 2;   //
const int INTERRUPT_PIN = 3;  // interrups pin 3 to automate program and solve timing problems
const int DHT11PIN = 7;       // Connects DHT11 Humidity Sensor to pin 7
const int chipSelect = 10;    // sets a constant value that defines that the chipselect can be read by the SD card reader from the 10 pin

// create float variables if necessary
float distance = distanceSensor.getDistance();  //Get the result of the measurement from the sensor

void setup() {
  // initialize serial monitor with a information transfer rate of 9600 baud (maximum 9600 bits per secound)
  Serial.begin(9600);
  // initialize the real time clock module
  rtc.begin();
  // print the measured data from the IR distance sensor
  Serial.println("VL53L1X Qwiic Test");
  // if the data cannot be measured then print an error message
  if (distanceSensor.begin() != 0)  //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  // if the data can be measured then print an message that confirms this
  Serial.println("Sensor online!");

  // intialize the lcd and it's backlight

  lcd.init();       // initialize the lcd
  lcd.backlight();  // initialize the lcd backlight

  // initialize the SD card reader
  SD.begin(10);  //Starts SD card reader at the chip select pin
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }

  // initialize and print measured data and save it into the SD card reader
  Serial.println("card initialized.");           // if the card is initialized then print a message in the serial monitor
  myFile = SD.open("filename.TXT", FILE_WRITE);  //creates a file on the SD card. If the data.txt file already exists,  Arduino will open the file instead of creating another one. (FILE_WRITE argument enables you to read and write into the file.)
  myFile.print("Timer");
  myFile.print("\t");
  myFile.print("Date");
  myFile.print("\t");
  myFile.print("Time");
  myFile.print("\t");
  myFile.println("Distance (mm)");
  myFile.print("\t");
  myFile.println("Humidity (%)");
  myFile.print("\t");
  myFile.println("Temperature (C)");
  myFile.close();
  delay(1000);
}

void loop() {
  // initialize the IR distance sensor
  distanceSensor.startRanging();  //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady()) {
    delay(1);
  }
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  // print the measured data from the clock module and the IR sensor into the seriel monitor
  Serial.print("Clock: ");
  Serial.print(rtc.getDateStr());    // Prints Date.
  Serial.println(rtc.getTimeStr());  // Prints Time.
  Serial.print("Distance: ");
  Serial.println(distance, 2);  // Prints distance
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);  // Prints humidity
  Serial.print("Temperature   (C): ");
  Serial.println((float)DHT11.temperature, 2);  // Prints temperature

  // print the data measured from the DHT11 Humidity Sensor into the serial monitor

  //lcd.clear();
  lcd.setCursor(0, 0);  // sets the curser on the screen to define where the letters should start being written
  lcd.print("H: (%)");
  lcd.setCursor(6, 0);
  lcd.print((float)DHT11.humidity, 2);  // Prints humidity
  lcd.setCursor(0, 1);
  lcd.print("T: (C)");
  lcd.print((float)DHT11.temperature, 2);  // Prints temperature
  delay(600000);                           // Measures in 10 min intervals

  // save the measured data from the clock module, the IR sensor and the DHT11 Humidity Sensor to the SD card
  myFile = SD.open("filename.TXT", FILE_WRITE);  // creates a file on the SD card. If the data.txt file already exists,  Arduino will open the file instead of creating another one. (FILE_WRITE argument enables you to read and write into the file.)
  myFile.print(millis() / 1000);                 // Prints Timer in seconds
  myFile.print("\t");                            // prints artificial tab
  myFile.print(rtc.getDateStr());                // Prints Date.
  myFile.print("\t");
  myFile.print(rtc.getTimeStr());  // Prints Time.
  myFile.print("\t");
  myFile.println(distance);  // Prints distance
  myFile.print("\t");
  myFile.println((float)DHT11.humidity);  // Prints humidity
  myFile.print("\t");
  myFile.println((float)DHT11.temperature);  // Prints temperature
  myFile.print("\t");
  myFile.close();  // closes and saves file to the SD card
}
