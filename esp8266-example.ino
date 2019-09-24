// Sample code to connect to a network, connect to an MQTT server, allow
// OTA updates via HTTP
//
// The MQTT channels are based off the word "example" and the last 6 digits
// of the MAC, followed by "test"
//
// eg example/123456/test
//
// Uses PubSubClient library on top of the ESP8266WiFi one

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "network_conn.h"

#define _mqttBase    "example"

// For MQTT
WiFiClient espClient;
PubSubClient client(espClient);

char mqttChannel[30];  // Enough for Base + "/" + 6 hex digits + "/" + channel

// Generate log messages on the serial port
void log_msg(String msg)
{
  time_t now = time(nullptr);
  String tm = ctime(&now);
  tm.trim();
  tm = tm + ": " + msg;
  Serial.println(tm);
}

void setup()
{
  // Let's create the channel names based on the MAC address
  unsigned char mac[6];
  char macstr[7];
  WiFi.macAddress(mac);
  sprintf(macstr, "%02X%02X%02X", mac[3], mac[4], mac[5]);
  sprintf(mqttChannel, "%s/%s/test", _mqttBase, macstr);

  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);     // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) // Wait for the Wi-Fi to connect
  {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname:\t");
  Serial.println(WiFi.hostname());

  // Get the current time.  Initial log lines may be wrong 'cos
  // it's not instant.  Timezone handling... eh, this is just for
  // debuging, so I don't care.  GMT is always good :-)
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  delay(1000);

  // Now we're on the network, setup the MQTT client
  client.setServer(mqttServer, mqttPort);

#ifdef NETWORK_UPDATE
   __setup_updater();
#endif

}

void loop()
{

 // Try to reconnect to MQTT each time around the loop, in case we disconnect
  while (!client.connected())
  {
    log_msg("Connecting to MQTT Server " + String(mqttServer));

    // Generate a random ID each time
    String clientId = "ESP8266Client-bme820-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      log_msg("MQTT connected.  I am " + WiFi.localIP().toString());
    } else {
      log_msg("failed with state " + client.state());
      delay(2000);
    }
  }

  // Keep MQTT alive
  client.loop();

  // Send data
  client.publish(mqttChannel, "Hello, I'm alive!");

#ifdef NETWORK_UPDATE
  __netupdateServer.handleClient();
#endif

  delay(1000);
}
