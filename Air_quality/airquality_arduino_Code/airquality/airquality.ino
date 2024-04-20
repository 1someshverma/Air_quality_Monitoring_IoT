#include <Wire.h>
#include "DHTesp.h"
#include "ThingSpeak.h"

// const int DHT_PIN = 15;
// const int LED_PIN = 13;
const char* WIFI_NAME = "SomeshVerma";
const char* WIFI_PASSWORD = "alphabeta123";
const int myChannelNumber =2501109 ;
const char* myApiKey = "4MF906R4JQFA26DD";
const char* server = "api.thingspeak.com";

DHTesp dhtSensor;
WiFiClient client;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include "DHT.h"

#define DHTPIN 3
//----------------------------------------Sensors------------------------
#define DHTTYPE DHT11
#define MQ135 A0
#define Flame_Sen 2
#define MQ2 4
//-----------------------------------output-------------------------------------
#define Red 8
#define Green 9
#define Buzzer 10
//--------------------------------------------------------------------
int MQ2_output;
int F_output;
int Gas_Sensor;

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  
Serial.begin(9600);
lcd.init();                      
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("Air Quality");
lcd.setCursor(0, 1);
lcd.print("System");
delay(1000);
dht.begin();
pinMode(MQ135,OUTPUT);
pinMode(Flame_Sen,OUTPUT);
pinMode(MQ2,OUTPUT);
pinMode(Green,OUTPUT);
pinMode(Red,OUTPUT);
pinMode(Buzzer,OUTPUT);
lcd.clear();

WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Wifi not connected");
  }
  Serial.println("Wifi connected !");
  Serial.println("Local IP: " + String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

}

void loop() {
 Gas_Sensor=analogRead(MQ135);
 Gas_Sensor=map(Gas_Sensor,0,1023,0,100);
 Serial.print("Carbon_monoxide Present in air=");
 Serial.print(Gas_Sensor);
 Serial.println("%");
 delay(200);
 F_output=digitalRead(Flame_Sen);
 MQ2_output=digitalRead(MQ2);
 if(F_output==HIGH)
{
  Serial.println("Fire Detected Pkease take action");
 }

 if(MQ2_output==HIGH)
 {
  Serial.println("Smoke Detected Please Take Action as soon as posible");
 }
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  lcd.setCursor(0, 0);
  lcd.print("Air Temp=");
  lcd.setCursor(9, 0);
  lcd.println(t);
  lcd.setCursor(0, 1);
  lcd.print("Humidity=");
  lcd.setCursor(10, 1);
  lcd.println(h);
  lcd.setCursor(1, 2);
  lcd.print("Gas=");
  lcd.setCursor(5, 2);
  lcd.println(Gas_Sensor);
  lcd.setCursor(7, 2);
  lcd.println("%");
  if(((t>25)&&(t<35))||(F_output==LOW)||(MQ2_output==LOW))// when everthing normal
  {
    digitalWrite(Red,LOW);
    digitalWrite(Green,HIGH);
    digitalWrite(Buzzer,LOW);
  }
  if((t>36)||(F_output==HIGH)||(MQ2_output==HIGH) || gas_sensor>20)
  {
    digitalWrite(Red,HIGH);
    digitalWrite(Green,LOW);
    digitalWrite(Buzzer,HIGH);
  }
  if(t>36)
  {
    Serial.println("Temperature Incerease Please take action ASAP");
  }
  ThingSpeak.setField(1,data.temperature);
  ThingSpeak.setField(2,Gas_Sensor);
  ThingSpeak.setField(2,h);
  int x = ThingSpeak.writeFields(myChannelNumber,myApiKey);
 if(x == 200){
    Serial.println("Data pushed successfull");
  }else{
    Serial.println("Push error" + String(x));
  }
  Serial.println("---");

  delay(10000);

}
