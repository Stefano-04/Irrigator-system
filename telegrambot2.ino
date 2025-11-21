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
  "/start - restart the bot \n"
  "/stop - stop bot actions\n"
  "/water X - start watering for X seconds\n"
  "/realtime - display real-time measurements\n"
  "/getdata - get instant data from Arduino sensors";

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Timers
unsigned long lastBotCheck    = 0;
unsigned long lastWiFiCheck   = 0;
unsigned long lastBotResponse = 0;

const unsigned long botInterval       = 5000;     // Polling bot
const unsigned long wifiCheckInterval = 30000;    // Check WiFi
const unsigned long botTimeout        = 20000;    // 20s senza risposta → reset

// Watering non bloccante
bool watering = false;
unsigned long wateringEnd = 0;


// ------------------------------------------------------
// 🔧 FUNZIONE: Controllo Internet
// ------------------------------------------------------
bool internetOK() {
  WiFiClient testClient;
  testClient.setTimeout(2000);
  bool ok = testClient.connect("api.telegram.org", 443);
  testClient.stop();
  return ok;
}


// ------------------------------------------------------
// 🔁 Reset sicuro Telegram Client
// ------------------------------------------------------
void safeResetClient() {
  Serial.println("🔁 Resetting Telegram client...");

  client = WiFiClientSecure();
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  client.setTimeout(3000);

  bot = UniversalTelegramBot(BOT_TOKEN, client);
  bot.last_message_received = 0;

  // Pulisce vecchi messaggi
  while (bot.getUpdates(bot.last_message_received + 1) > 0) {}
}


// ------------------------------------------------------
// 💧 Attiva irrigazione NON bloccante
// ------------------------------------------------------
void startWatering(int seconds) {
  if (watering) return;

  watering = true;
  wateringEnd = millis() + (unsigned long)seconds * 1000;

  digitalWrite(RELE, LOW);
  bot.sendMessage(CHAT_ID, "💧 Started watering for " + String(seconds) + " seconds.", "");
}


// ------------------------------------------------------
// 💧 Gestione watering nel loop
// ------------------------------------------------------
void handleWatering() {
  if (watering && millis() > wateringEnd) {
    digitalWrite(RELE, HIGH);
    watering = false;
    bot.sendMessage(CHAT_ID, "✅ Done watering!", "");
  }
}


// ------------------------------------------------------
// 📩 Ricezione e gestione comandi Telegram
// ------------------------------------------------------
void handleNewMessages(int numMessages) {
  for (int i = 0; i < numMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) continue;

    String text = bot.messages[i].text;
    Serial.println("[📩] Msg: " + text);

    if (text == "/start") {
      bot.sendMessage(chat_id, info, "");
    }

    else if (text.startsWith("/water")) {
      int seconds = 0;
      if (sscanf(text.c_str(), "/water %d", &seconds) == 1 && seconds > 0) {
        startWatering(seconds);
      } else {
        bot.sendMessage(chat_id, "⚠️ Use: /water <seconds> (example: /water 10)", "");
      }
    }

    else if (text == "/realtime") {
      bot.sendMessage(chat_id, "📡 Requesting real-time measurements...", "");
      Serial2.println("realtime");

      String arduinoData = "";
      unsigned long start = millis();
      while (millis() - start < 5000) {
        if (Serial2.available()) {
          arduinoData = Serial2.readStringUntil('\n');
          arduinoData.trim();
          break;
        }
      }

      if (arduinoData.length() > 0) {
        bot.sendMessage(chat_id, "📊 Arduino: " + arduinoData, "");
      } else {
        bot.sendMessage(chat_id, "⚠️ No response from Arduino.", "");
      }
    }

    else if (text == "/getdata") {
      bot.sendMessage(chat_id, "📡 Requesting sensor data...", "");
      Serial2.println("getdata");

      String arduinoData = "";
      unsigned long start = millis();
      while (millis() - start < 5000) {
        if (Serial2.available()) {
          arduinoData = Serial2.readStringUntil('\n');
          arduinoData.trim();
          break;
        }
      }

      if (arduinoData.length() > 0) {
        bot.sendMessage(chat_id, "📈 Arduino: " + arduinoData, "");
      } else {
        bot.sendMessage(chat_id, "⚠️ No response from Arduino.", "");
      }
    }

    else {
      bot.sendMessage(chat_id, "❓ Unknown command. Use /start for help.", "");
    }
  }
}


// ------------------------------------------------------
// ⚙️ SETUP
// ------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX, TX);

  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, HIGH);

  WiFi.begin(SSID, PASSWORD);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }

  Serial.println("\n✅ WiFi connected");

  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  client.setTimeout(3000);

  // Pulisce messaggi vecchi
  while (bot.getUpdates(bot.last_message_received + 1) > 0) {
    delay(100);
  }

  lastBotResponse = millis();
}


// ------------------------------------------------------
// 🔁 LOOP SUPER STABILE
// ------------------------------------------------------
void loop() {

  // 💧 Gestione irrigazione non bloccante
  handleWatering();


  // 🌐 Controllo WiFi periodico
  if (millis() - lastWiFiCheck > wifiCheckInterval) {
    lastWiFiCheck = millis();

    if (!internetOK()) {
      Serial.println("⚠️ Internet DOWN → reconnecting WiFi");
      WiFi.disconnect(true);
      delay(200);
      WiFi.begin(SSID, PASSWORD);
      return;
    }
  }


  // 🤖 Poll Telegram ogni 5s
  if (millis() - lastBotCheck > botInterval) {

    int numMessages = bot.getUpdates(bot.last_message_received + 1);

    if (numMessages > 0) {
      handleNewMessages(numMessages);
      lastBotResponse = millis();       // Telegram OK
    }
    else if (numMessages == -1) {
      Serial.println("⚠️ Telegram error, waiting before reset...");
    }

    lastBotCheck = millis();
  }


  // ⏱ Watchdog Telegram → reset client se nessuna risposta da 20s
  if (millis() - lastBotResponse > botTimeout) {
    safeResetClient();
    lastBotResponse = millis();
  }
}