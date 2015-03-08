/*
 Bridge REST parser example
 Context: Arduino, for Yun

Creates a web server on the Arduino Yun. Once your Yun is configured on your network,
 you can go go to http://arduino.local/arduino/any/string/you/want
 to see the output of this sketch.
 
 created 10 Dec 2013
 modified 8 Mar 2015
 by Tom Igoe
 
 */
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

// Server will listen on default port 5555. The webserver on the Yun
// will forward to that port all the HTTP requests for you.
YunServer server;

void setup() {
  Serial.begin(9600);

  // Bridge startup
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network can connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get client request coming from server
  YunClient request = server.accept();

  // Is there a new client?
  if (request) {
    // Process request
    process(request);

    // Close connection and free resources.
    request.stop();
  }

  delay(50); // Poll for requests every 50ms
}

void process(YunClient request) {
  // count the number of tokens/between/slashes:
  int tokenNumber = 0;
  // read from the client request:
  while (request.available() > 0) {
    // read to the next /:
    String inString = request.readStringUntil('/'); 
    // reply to the client with what it sent:
    request.print("Token number ");
    request.print(tokenNumber);
    request.print(": ");
    // print it in the Serial monitor too:
    Serial.println(inString); 
    request.println(inString);
    // if you get a valid numeric string, convert to an int:
    if (inString.toInt()) {
      // let the user know that this token was a number:
      request.print("That's a number, buddy: ");
      request.println(inString.toInt());
    }
    // increment the number of tokens read:
    tokenNumber++;
  }
}

