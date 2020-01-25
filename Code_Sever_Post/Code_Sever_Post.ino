#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
 
void setup() {
 
  Serial.begin(115200);                            //Serial connection
  WiFi.begin("Minipan", "999999999");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 

    
    int val11;
    float val2;
    float temp;
 
    float current_mA = 0;
    float power_mW = 0;

    val11 = analogRead(A0);
    temp = val11/4.092;
    val11 = (int)temp;
    val2 = ((val11%100)/10.0);
    
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
  
    JSONencoder["sensorType"] = "val2";
  

      if(current_mA>=0){
        
      
        JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
        values.add(val2); //Add value to array30
        values.add(current_mA);
        values.add(power_mW);
  
      }
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://172.20.10.10:3500/pv/testing/system/api-dev/detail");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(30000);  //Send a request every 30 seconds
 
}
