# Applicazione UDP client/server, dove il server è una calcolatrice remota che risponde alle richieste del client


#### Applicazione UDP client/server, dove il server è una calcolatrice remota che risponde alle richieste del client

&nbsp;&nbsp;

#### Autori

- Amato Natale
- de Gioia Francesco

&nbsp;&nbsp;

#### Guida alla configurazione

##### Lato server

Avviare il file Server.exe

Una volta avviato, il server chiederà l'inserimento dell'indirizzo o del dns e il numero di porta, rispettando il seguente formato: `localhost:60000`.

Terminata la configurazione, il server rimarrà in attesa di un client.

Ricevuti i dati dal client, il server visualizza sullo std output un messaggio contenente sia il *nome* sia l’*indirizzo* dell’host del client e l'operazione da eseguire (Esempio: `Richiesta operazione '+ 23 45' dal client localhost, ip 127.0.0.1`).



> Nota bene:
>
> Il server interagisce anche da remoto, pertanto se al client inserirete il DNS del server, la connessione avverrà con successo, occorre però come primo passaggio impostare il Port forwarding affinché un computer remoto possa collegarsi ad un computer o ad un servizio specifico all'interno di una LAN.

&nbsp;&nbsp;

##### Lato client

Avviare il file Client.exe

Il client è avviato passando come parametri di input il *nome* e il numero di porta del server UDP da contattare, rispettando il seguente formato: `localhost:60000`.

&nbsp;&nbsp;

#### Funzionamento

Stabilita la connessione, il client legge l'operazione da compiere (usando i caratteri `+`, `x`, `-`, `/`, rispettivamente per *Addizione*, *Moltiplicazione*, *Sottrazione* e *Divisione*) e due numeri interi dallo standard input (es. `+ 23 45`) ed invia al server quanto letto da tastiera.



> Nota bene:
>
> - La lettura da riga di comando è effettuata in una singola lettura (ossia `+ 23 45[invio]`, non `+[invio]23[invio]45[invio]`).



Il server legge quanto inviato dal client, esegue l'operazione richiesta e visualizza sullo std output un messaggio contenente sia il *nome* sia l’*indirizzo* dell’host del client e l'operazione da eseguire (Esempio: `Richiesta operazione '+ 23 45' dal client localhost, ip 127.0.0.1`).

Il client legge la risposta inviata dal server e visualizza la risposta sullo std output (Esempio: `Ricevuto risultato dal server localhost, ip 127.0.0.1: 23 + 45 = 68`).

Successivamente, il client potrà leggere dallo standard input la successiva operazione da compiere o se invece di un'operazione si è scelto il simbolo di `=`, il client chiude la connessione con il server e termina qui il suo processo.

Il server rimarrà in ascolto e non terminerà mai il suo processo.
