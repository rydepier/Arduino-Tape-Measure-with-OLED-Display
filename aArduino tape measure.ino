/***********************************************************
 * 
 * Arduino Tape Measure with OLED Display
 * 
 * By Chris Rouse Nov 2015
 * 
 * HC-SRO4 Connections:
 * VCC connection of the sensor attached to +5V
 * GND connection of the sensor attached to ground
 * TRIG connection of the sensor attached to digital pin 2
 * ECHO connection of the sensor attached to digital pin 3
 * 
 * OLED Connections:
 * VCC +5v
 * GND GND
 * SDA Analog pin 4
 * SCL Analog pin 5
 * 
 * 
 ********************************************************/

 #include "U8glib.h"
 #include <SPI.h>
 #include <Wire.h>
// setup u8g object
  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C 
//
const int trigPin = 2;
const int echoPin = 3;
long duration, inches, cm, avgCm; 
String longString="";
int tapeLength = 0;

void setup() {
 Serial.begin(9600);
 Wire.begin();
}

void loop() {
  avgCm = 0;
  for(int f = 0; f <5; f++){ // average out 5 readings
    ultrasonic();
    avgCm = avgCm + cm;
  }
  cm = avgCm/5;
  //draw loop
  u8g.firstPage();  
  do {
    draw(); 
  } while( u8g.nextPage() );
  delay(50);
}

void ultrasonic()
{
pinMode(trigPin, OUTPUT);
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
inches = microsecondsToInches(duration);
cm = microsecondsToCentimeters(duration);
Serial.print(inches);
Serial.print("in, ");
Serial.print(cm);
Serial.print("cm");
Serial.println();
delay(100);
}
long microsecondsToInches(long microseconds)
{
return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}


void draw(void){
  u8g.setFont(u8g_font_profont12); 
  u8g.drawStr(5,8, "Arduino Tape Measure"); 
  if(cm > 110){
    longString = "---";
    tapeLength = 0;
  }
  else{
    longString=String(cm)+ "cm";
    tapeLength = cm;
  }
  const char* newData = (const char*) longString.c_str();
  u8g.setFont(u8g_font_profont29);  
  u8g.drawStr(30,40, newData); 
  // now draw tape
  for(int f = 10;f <120; f = f+10){
    u8g.drawLine(f,48,f,61);
  }
  for(int f = 2; f< tapeLength+3; f++){
    u8g.drawBox(f,50,1,10);
  } 
}
