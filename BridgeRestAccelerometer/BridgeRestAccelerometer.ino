/*
 Bridge REST accelerometer example
 Context: Arduino, for Yun

 Creates a web server on the Arduino Yun. Once your Yun is configured on your network,
 you can go to http://arduino.local/arduino/accelerometer/x or /y or /z
 to see the output of this sketch.

 A 3-axis accelerometer is attached to pins A0, A1, and A2.

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
int accelerometer[3];    // array for the accelerometer values

void setup() {
  Serial.begin(9600);

  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost.
  // no one from the external network can connect. The main server on
  // the Yun passes requests for /arduino to this port.
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  YunClient request = server.accept();

  // Is there a new request?
  if (request) {
    process(request);  // Handle it
    request.stop();    // Close connection and free resources.
  }

  delay(50); // Poll for requests every 50ms
}

void process(YunClient request) {
  // if there's any request from the client:
  if (request.available() > 0) {
    // read into a String until you get a /
    String inString = request.readStringUntil('/');
    Serial.println(inString);
    inString.trim(); // trim any whitespace
    // if the client contains "/accelerometer/":
    if (inString == "accelerometer") {
      // get the sensor readings:
      accelerometerRead();
      // now look for x, y, or z after the /:
      char token = request.read();
      // assemble a JSON string to return to the client:
      String content = "{";
      switch (token) {
        case 'x':      // if the client asks for x, send {"x": xValue}
          content += "\"x\":";
          content += accelerometer[0];
          content += "}";
          break;
        case 'y':      // if the client asks for y, send {"y": yValue}
          content += "\"y\":";
          content += accelerometer[1];
          content += "}";
          break;
        case 'z':      // if the client asks for z, send {"z": zValue}
          content += "\"z\":";
          content += accelerometer[2];
          content += "}";
          break;
        default:      // if the client asks for just /accelerometer. send all three:
          content = makeJson();
      }
      // send the JSON string to the client:
      request.println(content);
    }
  }
}

void accelerometerRead() {
  // read the accelerometer values into the array:
  accelerometer[0] = analogRead(A0);
  delay(1);
  accelerometer[1] = analogRead(A1);
  delay(1);
  accelerometer[2] = analogRead(A2);
}

// make a JSON string with all the acceleromter values:
String makeJson() {
  String result = "{\"x\":";
  result += accelerometer[0];
  result += ",\"y\":";
  result += accelerometer[1];
  result += ",\"z\":";
  result += accelerometer[2];
  result += "}";
  return result;
}
