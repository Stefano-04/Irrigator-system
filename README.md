# Irrigator-system
🌱 ESP32 Smart Irrigation System + Telegram Bot

Controllo remoto dell’irrigazione e lettura sensori tramite Telegram

📌 Descrizione

Questo progetto permette di controllare un impianto di irrigazione tramite ESP32 e un Telegram Bot, con la possibilità di:

-avviare l’irrigazione per un numero di secondi desiderato
-richiedere dati in tempo reale dall’Arduino (tramite canale Serial2)
-leggere istantaneamente sensori di temperatura e umidità 

Il sistema usa un relè a 5V per controllare la pompa d'acqua e un microcontrollore Arduino Uno Mini per la lettura dei sensori.

Dettaglio del progetto

Il progetto prevede di poter controllare e gestire l'azionamento di una pompa d'acqua per l'irrigazione delle piante.

La pompa elettrica, grazie a un cavo USB, controllato da un modulo relé a 5V con isolamento galvanico, è alimentata da un alimentatore per PC che si interfaccia con la rete elettrica.

Il modulo relè è pilotato dal GPIO4 della scheda di sviluppo "IdeaSpark ESP32" e alimentato da un modulo Power Supply con uscite a 5V e 3.3 V.
L'ESP32, invece, è alimentato tramite un regolatore di tensione a 3.3 V e controlla un bot Telegram (lettura e scrittura messaggi). 
L'utente può controllare e monitorare l'irrigazione e leggere i dati dei sensori tramite il bot. Quando riceve un messaggio, l'ESP32 lo riceve ed esegue le istruzioni in base al comando lanciato. Nel caso di richiesta di dati dai sensori DHT dell'aria o dei sensori di umidità del terreno, l'ESP32 comunica tramite un canale di comunicazione seriale a un microcontrollore Arduino Uno Mini, che si occupa di leggere i dati dei sensori e inoltrarli al modulo ESP. L'Arduino, inoltre, si occupa di mostrare su un display LCD i valori correnti di temperatura e umidità dell'aria. Il modulo Arduino riesce a controllare e ad alimentare fino a tre sensori di umidità capacitivi, il sensore DHT e lo schermo LCD.
