/* 

  Benvenuto, questo è il mio primo codice in C.

  * In questo codice ho creato un semplice database in cui è possibile aggiungere utenti e visualizzarli.
  * Il database è un file di testo chiamato "database.txt" che si trova nella stessa cartella del programma.
  * Ogni riga del file rappresenta un utente e contiene l'ID, il nome e l'età dell'utente separati da uno spazio.
  * Il programma permette di aggiungere utenti al database e di visualizzarli.
  

  Disclaimer:

  In questo codice ci saranno parecchi commenti perchè mi serviranno per tenere traccia di quello che faccio e di imparare meglio.

*/

#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fprintf, fscanf
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <stdbool.h>                // Definisce il tipo di dato bool e le costanti true e false

#define DB_FILE "database.txt"      // Definisco il nome del file del database, in questo caso si trova nella stessa cartella del programma

// Definisco una struttura per rappresentare un utente
typedef struct {
  int id;
  char nome[50];
  int eta;
} Utente;


/* Funzione per verificare se un utente esiste nel database
   La funzione legge il file del database riga per riga e verifica se l'ID dell'utente da cercare è presente.
   Se l'utente è stato trovato, la funzione restituisce true, altrimenti restituisce false.
*/

bool utente_esiste(int id_da_cercare) {
  FILE *file = fopen(DB_FILE, "r");   // Apro il file in modalità lettura, perchè devo solo leggere i dati.
  if (file == NULL) {
    return false;                     // Se il file non esiste, restituisco false
  }

  Utente utente;

  while(fscanf(file, "%d %s %d", &utente.id, utente.nome, &utente.eta) != EOF) {
    if (utente.id == id_da_cercare) {
      fclose(file);                   // Chiudo il file
      return true;                    // Se l'utente è stato trovato, restituisco true
    }
  }

  fclose(file);                       // Chiudo il file
  return false;                       // Se l'utente non è stato trovato, restituisco false
}


/* Funzione per ottenere il prossimo ID da utilizzare per un nuovo utente
   La funzione legge il file del database e restituisce l'ID più grande trovato + 1
   Se il file non esiste, restituisce 1 come primo ID da utilizzare
*/

int prossimo_id() {
  FILE *file = fopen(DB_FILE, "r");   // Apro il file in modalità lettura, perchè devo solo leggere i dati.
  if (file == NULL) {
    return 1;                         // Se il file non esiste, restituisco 1 come primo ID da utilizzare
  }

  Utente utente;
  while(fscanf(file, "%d %s %d", &utente.id, utente.nome, &utente.eta) != EOF) {
    // Voglio solo scansionare il file fino alla fine per trovare l'ID più grande.
    // L'idea è quella di rendere l'ID univoco per ogni utente, e inserito e gestito in modo automatico.
    // Non voglio dover pensare a inserire l'ID manualmente.

    // Facendo cosi dovrò comunque assicurarmi che l'ID sia effettivamente univoco e che non possa essere modificato o inserito a mano.
  }

  fclose(file);                       // Chiudo il file
  return utente.id + 1;               // Restituisco l'ID più grande trovato + 1
}

/* Funzione per aggiungere un utente al database
 * Parametri:
 * - utente: l'utente da aggiungere
*/

void aggiungi_utente(Utente utente) {
  utente.id = prossimo_id();          // Ottengo il prossimo ID da utilizzare per l'utente

  FILE *file = fopen(DB_FILE, "a");   // Apro il file in modalità append, in questo modo i dati vengono aggiunti alla fine del file.
                                      // Attenzione qui, se il file non esiste, viene creato automaticamente.
  if (file == NULL) {
    printf("Errore nell'apertura del file\n");
    return;
  }

  fprintf(file, "%d %s %d\n", utente.id, utente.nome, utente.eta);    // Scrivo i dati dell'utente nel file. %d per interi, %s per stringhe
  fclose(file);                                                       // Chiudo il file

  printf("Utente aggiunto con successo!\n");
}

/* Funzione per modificare un utente nel database
   La tecnica utilizzata qui per modificare un utente è quella di copiare tutti gli utenti tranne quello da modificare in un file temporaneo.
   Solo se l'utente da modificare è stato trovato, il file del database viene rimosso e il file temporaneo viene rinominato con il nome del file del database.

   Per mantenere l'ID Univoco e automatico, l'ID non può essere modificato manualmente.
*/

void modifica_utente(int id_da_modificare, char nuovo_nome[], int nuova_eta) {
  FILE *file = fopen(DB_FILE, "r");                                   // Apro il file in modalità lettura, quindi posso solo leggere i dati.  
  FILE *temp = fopen("temp.txt", "w");                                // Apro un file temporaneo in cui copierò tutti gli utenti tranne quello da modificare

  if (file == NULL || temp == NULL) {
    printf("Errore nell'apertura del file\n");
    return;
  }

  Utente utente;
  int trovato = 0;                                                   // Variabile per indicare se l'utente da modificare è stato trovato

  while(fscanf(file, "%d %s %d", &utente.id, utente.nome, &utente.eta) != EOF) {  // Leggo i dati di ciascun utente finchè non arrivo alla fine del file. Uso scanf perchè i dati sono separati da uno spazio.
    if (utente.id == id_da_modificare) {                                          // Se l'utente da modificare è stato trovato
      trovato = 1;                                                                // Imposto la variabile trovato a 1
      fprintf(temp, "%d %s %d\n", utente.id, nuovo_nome, nuova_eta);              // Altrimenti copio l'utente nel file temporaneo
    } else {
      fprintf(temp, "%d %s %d\n", utente.id, utente.nome, utente.eta);            // Altrimenti copio l'utente nel file temporaneo
    }
  }

  fclose(file);                                                                   // Chiudo il file
  fclose(temp);                                                                   // Chiudo il file temporaneo

  if (!trovato) {
    printf("Utente con ID %d non trovato!\n", id_da_modificare);
    remove("temp.txt"); // Cancella il file temporaneo se l'ID non esiste
  } else {
    remove(DB_FILE);
    rename("temp.txt", DB_FILE);
    printf("Utente con ID %d modificato con successo!\n", id_da_modificare);
  }
}


