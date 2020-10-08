#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <Wire.h>    

uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};

EthernetClient ethClient;
PubSubClient mqttClient;

void(* resetFunc) (void) = 0; //Ressteing for eternet reconecting

void setup() {
  // setup serial communication
  Serial.begin(9600);
  
  // setup ethernet communication using DHCP
  if(Ethernet.begin(mac) == 0) {
    //Serial.println(F("Ethernet configuration using DHCP failed"));
    for(;;);
  }
  // setup mqtt client
  mqttClient.setClient(ethClient);
  mqttClient.setServer("192.168.0.193",1883);
  
  //Serial.println(F("MQTT client configured"));
  
  if(mqttClient.connect("UnoMQTT", "home", "199")) { 
    //Serial.println("Connection has been established, well done");
 
    // Establish the subscribe event
    mqttClient.setCallback(subscribeReceive);
  } 
  else 
  {
    //Serial.println("Looks like the server connection failed...");
    //Serial.println("resetting");
    delay(1000);
    resetFunc();  //call reset
  }
  // Start communication whit the dimmer
  Wire.begin();
 
}

void loop() {
    // This is needed at the top of the loop!
  mqttClient.loop();
 
  // Ensure that we are subscribed to the topic "MakerIOTopic"
  mqttClient.subscribe("Maker");
 
  // Dont overload the server!
  //delay(20);
}

void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  String PayloadString;
  for(int i = 0; i < length; i ++)
  {
    PayloadString = String(PayloadString + char(payload[i]));
  }
  
  Serial.println(PayloadString.toInt());

  Wire.beginTransmission(4); // transmit to dimmer #4
  //Wire.write("Payload");        
  Wire.write(PayloadString.toInt());              // sends Payload  
  Wire.endTransmission();    // stop transmitting
  delay(200);

}