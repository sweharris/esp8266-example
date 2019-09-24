# esp8266-example
Example of a core esp8266 with MQTT and OTA update abilities

Typically when I start a new sketch I need 3 core functions:
  * Join the network
  * Talk MQTT
  * Allow for remote updates

I don't use mDNS because my IoT network assigns static IPs via DHCP.

This code is essentially a core template that I use.  I split out the
defines for network connectivity to a separate file so that this file
can be added to .gitignore.  It means I can publish my sketches to git
without exposing passwords.

I'm a command line user, so the Makefile means I cat just type `make`
to build a sketch and `make netupload host=remote.ip.address`

Compilation is done using [arduino-cli](https://github.com/arduino/arduino-cli)
