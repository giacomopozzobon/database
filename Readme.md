# Database Lightweight Engine

Un database leggero scritto in C, con un'architettura minimale che permette di definire, salvare e interrogare dati senza l'uso di un DBMS tradizionale.

## 🚀 Funzionalità principali
- **Definizione dello schema**: permette di creare tabelle con colonne di diversi tipi.
- **Persistenza dello schema**: i dati vengono salvati su file e caricati all'avvio.
- **Inserimento dati**: è possibile inserire record nelle tabelle definite.
- **Lettura dati**: supporta query semplici per ottenere i dati salvati.
- **Gestione dinamica della memoria**: previene perdite di memoria e garantisce stabilità.

## 🛠️ Architettura
Il programma è suddiviso in più componenti:
- **Gestione dello schema**: Definisce e memorizza le tabelle con le loro colonne.
- **Parser dei comandi**: Interpreta i comandi dell'utente e li traduce in operazioni sul database.
- **Sistema di validazione**: Controlla che i dati inseriti rispettino lo schema definito.
- **Motore di esecuzione**: Si occupa dell'effettiva scrittura e lettura dei dati.

### 📁 Struttura del codice
```
- main.c                 # Entry point del programma
- config.h               # Definizione delle Struct Principali e variabili di sistema
/src
  |- parser.c            # Parsing dei comandi
  |- schema.c            # Gestione dello schema
  |- utils.c             # Funzioni di supporto
  /commands
    |- define.c          # Comando per aggiungere una tabella allo schema
    |- create.c          # Comando per creare un record di una tabella
    |- read.c            # Comando per leggere il contenuto di una tabella
```

## 🏗️ Come funziona
### 1️⃣ Definizione di una tabella
Esempio di comando per definire una tabella `Gatto` con due colonne:
```
DEFINE Gatto nome:char eta:int
```

### 2️⃣ Inserimento di un record
Per aggiungere un record nella tabella `Gatto`:
```
CREATE Gatto nome:Micio eta:5
```

### 3️⃣ Lettura dei dati
Per leggere tutti i dati della tabella `Gatto`:
```
READ Gatto
```

## 💡 Ambizione del progetto
Questo progetto nasce come esercizio di programmazione a basso livello, con l'obiettivo di comprendere il funzionamento interno di un database.

Le ambizioni future includono:
- **Supporto a query più complesse** (filtraggio, ordinamento, aggregazioni).
- **Miglior gestione della concorrenza** per supportare accessi simultanei.
- **Ottimizzazione dello storage** per rendere la persistenza più efficiente.

## 📌 Conclusione
Questo database minimalista è un'ottima base per esplorare il funzionamento interno di un motore di archiviazione dati. Se hai suggerimenti o vuoi contribuire, sentiti libero di farlo! 🚀

