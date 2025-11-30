# Irrigator-system
🌱 ESP32 Smart Irrigation System + Telegram Bot

Controllo remoto dell’irrigazione e lettura sensori tramite Telegram

📌 Descrizione

Questo progetto permette di controllare un impianto di irrigazione tramite ESP32 e un Telegram Bot, con la possibilità di:

avviare l’irrigazione per un numero di secondi desiderato

richiedere dati in tempo reale dall’Arduino (tramite Serial2)

leggere sensori istantaneamente

monitorare la connessione e garantire stabilità (watchdog Telegram + WiFi)

evitare blocchi, duplicazioni di messaggi e problemi TLS

Il sistema usa un relay per controllare l’elettrovalvola e un Arduino secondario per la lettura sensori.
