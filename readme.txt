L'utente può:
- Definire un nuovo oggetto da salvare
- Rimuovere un oggetto dallo schema
- Ottenere informazioni su tutti gli oggetti che ha già definito
- Creare un nuovo oggetto e salvarlo
- Ottenere informazioni su tutti gli oggetti di una classe che ha già salvato
- Aggiornare un oggetto già salvato
- Eliminare un oggetto già salvato
- Cercare un oggetto già salvato in base a un attributo qualsiasi





PROBLEMA DA RISOLVERE
- Il timestamp created_at non viene creato correttamente in EXECUTE CREATE

- Sembra che la prima volta che avvio il programma e creo lo schema da capo e creo dei record va tutto bene
  Quando chiudo e riapro il programma, posso leggere lo schema e le tabelle, ma se:
  se provo a creare un record di una tabella SEGFAULT



TO-DO:

- Devo ricordarmi di avvisare l'utente e aggiungere il controllo che non serve mettere ID, CreatedAt e UpdatedAt come campi definiti in una tabella
- Devo ricordarmi di aggiungere una validazione ai token di CREATE, UPDATE in cui vado a provare a convertire il valore che sto inserendo nel tipo effettivo

- Ogni volta che aggiorno un record, voglio che venga salvato anche il timestamp della sua modifica sotto il campo :updated_at automaticamente. Questo campo non può essere modificato da nessuno.

- Se rimuovo una tabella dallo schema devo chiedere anche se eliminare la tabella con tutti i dati relativi.
- Aggiungere un comando che mi stampi come devo usare uno specifico comando: ad esempio INFO CREATE

- APPUNTO PER DOPO: All'avvio del programma potrei provare a creare una struct per ogni tabella esistente pescando i dati dallo schema.
    - In realtà forse non serve perchè avendo lo Schema come variabile globale, in ogni momento posso accedere ai miei DEFINE e quindi creare una struct ad HOC