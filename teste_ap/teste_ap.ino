/* Create a WiFi access point and provide a web server on it. 
** For more details see http://42bots.com.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

IPAddress    apIP(42, 42, 42, 42);  // Defining a static IP address: local & gateway
                                    // Default IP in AP mode is 192.168.4.1

/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "ESP8266";
const char *password = "ESP8266Test";

// Define a web server at port 80 for HTTP
ESP8266WebServer server(80);

const int ledPin = D1; // an LED is connected to NodeMCU pin D1 (ESP8266 GPIO5) via a 1K Ohm resistor

bool ledState = false;

void handleRoot() {
  Serial.println(server.arg("funcao"));
  Serial.println(server.arg("back"));
  Serial.println(server.arg("main"));
  if(server.arg("testeConnect")){
    server.send ( 200, "text/plain", "i'm here" );
  }
}

void testConnect() {
  
}

void handleNotFound() {
  digitalWrite ( D0, 0 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( D0, 1 ); //turn the built in LED on pin D0 of NodeMCU off
}

void setup() {
  pinMode ( ledPin, OUTPUT );
  digitalWrite ( ledPin, 0 );
  
  delay(1000);
  Serial.begin(115200);
  //Serial.println();
  //Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
  server.on ( "/", handleRoot );
  server.on ( "/funcao", handleRoot);
  server.on ( "/back", handleRoot); 
  server.on ( "/main", handleRoot);
  server.on ( "/testConnect", handleRoot);
  /*server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );*/
  server.onNotFound ( handleNotFound );
  
  server.begin();
  //Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
