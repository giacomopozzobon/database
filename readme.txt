L'utente può:
- Definire un nuovo oggetto da salvare
- Rimuovere un oggetto dallo schema
- Ottenere informazioni su tutti gli oggetti che ha già definito
- Creare un nuovo oggetto e salvarlo
- Ottenere informazioni su tutti gli oggetti di una classe che ha già salvato
- Aggiornare un oggetto già salvato
- Eliminare un oggetto già salvato
- Cercare un oggetto già salvato in base a un attributo qualsiasi





TO-DO:

- EXECUTE CREATE:
  - se la tabella non esiste la creo
  - ogni volta che creo un nuovo record, come primo attributo ci deve essere un ID incrementale e che non può essere modificato da nessuno
  - ogni volta che creo un nuovo record, voglio che venga salvato il timestamp della sua creazione come campo :created_at. Anche questo non può mai essere modificato
  - ogni volta che creo un nuovo record, voglio che venga creato anche un campo :update_at vuoto che contenga i timestamp di quando lo modifico.


- Ogni volta che aggiorno un record, voglio che venga salvato anche il timestamp della sua modifica sotto il campo :updated_at automaticamente. Questo campo non può essere modificato da nessuno.

- Se rimuovo una tabella dallo schema devo chiedere anche se eliminare la tabella con tutti i dati relativi.
- Aggiungere un comando che mi stampi come devo usare uno specifico comando: ad esempio INFO CREATE

- APPUNTO PER DOPO: All'avvio del programma potrei provare a creare una struct per ogni tabella esistente pescando i dati dallo schema.
    - In realtà forse non serve perchè avendo lo Schema come variabile globale, in ogni momento posso accedere ai miei DEFINE e quindi creare una struct ad HOC