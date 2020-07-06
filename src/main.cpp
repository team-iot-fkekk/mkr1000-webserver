/**WiFi Web Server
 * A simple web server
 * created 13 July 2010 by dlf (Metodo2 srl)
 * modified 31 May 2012 by Tom Igoe
 * modified Jan 2020 by Gary Sims
 * modified 30 March 2020 by team-iot-fkekk (revisit 7 July 2020) again
 */

#include <SPI.h>
#include <WiFi101.h>
#include <secrets.h>
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = YOUR_SSID;     // your network SSID (name)
char pass[] = YOUR_PASSWORD; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
int ledpin = 6;
WiFiServer server(80);
void printWiFiStatus();
void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true)
      ;
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WiFi network named: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWiFiStatus();
}

void loop()
{
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("new client");
    // an http request ends with a blank line
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n')
        { // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<p>Welcome to the world's smallest web server.");
            client.println("<p>Thanks for visiting.");
            // print the received signal strength:
            long rssi = WiFi.RSSI();
            client.println("<p><em>Signal strength (RSSI):");
            client.print(rssi);
            client.println(" dBm</em><br>");

            // the content of the HTTP response follows the header:
            client.println("<p>Click <a href=\"/H\">here</a> turn the LED on pin 6 ON<br>");
            client.println("<p>Click <a href=\"/L\">here</a> turn the LED on pin 6 OFF<br>");

            // The HTTP response ends with another blank line:
            client.println("</html>");
            client.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H"))
        {
          digitalWrite(ledpin, HIGH); // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L"))
        {
          digitalWrite(ledpin, LOW); // GET /L turns the LED off
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWiFiStatus()
{
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}