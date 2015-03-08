/*
  Bridge curl client
  Context:  Arduino, for Yun

  This program uses the Process class of the Bridge library for the Arduino Yun
  to connect to a server and make an HTTP GET request, with parameters.

 created 10 Oct 2013
 modified 8 Mar 2015
 by Tom Igoe
 */
#include <Process.h>    // include the Process class of the Bridge lib

void setup() {
  Bridge.begin();      // Initialize Bridge
  Serial.begin(9600);  // Initialize Serial
}

void loop() {
  // start a new Process, add the command and the parameters:
  Process p;
  p.begin("curl");                     // the command you'll run
  p.addParameter("http://www.example.com/age_checker.php");  // the URL
  p.addParameter("-d");                // tells curl to include data
  p.addParameter("name=Bob&age=23");   // the data to include
  p.addParameter("-i");                // include HTTP headers too
  p.run();                             // run the command

  // take any text that the command outputs
  // and print it to the serial port:
  while (p.available() > 0) {
    char c = p.read();
    Serial.print(c);
  }

  // do nothing more forever:
  while (true);

}


