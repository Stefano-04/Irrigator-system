#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <MyLogin.h>

#define RX 16
#define TX 17
#define RELE 4 // GPIO4

const char* SSID     = MY_SSID;
const char* PASSWORD = MY_PASSWORD;

const String BOT_TOKEN = BOT_TOKEN_IRRIGATOR;
const String CHAT_ID = CHAT_ID_BOT_IRRIGATOR;

const char* info =
  "Commands:\n\n"
  "/water X start watering for X seconds\n"
  "/realtime display realtime measurements from Arduino Board\n"
  "/getdata to receive instant data from Arduino soil moisture sensors";


WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

int getNumMessages() {
  return bot.getUpdates(bot.last_message_received + 1);
}

void handleNewMessages(int numMessages) {
  for (int i = 0; i < numMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) continue;

    String text = bot.messages[i].text;
    Serial.println(text);

    if (text == "/start") {
      bot.sendMessage(chat_id, info, "");
    }

    else if (text.startsWith("/water")) {
      int seconds = 0;
        if (sscanf(text.c_str(), "/water %d", &seconds) == 1 && seconds > 0) {
        bot.sendMessage(chat_id, "Watering for " + String(seconds) + " seconds...", "");
        digitalWrite(RELE, LOW);
        delay((seconds+11) * 1000);
        digitalWrite(RELE, HIGH);
        bot.sendMessage(chat_id, "Done watering!", "");
        } 
        else {
          bot.sendMessage(chat_id, "Usage: /water <seconds> (e.g. /water 10)", "");
        }
      }
else if (text == "/realtime") {
  bot.sendMessage(chat_id, "Requesting real time measurements from Arduino...", "");

  Serial2.println("realtime");  // Comando corretto, senza underscore

  // Aspetta risposta con timeout controllato
  unsigned long startTime = millis();
  String arduinoData = "";
  while (millis() - startTime < 5000) {  // timeout 5s
  if (Serial2.available()) {
    arduinoData = Serial2.readStringUntil('\n');
    arduinoData.trim();
    break;  // esco appena leggo una linea
  }
}

  if (arduinoData.length() > 0) {
    Serial.println(arduinoData);
    bot.sendMessage(chat_id, "Arduino: " + arduinoData, "");
  } else {
    bot.sendMessage(chat_id, "⚠️ Nessuna risposta da Arduino.", "");
  }
}
else if (text == "/getdata") {
  bot.sendMessage(chat_id, "Requesting data from Arduino...", "");

  Serial2.println("getdata");  // Comando corretto, senza underscore

  // Aspetta risposta con timeout controllato
  unsigned long startTime = millis();
  String arduinoData = "";
  while (millis() - startTime < 5000) {  // timeout 5s
  if (Serial2.available()) {
    arduinoData = Serial2.readStringUntil('\n');
    arduinoData.trim();
    break;  // esco appena leggo una linea
  }
}
if (arduinoData.length() > 0) {
    Serial.println(arduinoData);
    bot.sendMessage(chat_id, "Arduino: " + arduinoData, "");
  } else {
    bot.sendMessage(chat_id, "⚠️ Nessuna risposta da Arduino.", "");
  }
}
 
else {
      bot.sendMessage(chat_id, "Unknown command. Send /start to see options.", "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX, TX); // deve essere 9600!

  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, HIGH);
  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(100);
}

void loop() {
  while (int numMessages = getNumMessages()) {
    handleNewMessages(numMessages);
  }
  delay(1000);
}
