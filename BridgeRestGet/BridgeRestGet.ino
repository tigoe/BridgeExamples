/*
  RGB LED controller using REST interface

 This example shows how to use the Bridge Library's get() command
 to get data from the linux processor to the Arduino processor.
 That data can be put in the linux processor's data store using the
 REST API like so:
 
http://arduino.local/data/put/red/xxx
http://arduino.local/data/put/green/xxx
http://arduino.local/data/put/blue/xxx

Where xxx is a numeric string from 0 -255
 The circuit:
 * Common cathode RGB LED on pins 8 - 11.
 *    Pin 8: cathode
 *    Pin 9: green
 *    Pin 10: blue
 *    Pin 11: red
 created  4 Mar 2015
 by Tom Igoe

 */

#include <Bridge.h>

char dataBuffer[4];
int red, green, blue;
String dataString;

void setup() {
  // initiate Bridge connection to linux processor:
  Bridge.begin();
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
}

void loop() {
  Bridge.get("red", dataBuffer, 3);  // get the red value
  dataString = String(dataBuffer);   // convert to a String
  red = dataString.toInt();          // convert String to int

  Bridge.get("green", dataBuffer, 3);// get the green value
  dataString = String(dataBuffer);   // convert to a String
  green = dataString.toInt();        // convert String to int

  Bridge.get("blue", dataBuffer, 3);  // get the blue value
  dataString = String(dataBuffer);    // convert to a String
  blue = dataString.toInt();          // convert String to int
  
  // turn on the LED channels:
  analogWrite(11, red);
  analogWrite(9, green);
  analogWrite(10, blue);
  delay(100);
}
