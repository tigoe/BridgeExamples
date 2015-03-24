/*
  Check Linux version

 This sketch checks the system information of the linux
 distribution on the Arduino Yún. It runs a
 process on the linux side to check uname -a

 created 24 Mar 2015
 by Tom Igoe

 */

#include <Process.h>

void setup() {
  Bridge.begin();     // Initialize Bridge
  Serial.begin(9600); // Initialize serial communications
  while (!Serial);    // Wait until a Serial Monitor is connected

  Process cli;        // Create a process ("cli" = "command line interface")
  cli.runShellCommand("uname -a");	// run the uname command

  // read from the command line response:
  while (cli.available() > 0) {
    char c = cli.read();
    Serial.write(c);
  }
}

void loop() {
  // Nothing happening here
}
