/*
  Temperature web interface
  Context:  Arduino, for Yun

  This example shows how to use the Bridge Library's put() command
  to send data from the Arduino processor to the linux processor.
  That data can later be requested from the linux processor using the
  REST API like so:
  http://arduino.local/data/get/temperature

  The circuit:
  * TMP36 temperature sensor on analog pin A1
  * SD card attached to SD card slot of the Arduino Yún

  created  24 Feb 2014
  modified 4 Mar 2015
  by Tom Igoe
 */

#include <Bridge.h>

void setup() {
  // initiate Bridge connection to linux processor:
  Bridge.begin();
  Serial.begin(9600);

  // using A0 and A2 as vcc and gnd for the TMP36 sensor:
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A2, LOW);
}

void loop() {
  int sensorValue = analogRead(A1);
  // convert the reading to millivolts:
  float voltage = sensorValue *  (5000 / 1024);
  // convert the millivolts to temperature celsius:
  float temperature = (voltage - 500) / 10;
  // make a String to output the data:
  String output = String(temperature);
  // print it to the Serial Monitor:
  Serial.println(output);
  Serial.println("uploading data for REST API");
  // send it to the Linux database:
  Bridge.put("temperature", output);
  // wait five seconds before updating again:
  delay(5000);
}
