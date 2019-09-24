const char* ssid       = "YOURSSID";
const char* password   = "YOUR_PASSWD";
const char* mqttServer = "YOUR.MQTT.SERVER";
const int mqttPort     = 1883;

#define NETWORK_UPDATE 1

#ifdef NETWORK_UPDATE

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer __netupdateServer(8266);
ESP8266HTTPUpdateServer __networkUpdater;

void __setup_updater()
{
  __networkUpdater.setup(&__netupdateServer);
  __netupdateServer.begin();
}

#endif
