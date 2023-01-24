#include <WiFi.h>
#include <DHT.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define PIRPANA
#include "secrets.h"

// WiFi parameters

#define WLAN_SSID       WIFI_SSID
#define WLAN_PASS       WIFI_PASS

// Adafruit IO

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

//Enter the username and key from the Adafruit IO

#define AIO_USERNAME    IO_USERNAME
#define AIO_KEY         IO_KEY

WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/home-tampere.sisa-temperature");
float temp = 20.0; //to store the temperature value

void setup() 
{
    Serial.begin(115200);
    Serial.println(F("Adafruit IO Example"));
    // Connect to WiFi access point.
    Serial.print(F("Connecting to "));
    Serial.println(WLAN_SSID);
    WiFi.begin(WLAN_SSID, WLAN_PASS);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(F("."));
    }

    Serial.println();
    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());
    // connect to adafruit io
    connect();
}

// connect to adafruit io via MQTT
void connect() {
  Serial.print(F("Connecting to Adafruit IO… "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) 
  {
      switch (ret) 
      {
          case 1: Serial.println(F("Wrong protocol")); break;
          case 2: Serial.println(F("ID rejected")); break;
          case 3: Serial.println(F("Server unavail")); break;
          case 4: Serial.println(F("Bad user/pass")); break;
          case 5: Serial.println(F("Not authed")); break;
          case 6: Serial.println(F("Failed to subscribe")); break;
          default: Serial.println(F("Connection failed")); break;
      }

      if(ret >= 0) mqtt.disconnect();
      Serial.println(F("Retrying connection…"));
      delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}

void loop() 
{
    // ping adafruit io a few times to make sure we remain connected
    if(! mqtt.ping(3)) 
    {
        // reconnect to adafruit io
        if(! mqtt.connected())
            connect();
    }
    temp = temp + 0.1; 

    Serial.print("temperature = ");
    Serial.println(temp);

    delay(5000);
    if (! Temperature.publish(temp))  //Publish to Adafruit
    {                     
        Serial.println(F("Failed"));
    }
  
      else 
    {
        Serial.println(F("Sent!"));

    }
}