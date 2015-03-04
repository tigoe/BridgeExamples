/*
  Bridge Jukebox

  Serves a web page that allows the user to enter the URL of
  an mp3 file. When the user enters an mp3, the sketch downloads
  it using wget on the linux side of the Yun, and saves it to
  an SD card. Then it plays the file using madplay.

  To prepare the linux side:
  * opkg update
  * opkg install kmod-usb-audio
  * opkg install madplay
  * add an SD card
  * upload the whole sketch via wifi, or add the www directory
    of the sketch to /arduino/www/BridgeJukebox/ on the SD card

  created 7 Feb 2014
  modified 23 Jan 2015
  by Tom Igoe
*/

#include <Bridge.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

YunServer server;    // instance of the YunServer library

void setup() {
  Serial.begin(9600);

  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network can connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  YunClient client = server.accept();  // listen for client requests
  Process myProcess;                   // process for downloading and playing file
  String inString = "";                // string to hold the client request

  // if a client is connected:
  if (client) {
    Serial.println("Got a client");
    // read the incoming client request string:
    while (client.available()) {
      inString = client.readStringUntil('\n');
    }
    // if the string's not empty, assume it's an .mp3 file:
    if (inString != "") {
      Serial.println("Downloading song...");
      // trim off the newlines from the request string:
      inString.trim();
      // find out where the = sign is and get everything after it.
      // that'll be the song name:
      int songStart = inString.indexOf("=") + 1;
      inString = inString.subString(songStart);
      // start the command string, and add the song name:
      String command = "wget -O /mnt/sda1/song.mp3 \'";
      command += inString;
      command += "\'";
      Serial.println(command);
      // run the command asynchronously, so you can watch progress
      // in the serial monitor:
      myProcess.runShellCommandAsynchronously(command);
      // downloading large songs can be slow, depending on your network.
      // every half second, print the elapsed waiting time:
      long start = millis();
      while (myProcess.running()) {
        long elapsed = millis() - start;
        if (elapsed % 500 < 5) {
          Serial.print("Elapsed msecs: ");
          Serial.println(elapsed);
        }
      }
      // you've finished downloading:
      Serial.println("got the song");
      // now use the command string to play a song using madplay:
      command = "madplay /mnt/sda1/song.mp3 -a -16";
      Serial.println(command);
      myProcess.runShellCommandAsynchronously(command);
      Serial.println("playing it");
    }
  }
}
