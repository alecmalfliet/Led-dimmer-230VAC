#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <Wire.h>

#define CLIENT_ID       "UnoMQTT"
#define INTERVAL        3000 // 3 sec delay between publishing

const char* mqtt_server = "192.168.0.193";
const char* mqtt_topic = "Maker";
const char* mqtt_username = "home";
const char* mqtt_password = "199";


uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};

EthernetClient ethClient;
PubSubClient mqttClient;

long previousMillis;

void(* resetFunc) (void) = 0; //Ressteing for eternet reconecting

void setup() {
  // setup serial communication
  Serial.begin(9600);
  
  // setup ethernet communication using DHCP
  if(Ethernet.begin(mac) == 0) {
    Serial.println(F("Ethernet configuration using DHCP failed"));
    for(;;);
  }
  // setup mqtt client
  mqttClient.setClient(ethClient);
  mqttClient.setServer(mqtt_server, 1883);
  
  Serial.println(F("MQTT client configured"));
  
  if(mqttClient.connect(CLIENT_ID, mqtt_username, mqtt_password)) { 
    Serial.println("Connection has been established, well done");
 
    // Establish the subscribe event
    mqttClient.setCallback(subscribeReceive);
  } 
  else 
  {
    Serial.println("Looks like the server connection failed...");
    Serial.println("resetting");
    delay(1000);
    resetFunc();  //call reset
  }
  // Start communication whit the dimmer
  Wire.begin();
  previousMillis = millis();
}

void loop() {
    // This is needed at the top of the loop!
  mqttClient.loop();
 
  // Ensure that we are subscribed to the topic "MakerIOTopic"
  mqttClient.subscribe(mqtt_topic);
 
  // Dont overload the server!
  //delay(20);
}

void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  // Print the topic
  Serial.print("Topic: ");
  Serial.println(topic);
  // Print the message
  Serial.print("Message: ");
  String PayloadString;
  for(int i = 0; i < length; i ++)
  {
    PayloadString = String(PayloadString + char(payload[i]));
  }
  int PayloadInt;
  PayloadInt = PayloadString.toInt();
  
  Serial.println(PayloadInt);

  Wire.beginTransmission(4); // transmit to dimmer #4
  Wire.write("Payload");        
  Wire.write(PayloadInt);              // sends Payload  
  Wire.endTransmission();    // stop transmitting
  delay(200);

}