/* Funzione per visualizzare la lista degli utenti presenti nel database
   La funzione legge il file riga per riga e stampa i dati degli utenti
*/

void mostra_utenti() {
  FILE *file = fopen(DB_FILE, "r");                                   // Apro il file in modalità lettura, quindi posso solo leggere i dati.  
  if (file == NULL) {
    printf("Errore nell'apertura del file\n");
    return;
  }

  Utente utente;                                                      // Creo una variabile di tipo Utente per leggere i dati di ciascun utente   
  printf("Lista utenti:\n");

  while (fscanf(file, "%d %s %d", &utente.id, utente.nome, &utente.eta) != EOF) {             // Leggo i dati di ciascun utente finchè non arrivo alla fine del file. Uso scanf perchè i dati sono separati da uno spazio.
    printf("ID: %d, Nome: %s, Età: %d\n", utente.id, utente.nome, utente.eta);                // Stampo i dati dell'utente
  }

  fclose(file);                                                                               // Chiudo il file          
}


/* Funzione per rimuovere un utente dal database
   La tecnica utilizzata qui per rimuovere un utente è quella di copiare tutti gli utenti tranne quello da rimuovere in un file temporaneo
   Solo se l'utente da rimuovere è stato trovato, il file del database viene rimosso e il file temporaneo viene rinominato con il nome del file del database.
*/

void rimuovi_utente(int id) {
  FILE *file = fopen(DB_FILE, "r");                                                           // Apro il file in modalità lettura, quindi posso solo leggere i dati.  
  FILE *temp = fopen("temp.txt", "w");                                                        // Apro un file temporaneo in cui copierò tutti gli utenti tranne quello da rimuovere

  if (file == NULL || temp == NULL) {
    printf("Errore nell'apertura del file\n");
    return;
  }

  Utente utente; 
  int trovato = 0;                                                                           // Variabile per indicare se l'utente da rimuovere è stato trovato

  while (fscanf(file, "%d %s %d", &utente.id, utente.nome, &utente.eta) != EOF) {            // Leggo i dati di ciascun utente finchè non arrivo alla fine del file. Uso scanf perchè i dati sono separati da uno spazio.
    if (utente.id == id) {                                                                   // Se l'utente da rimuovere è stato trovato
      trovato = 1;                                                                           // Imposto la variabile trovato a 1
    } else {
      fprintf(temp, "%d %s %d\n", utente.id, utente.nome, utente.eta);                        // Altrimenti copio l'utente nel file temporaneo
    }
  }  

  fclose(file);                                                                               // Chiudo il file
  fclose(temp);                                                                               // Chiudo il file temporaneo

  if (trovato) {
    remove(DB_FILE);                                                                         // Rimuovo il file del database
    rename("temp.txt", DB_FILE);                                                             // Rinomino il file temporaneo con il nome del file del database
    printf("Utente rimosso con successo!\n");
  } else {
    remove("temp.txt");                                                                      // Rimuovo il file temporaneo
    printf("Utente non trovato\n");
  }
}


/* Funzione principale del programma
   Il programma permette di aggiungere utenti al database e di visualizzarli
   Seleziona l'operazione da eseguire tramite un menu.
*/
int main () {
  Utente utente;
  int scelta;

  do {
    printf("1. Aggiungi utente\n");
    printf("2. Mostra utenti\n");
    printf("3. Elimina utente\n");
    printf("4. Modifica utente\n");
    printf("5. Esci\n");
    printf("Scelta: ");
    scanf("%d", &scelta);

    switch (scelta) {
      case 1:
        printf("Inserisci il nome dell'utente: ");
        scanf("%s", utente.nome);
        printf("Inserisci l'età dell'utente: ");
        scanf("%d", &utente.eta);
        aggiungi_utente(utente);
        break;
      case 2:
        mostra_utenti();
        break;
      case 3:
        printf("Inserisci l'ID dell'utente da eliminare: ");
        scanf("%d", &utente.id);
        
        if (!utente_esiste(utente.id)) {
          printf("Utente con ID %d non trovato!\n", utente.id);
        } else {
          rimuovi_utente(utente.id);
        }
        break;
      case 4: {
        int id_da_modificare, nuova_eta;
        char nuovo_nome[50];

        printf("Inserisci l'ID dell'utente da modificare: ");
        scanf("%d", &id_da_modificare);

        if (!utente_esiste(utente.id)) {
          printf("Utente con ID %d non trovato!\n", utente.id);
        } else {
          printf("Nuovo Nome: ");
          scanf("%s", nuovo_nome);
          printf("Nuova Età: ");
          scanf("%d", &nuova_eta);

          modifica_utente(id_da_modificare, nuovo_nome, nuova_eta);
        }
        break;
      }
      case 5:
        printf("Arrivederci!\n");
        break;
      default:
        printf("Scelta non valida\n");
    }
  } while (scelta != 5);

  return 0;       // Ritorno 0 per indicare che il programma è terminato correttamente
}