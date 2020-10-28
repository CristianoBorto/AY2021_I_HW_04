/* 
====================================================================================================================================
                                                    Commenti sul codice
Il codice è composto dai seguenti file:
- MyFunction.h e MyFunction.c in cui, rispettivamente, vengono dichiarate e sviluppate le varie funzioni presenti nel codice che 
raggruppano varie linee di codice; inizialmente vengono definiti utili parametri per rendere più chiaro il codice.
- MyISR.h e MyISR.c: nell'header vengono dichiarati i vari flag utilizzati, alcune importanti variabili e le due interrupt service 
routines, mentre nel file C vengono sviluppate le due ISR (una triggerata dall'arrivo di un byte dall'UART e l'altra dal Timer):
    1) ISR UART: se c'è un nuovo byte in arrivo viene messo a 1 il flag "Char_arrived" per effettuare la lettura nel main.
    2) ISR Timer: se il Timer è acceso ed è in atto il campionamento viene letto il valore dall'ADC del canale selezionato, si 
    aumenta il contatore "count" e viene sommato il valore per poi fare la media nel main una volta che il contatore ha raggiunto il 
    valore 5. 
- main.c in cui è sviluppato il codice principale dopo una fase iniziale di inizializzaione composta da tre funzioni:
    1) Flag_settings: vengono settate a 0 tutti i flag e le variabili globali; inoltre viene stabilito l'header e la tail del 
    pacchetto.
    2) Start_interrupts: vengono abilitate tutte le interrupt e vengono indirizzate alle custom ISR.
    3) Start_peripherls: viene dato lo start al PWM (settato in modo tale che il LED inizialmente sia spento), all'ADC e all'UART

Componenti:
- LED on board utilizzato per indicare se il dispositivo sta campionando i segnali e sta comunicando con l'esterno.
- LED esterno pilotato da un PWM con una frequenza abbastanza elevata da evitare lampeggii percettibili; andando a variare il valore
di compare in base al valore campionato dal potenziometro (è stata utilizzata una retta di calibrazione imponendo il passaggio per 
l'origine e stimando il coefficiente angolare --> a_potenz = 255/65535 = 3.89e-3), sarà possibile modificare il DC del PWM andando a 
variare l'intensità del LED
- UART come protocollo di comunicazione per ricevere i comandi di avvio/stop e per inviare i risultati (due variabili a 16 bit) del 
campionamento mediante una specifica codifica --> pacchetto composto da 6 Byte composto così:
    a) header: A0
    b) MSB del primo segnale (fotoresistore)
    c) LSB del primo segnale (fotoresistore)
    d) MSB del secondo segnale (potenziometro)
    e) LSB del secondo segnale (potenziometro)
    f) Tail: C0
- ADC con un AMux per campionare i segnali provenienti dalla fotoresistenza e dal potenziometro
- Timer utilizzato per andare a leggere con una frequenza precisa i valori all'uscita dell'ADC

Il problema è stato affrontato scorporandolo in due macro-fasi:
A) PRE-CAMPIONAMENTO: in questa fase il LED e il Timer sono spenti e il dispositivo è in attesa di un segnale proveniente da remoto; 
sono state individuate 4 possibili risposte:
    1) Arrivo del carattere B: il dispositivo si accende e inizia a campionare i segnali provenienti dai due sensori (Fase 2);
    2) Arrivo del carattere O: considerando un'applicazione reale, ho introdotto questa funzionalità che prevede il semplice utilizzo
    del sistema come lampada "normale" senza la sensibilità alla luce in modo tale che possa essere utilizzata in qualsiasi momento e
    in qualsiasi condizione di luce; in questa situazione è possibile modulare l'intensità della luce e il dispositivo invia comunque
    i dati del campionamento utilizzando lo stesso formato del pacchetto imponendo a 0 i valori provenienti dalla fotoresistenza ma 
    avendo utili informazioni sul comportamento del potenziometro per fini diagnostici. 
    3) Arrivo del carattere S: vengono spenti il Timer (quindi viene fermato il campionamento), il LED e l'on-board LED, vengono 
    azzerate le varie variabili.
    4) Default: segnalazione di errore con una piccola stringa e stop al campionamento se era in atto.
B) CAMPIONAMENTO: il timer viene acceso, viene acceso l'on-board LED e si comincia a leggere i valori in uscita dall'ADC in base alla
working mode scelta; per avere una misura più stabile viene effettuata la media su 5 campioni acquisiti ogni 20 ms per ottenere un 
dato finale mediato ("mean_value") ogni 100 ms. Due flag ("Always_on" e "Dark_mode") permettono di gestire le due diverse situazioni 
presentate sopra (arrivo 'B' e arrivo 'O') e l'eventuale switch tra i due segnali. Analizziamo le due situazioni:
    1) Always_on mode: in questa modalità la fotoresistenza non viene mai campionata e perciò il LED è sempre acceso e modulabile con 
    il potenziometro. Per fare questo viene utilizzato il flag "Always_on" che permette di non entrare nella parte di codice che
    effettua il campionamento della fotoresistenza non aggiornando il flag "Dark_mode". In questa modalità viene mandato il pacchetto
    imponendo a 0 la parte dedicata al segnale della fotoresistenza.
    2) Light_sensible mode: il campionamento comincia la prima volta andando ad analizzare il segnale della fotoresistenza:
        I) se il segnale è minore di una determinata soglia viene cambiato il canale dell'AMux e viene fatto il campionamento del
        potenziometro, viene salvato il valore dentro il pacchetto e viene modificato il flag "Dark_mode".
        II) se il segnale è maggiore non viene cambiato canale e viene campionata ancora la fotoresistenza e viene inviato il pac-
        chetto con i dati del segnale e imponendo a 0 il segnale del potenziometro. Non potendo modulare l'intensità essendo il LED
        spento, ho ritenuto inutile campionare anche il potenziometro soprattutto perchè, in questo modo, il dispositivo campiona al
        doppio della velocità, rispetto a quando vengono campionati entrambi i canali, la fotoreistenza rendendo il sistema molto più 
        reattivo a rapidi cambiamenti di luce; lo stesso ragionamento è stato applicato alla condizione precedente in cui è campio-
        nato solamente il potenziometro. In questo modo è possibile campionare i segnali sia singolarmente che insieme.
    Successivamente viene campionato il potenziometro, viene modulato il LED andando a cambiare il compare (sfruttando la retta di 
    calibrazione), viene salvato il valore nel pacchetto e viene inviato. Se si è in questa working mode si ritorna a campionare la 
    fotoresistenza (dunque vi è lo shift tra un canale e l'altro ogni 0,1s campionando 5 valori ogni 20 ms per mediarli) per valutare
    se le condizioni di luce sono cambiate. 
    
Il pacchetto viene inviato quando viene messo a 1 il flag "PacketReadyFlag".   
====================================================================================================================================
*/ 

