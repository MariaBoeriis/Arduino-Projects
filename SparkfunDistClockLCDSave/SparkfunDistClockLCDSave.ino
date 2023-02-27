
//#include <Adafruit_Sensor.h>

#include <DS3231.h>

#include <SPI.h>
#include <SD.h>
String filename="testny.txt";
//rgb_lcd lcd;
File myFile;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231  rtc(SDA, SCL); //SDA og SCL fra baade clockmodul og display er forbundet til SDA og SCL indgange, ved at lave faelles kanaler på breadboard 
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

const int chipSelect = 10;

void setup() { 
rtc.begin();
Serial.begin(9600); 
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");
  lcd.init();                      // initialize the lcd 
  lcd.backlight();

 
    // Print a message to the LCD.
    lcd.print("test!");

  // SD.begin(10);        //Starts SD card reader at the chip select pin 
    if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  myFile = SD.open(filename, FILE_WRITE); //creates a file on the SD card. If the data.txt file already exists,  Arduino will open the file instead of creating another one. (FILE_WRITE argument enables you to read and write into the file.)
  //myFile = SD.open(filename, FILE_WRITE); //creates a file on the SD card. If the data.txt file already exists,  Arduino will open the file instead of creating another one. (FILE_WRITE argument enables you to read and write into the file.)
  myFile.print("Timer");
  myFile.print("\t");
  myFile.print("Date");
  myFile.print("\t");
  myFile.print("Time");
  myFile.print("\t");
  myFile.println("Distance (mm)");
  myFile.close();

    delay(1000);
} 

void loop() { 
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

Serial.print("Clock: ");
Serial.println(rtc.getTimeStr());
Serial.print("Distance: ");
Serial.println(distance); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist          mm");
  lcd.setCursor(6, 0);
  lcd.print(distance);
  lcd.setCursor(0, 1);
  lcd.print("Clock           ");
  lcd.setCursor(6, 1);
  lcd.print(rtc.getTimeStr());

    myFile = SD.open(filename, FILE_WRITE); //creates a file on the SD card. If the data.txt file already exists,  Arduino will open the file instead of creating another one. (FILE_WRITE argument enables you to read and write into the file.)
  myFile.print(millis() / 1000);                    //Prints Timer in seconds
  myFile.print("\t");
  myFile.print(rtc.getDateStr());                   //Prints Date.
  myFile.print("\t");
  myFile.print(rtc.getTimeStr());                   //Prints Time.
  myFile.print("\t");
  myFile.println(distance);
  myFile.close(); 
//delay(60000); 
delay(1000);
}
