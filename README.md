# Irrigator-system
🌱 **ESP32 Smart Irrigation System + Telegram Bot**

Controllo remoto dell’irrigazione e lettura sensori tramite chat Telegram

📌 Descrizione

Questo progetto permette di controllare tramite ESP32 e un Bot Telegram, un impianto di irrigazione DIY, con la possibilità di:

- avviare l’irrigazione impostando il numero di secondi desiderato
- richiedere dati in tempo reale dall’Arduino (tramite canale Serial2), leggendo i valori di umidità e temperatura dell'aria e del suolo.


Il sistema utilizza un relè a 5V per controllare la pompa d'acqua, una scheda Arduino Uno Mini per la lettura dei sensori e la stampa su un display LCD dei valori letti e una scheda "IdeaSpark ESP32" per la comunicazione con i server Telegram.

**Dettaglio del progetto**
Alimentazioni
Il progetto permette all'utente di controllare e gestire l'azionamento di una pompa d'acqua per l'irrigazione delle piante.

La pompa elettrica, controllata da un modulo relé, è alimentata a 5V grazie ad un alimentatore apposito.

Il modulo relè è alimentato e pilotato dalla scheda di sviluppo "IdeaSpark ESP32" e alimentato a 3.3V da un modulo Power Supply.  

La scheda Arduino è alimentata a 5V grazie ad un alimentatore che converte i 220V (AC, RMS) della rete elettrica in 5V DC.

Funzionamento

L'utente può controllare e monitorare l'irrigazione e leggere i dati dei sensori tramite un semplice messaggio nella chat in cui è presente il bot. Quando riceve un messaggio, l'ESP32 esegue il codice contenente le istruzioni per quel particolare comando. 
I comandi ammessi sono:
- "/water X" che avvia l'irrigazione per i prossimi X secondi. L'utente riceverà un messaggio di feedback quando avviene l'inizio e la fine dell'irrigazione.
- "/getdata" che avvia la lettura dei sensori di umidità del terreno.
- "/realtime" che avvia la lettura del sensore DHT che misura temperatura e umidità dell'aria.

Nel caso di richiesta di dati dai sensori DHT dell'aria o dei sensori di umidità del terreno (soil moisture sensors capacitivi), l'ESP32 comunica, tramite un protocollo di comunicazione seriale, con il microcontrollore montato sulla scheda Arduino, il quale si occupa di leggere i dati dei sensori (collegati alla scheda Arduino) e li inoltra al modulo ESP, che a sua volta li manderà ai server di Telegram, dopo aver stabilito una connessione WiFi. 

Come funzione aggiuntiva, l'Arduino mostra su un display LCD i valori correnti di temperatura e umidità dell'aria (DHT). 
Il modulo Arduino riesce a controllare e ad alimentare fino a tre sensori di umidità capacitivi, il sensore DHT e lo schermo LCD.

**Componenti utilizzati**
- 3 Capacitive soil moisture sensor
- Sensore temperatura DHT11
- Relé a 5V
- 3 Alimentatori a 5V
- schermo LCD
- Potenziometro (contrasto schermo LCD)
- scheda Arduino UNO Mini
- scheda IdeaSpark ESP32

  
 **Consigli**
  Si consiglia di rivestire bene la parte elettronica del sensore Capacitivo in quanto potrebbe arruginirsi restando a contatto con l'acqua.

  Per poter riutilizzare il codice è necessario scrivere un file "Login.h" da inserire nella stessa cartella del progetto, dove scrivere SSID e PASSWORD della propria rete fissa e il token e il chat-id del proprio Bot telegram. Per crearlo io ho usato "BotFather".

