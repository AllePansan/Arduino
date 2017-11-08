/*
 Basic MQTT example with Authentication
 
  - connects to an MQTT server, providing username
    and password
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =	"My ASUS";		// cannot be longer than 32 characters!
const char *pass =	"saladastreta";		//

// Update these with values suitable for your network.
IPAddress server(172, 16, 0, 2);

void callback(const MQTT::Publish& pub) {
  // handle message arrived
}

WiFiClient wclient;
PubSubClient client(wclient, server);

void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect("arn:aws:lambda:us-east-2:330169925083:function:sync_arduino")
			 .set_auth("gabriel.babler1995@gmail.com", "Gazao1995"))) {
        Serial.println("Connected to MQTT server");
	client.set_callback(callback);
	client.publish("outTopic","hello world");
	client.subscribe("inTopic");
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected())
      client.loop();
  }
}

