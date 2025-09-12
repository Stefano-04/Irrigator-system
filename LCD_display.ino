#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11

#define SENSOR1 A1
#define SENSOR2 A2
#define SENSOR3 A3

#define SENSOR_POWER 7

const int AirValue = 1024;
const int WaterValue = 330;

int soilMoistureValue = 0;
int soilmoisturepercent=0;

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

float readTemperature(){

  // read temperature as Celsius
  float tempC = dht.readTemperature();

  // check if any reads failed
  if (isnan(tempC)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  } else {
    //Serial.print("Temperature: ");
    //Serial.print(tempC);
    //Serial.println("°C");
    return tempC;
  }
}
float readHumidity(){
    // read humidity
  float humi  = dht.readHumidity();
  if (isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  } else {
    //Serial.print("Humidity: ");
   // Serial.print(humi);
   // Serial.println("%");
    return humi;
  }
}
enum DisplayState {
  SHOW_TEMP_LABEL,
  SHOW_TEMP_VALUE,
  SHOW_HUM_LABEL,
  SHOW_HUM_VALUE
};
void PrintOnLCDDisplay(float humi, float tempC) {
  static DisplayState state = SHOW_TEMP_LABEL;
  static unsigned long lastUpdate = 0;
  const unsigned long displayInterval = 3000;

  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= displayInterval) {
    lastUpdate = currentMillis;
    lcd.clear();

    switch (state) {
      case SHOW_TEMP_LABEL:
        lcd.setCursor(0, 0);
        lcd.print("Temperature");
        state = SHOW_TEMP_VALUE;
        break;

      case SHOW_TEMP_VALUE:
        lcd.setCursor(0, 1);
        lcd.print(tempC);
        lcd.print((char)176);
        lcd.print("C");
        state = SHOW_HUM_LABEL;
        break;

      case SHOW_HUM_LABEL:
        lcd.setCursor(0, 0);
        lcd.print("Humidity");
        state = SHOW_HUM_VALUE;
        break;

      case SHOW_HUM_VALUE:
        lcd.setCursor(0, 1);
        lcd.print(humi);
        lcd.print("%");
        state = SHOW_TEMP_LABEL;
        break;
    }
  }
}
void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  dht.begin();
  Serial.begin(9600);
  pinMode(SENSOR_POWER,OUTPUT);
}

void loop() {
  int lettura=0;
  int soilmoisturepercent=0;

  float tempC=readTemperature();

  float humi=readHumidity();

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "getdata") {
      Serial.println(F("Start measuring plant in 5s!"));
      digitalWrite(SENSOR_POWER, HIGH); // Give power to sensors
      delay(5000);  // Wait 5 seconds to get sensors warmed up
      lettura=analogRead(SENSOR1); // Read analog data [0-1024]
      Serial.println(lettura); // Print it on Serial monitor
      soilmoisturepercent=map(lettura, AirValue, WaterValue, 0, 100); // Convert it in a centigrade scale, from 0 to 100
      Serial.print("Sensor n.1: "); // Print transformed data onto serial monitor
      Serial.print(soilmoisturepercent,1);
      Serial.print("%| "); 

      lettura=analogRead(SENSOR2);
      soilmoisturepercent=map(lettura, AirValue, WaterValue, 0, 100);
      Serial.print("Sensor n.2: ");
      Serial.print(soilmoisturepercent,1);
      Serial.print("%| "); 

      lettura=analogRead(SENSOR3);
      soilmoisturepercent=map(lettura, AirValue, WaterValue, 0, 100);
      Serial.print("Sensor n.3: ");
      Serial.print(soilmoisturepercent,1);
      Serial.println("%."); 
    }
    else if(command="realtime"){
      Serial.print("Temperature: ");
      Serial.print(tempC,1);
      Serial.print("°C,"); 
      Serial.print("Humidity: ");
      Serial.print(humi,1);
      Serial.println("%"); 
    }
  digitalWrite(SENSOR_POWER, LOW);
  }

  PrintOnLCDDisplay(humi, tempC);


}
