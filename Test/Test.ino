#include <Ota_updater.h>
//#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 25
#define DHTTYPE DHT11

#define OTA_TITLE "Test"
#define MQTT_CLIENT_ID "test"
#define TOPIC_CONSIGNA "your/topic/here"
#define TOPIC_ON_OFF "your/topic/here"

char* TOPIC_DATA = "your/topic/here";

WiFiClientSecure espClient;

PubSubClient client(espClient);
Ota_updater ota(OTA_TITLE);
TaskHandle_t Task1;

DHT dht(DHTPIN, DHTTYPE);

float temperature, humidity, pressure, altitude;
char buffer[128], ip[64];
char temp[50];
long lastMsg = 0;
bool wifi_first = true;
String connected_mqtt = "-";
String _status = "";

const char* ssid = "**********";
const char* password = "****************";
const char* mqtt_server = "*******************";
int mqtt_port = 8883;

const char* mqtt_user_id = MQTT_CLIENT_ID;
const char* mqtt_user_name = "**************";
const char* mqtt_user_pass = "***********";

const char* mosq_cert_ca = "";


void setup() {
  Serial.begin(115200);

  dht.begin();

  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 0, &Task1, 0);

  delay(1000);

  espClient.setCACert(mosq_cert_ca);
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  ota.begin();

}

void Task1code( void * parameter) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  for(;;) {
    wifi_first = false;
    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("Connecting");
      WiFi.disconnect();
      WiFi.reconnect();
      delay(20000);
      Serial.println("IP");
      Serial.println(WiFi.localIP());
    }

    if (!client.connected()) {
      reconnect();
    }
    delay(1000);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  
  Serial.println(messageTemp);
  
}


void reconnect() {  
  Serial.println("Connecting to MQTT");
  // Attempt to connect
  if (client.connect(mqtt_user_id,mqtt_user_name,mqtt_user_pass)) {
    Serial.println("Connected to MQTT as Test");
    connected_mqtt = "Ok";
//    client.subscribe(TOPIC_CONSIGNA);
//    client.subscribe(TOPIC_ON_OFF);
  } else {
    Serial.print("Not connected: ");
    Serial.println(client.state());
    // Wait 5 seconds before retrying
    connected_mqtt = "-";
    delay(100);
  }
}

void loop() {
  ota.myhandleClient();
  client.loop();

  long now = millis();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float pressure = 0.0;
  float altitude = 0.0;
    
//  Serial.print("Temperatura: ");
//  Serial.print(temperature);
//  Serial.print(" - Humedad: ");
//  Serial.print(humidity);
//  Serial.print(" - Presion: ");
//  Serial.print(pressure);
//  Serial.print(" - Atitud: ");
//  Serial.println(altitude);
  
  if (now - lastMsg > 60 * 1000) {
    lastMsg = now;

    sprintf(ip,"%s.%s.%s.%s",String(WiFi.localIP()[0]),String(WiFi.localIP()[1]),String(WiFi.localIP()[2]),String(WiFi.localIP()[3]));
    sprintf(buffer,"%.2f,%d,%.2f,%.2f,%.2f,%s,%s",temperature, 0.0, humidity, pressure, altitude, ip, _status);
    client.publish(TOPIC_DATA, buffer, true);
  }

  delay(1000);

}
