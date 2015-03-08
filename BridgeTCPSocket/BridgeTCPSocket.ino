/*
 socket on the Yun
 Context:  Arduino, for Yun

 This example opens a TCP socket on port 8080

 The circuit: none

 created 17 Sept 2013
 modified 8 Mar 2015
 by Tom Igoe

 */

#include <Process.h>
Process socket;         // create a new instance of the Process class

void setup() {
  Bridge.begin();	// Initialize the Bridge
  Serial.begin(9600);	// Initialize the Serial

  // Wait until the Serial Monitor  on the IDE is opened:
  while (!Serial);
  // start a socket connection to the host on port 8080:
  socket.runShellCommandAsynchronously("telnet 192.168.0.10 8080");

}

void loop() {
  // If the process is running, listen for input:
  if (socket.running()) {
    if (Serial.available() > 0) {  // if there's serial input,
      char c = Serial.read();      // read serial in
      socket.write(c);             // send it to the socket process
      Serial.write(c);             // echo it back locally
    }
  }

  // listen for bytes from the socket process:
  if (socket.available()) {
    char c = socket.read();      // read TCP data in
    Serial.write(c);            // send it to the serial port
  }
}






