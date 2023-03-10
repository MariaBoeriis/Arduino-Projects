
//include libraries
#include <DS3231.h> // Library for Time Clock module
#include <SPI.h> // Library for SD Card reader
#include <SD.h> // Library for SD Card reader
String filename = "Karsedata nr. 2.txt";

//rgb_lcd lcd;
File myFile;
#include <LiquidCrystal_I2C.h> // Library for LCD screen
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231  rtc(SDA, SCL); //SDA og SCL fra baade clockmodul og display er forbundet til SDA og SCL indgange, ved at lave faelles kanaler på breadboard 
#include "SparkFun_VL53L1X.h" // Library for distance sensor

#include <dht11.h> // Library for DHT11 Humidity Sensor

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3
#define DHT11PIN 7

dht11 DHT11;
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


  // SD.begin(10);        //Starts SD card reader at the chip select pin 
    if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

  Serial.println("card initialized.");
  myFile = SD.open("filename.TXT", FILE_WRITE); //creates a file on the SD card. If the data.txt file already exists,  Arduino will open the file instead of creating another one. (FILE_WRITE argument enables you to read and write into the file.)
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
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  float distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

Serial.print("Clock: ");
Serial.println(rtc.getTimeStr());
Serial.print("Distance: ");
Serial.println(distance, 2); 
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Dist          mm");
  // lcd.setCursor(6, 0);
  // lcd.print(distance);
  // lcd.setCursor(0, 1);
  // lcd.print("Clock           ");
  // lcd.setCursor(6, 1);
  // lcd.print(rtc.getTimeStr());

    myFile = SD.open("filename.TXT", FILE_WRITE); //creates a file on the SD card. If the data.txt file already exists,  Arduino will open the file instead of creating another one. (FILE_WRITE argument enables you to read and write into the file.)
  myFile.print(millis() / 1000);                    //Prints Timer in seconds
  myFile.print("\t");
  myFile.print(rtc.getDateStr());                   //Prints Date.
  myFile.print("\t");
  myFile.print(rtc.getTimeStr());                   //Prints Time.
  myFile.print("\t");
  myFile.println(distance);
  myFile.println((float)DHT11.humidity);
  myFile.println((float)DHT11.temperature);
  myFile.close();

int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature   (C): ");
  Serial.println((float)DHT11.temperature, 2);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("H: (%)");
    lcd.setCursor(6, 0);
    lcd.print((float)DHT11.humidity, 2);
    lcd.setCursor(0, 1);
    lcd.print("T: (C)");
    lcd.print((float)DHT11.temperature, 2);
    
//Måler i intervaller af 10 min; 
delay(600000);

}
