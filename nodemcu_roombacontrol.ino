
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

//Web server off an example from this library https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi


// Serial1 Create2 connection
int rxPin=21;
int txPin=23;
int lastByte=0;
int sensorVal=0;
int ddPin = 22;

SoftwareSerial Roomba;

/*

  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Go to http://192.168.4.1
        
*/

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
  Roomba.begin(19200, rxPin,txPin);

  pinMode(ddPin, OUTPUT);
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);


  Roomba.write(128);  //Start
  Roomba.write(131);  //Safe mode
  delay(1000);
}

int getBattery() //work in progress
{
  Roomba.write(142);
  Roomba.write(25);
  float batVal = Roomba.read();
  Roomba.write(142);
  Roomba.write(26);
  float batCap = Roomba.read();
  return (batVal/batCap)*100;
}


void setDigitLEDs(byte digit1, byte digit2, byte digit3, byte digit4)
{
    Roomba.write(163);
    Roomba.write(digit1);
    Roomba.write(digit2);
    Roomba.write(digit3);
    Roomba.write(digit4);
}


void setPowerLED(byte setColor, byte setIntensity)
{
    int color = setColor;
    int intensity = setIntensity;
    Roomba.write(139);
    Roomba.write((byte)0x00);
    Roomba.write((byte)color);
    Roomba.write((byte)intensity);
}

void driveWheels(int right, int left)
{
  Roomba.write(145);
  Roomba.write(right >> 8);
  Roomba.write(right);
  Roomba.write(left >> 8);
  Roomba.write(left);
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            //client.print("<meta http-equiv=\"refresh\" content=\"1\">");
            client.print("<a href=\"/W\" ><button type=\"button\" style=\"width:90px;height:90px;margin-left:45%;\">^</button></a> <br><br>");
            client.print("<a href=\"/A\"><button type=\"button\" style=\"width:90px;height:90px;margin-left:40%;\"><</button></a> <a href=\"/S\"><button type=\"button\" style=\"width:90px;height:90px;margin-left:0%;\">v</button></a> <a href=\"/D\"><button type=\"button\" style=\"width:90px;height:90px;margin-left:0%;\">></button></a> <br>");
           // client.print("<img src=\"https://i.imgur.com/fAcyGS7.jpg\" alt=\"Italian Trulli\">");
            client.print("<br><br> battery value here");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
          
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        
        Serial.println(currentLine);

        //This is pretty simple, checks to see if the HTML request ends in any of these letters
        //A letter corresponds to a direction (basically arrow keys)
        
        if (currentLine.endsWith("GET /W")) {
            
          driveWheels(255,255);
          delay(1000);
          driveWheels(0,0);
         
        }
        if (currentLine.endsWith("GET /A")) {
              
          driveWheels(255,-255);
          delay(100);
          driveWheels(0,0);
        }
        if (currentLine.endsWith("GET /S")) {
          
          driveWheels(-255,-255);
          delay(1000);
          driveWheels(0,0);
        }
        if (currentLine.endsWith("GET /D")) {
              
          driveWheels(-255,255);
          delay(100);
          driveWheels(0,0);
        }

        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
