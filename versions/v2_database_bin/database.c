/* 

  Benvenuto, questo è il mio primo codice in C.

  * Si tratta di un semplice database in cui è possibile fare operazioni con degli utenti.
  * In questa versione il database è un file binario. In questo modo i dati sono salvati in modo permanente e più veloce rispetto ad un file di testo.
  * Ogni riga del file rappresenta un utente.
  

  Disclaimer:

  In questo codice ci saranno parecchi commenti perchè mi serviranno per tenere traccia di quello che faccio e di imparare meglio.

*/

#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fread, fwrite, fseek, remove, rename
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <string.h>                 // Funzioni per la manipolazione delle stringhe: strcpy
#include <stdbool.h>                // Definisce il tipo di dato bool e le costanti true e false

#define DB_FILE "database.bin"      // Definisco il nome del file del database, in questo caso si trova nella stessa cartella del programma
#define VERSIONE 1.1                // Versione del programma

#define MAX_NOME 100                // Lunghezza massima del nome

// Definisco una struttura per rappresentare un utente
typedef struct {
  int id;
  char nome[MAX_NOME];
  int eta;
} Utente;


/* Funzione per verificare se un utente esiste nel database */
bool utente_esiste(int id_da_cercare) {
  FILE *file = fopen(DB_FILE, "rb");   // Apro il file in modalità lettura, perchè devo solo leggere i dati.
  if (file == NULL) {
    return false;                      // Se il file non esiste, restituisco false
  }

  Utente utente;
  
  while(fread(&utente, sizeof(Utente), 1, file) == 1) {           // Leggo i dati di ciascun utente.
    if (utente.id == id_da_cercare) {
      fclose(file);                   // Chiudo il file
      return true;                    // Se l'utente è stato trovato, restituisco true
    }
  }

  fclose(file);                       // Chiudo il file
  return false;                       // Se l'utente non è stato trovato, restituisco false
}


/* Funzione per aggiungere un utente al database */
void aggiungi_utente(Utente nuovo_utente) {
  FILE *file = fopen(DB_FILE, "ab+");                                 // Apro il file in modalità append binary, quindi posso scrivere i dati, se il file non esiste, viene creato automaticamente.
                                                                      // Inoltre, la modalità append binary mi permette di scrivere alla fine del file senza sovrascrivere i dati esistenti.

  if (file == NULL) {
    printf("Errore nell'apertura del file\n");
    return;
  }

  nuovo_utente.id = 1;                                                // Imposto l'ID dell'utente a 1, nel caso non ci siano utenti nel database.


  // Gestione dell'ID automatico:
  // Qui devo cercare se nel database esistono già utenti, in tal caso devo impostare l'ID dell'utente con l'ID dell'ultimo utente + 1
  Utente temp;
  fseek(file, -sizeof(Utente), SEEK_END);                             // Mi posizione sull'ultimo utente nel file
  if(fread(&temp, sizeof(Utente), 1, file) == 1) {                    // Leggo l'ultimo utente nel file. Uso sizeof per ottenere la dimensione della struttura Utente, 1 per indicare che sto leggendo un solo utente.
    nuovo_utente.id = temp.id + 1;                                    // Imposto l'ID dell'utente con l'ID dell'ultimo utente + 1
  }



  fwrite(&nuovo_utente, sizeof(Utente), 1, file);                     // Scrivo i dati dell'utente nel file binario. Uso sizeof per ottenere la dimensione della struttura Utente, 1 per indicare che sto scrivendo un solo utente.
  fclose(file);                                                       // Chiudo il file

  printf("Utente aggiunto: ID=%d, Nome=%s, Età=%d\n", nuovo_utente.id, nuovo_utente.nome, nuovo_utente.eta);
}

/* Funzione per modificare un utente nel database
   La tecnica utilizzata qui, a differenza del database testuale, è quella di sovrascrivere i dati dell'utente nel file binario.

   Per mantenere l'ID Univoco e automatico, l'ID non può essere modificato manualmente.
*/

void modifica_utente(int id_da_modificare, char nuovo_nome[], int nuova_eta) {

  // Prima di fare qualsiasi modifica, verifichiamo se l'ID esiste
  if (!utente_esiste(id_da_modificare)) {
    printf("Errore: l'utente con ID %d non esiste!\n", id_da_modificare);
    return;  // Interrompo tutto se l'utente non esiste
  }

  FILE *file = fopen(DB_FILE, "r+b");                                   // Apro il file in modalità lettura e scrittura binaria, quindi posso leggere e scrivere i dati.

  if (file == NULL) {
    printf("Errore nell'apertura del file\n");
    return;
  }

  Utente utente;

  while(fread(&utente, sizeof(Utente), 1, file) == 1) {                 // Leggo i dati di ciascun utente finchè non arrivo alla fine del file. Uso sizeof per ottenere la dimensione della struttura Utente, 1 per indicare che sto leggendo un solo utente.
    if (utente.id == id_da_modificare) {                                          // Se l'utente da modificare è stato trovato
      
      // Modifico i dati
      strcpy(utente.nome, nuovo_nome);                                            // Copio il nuovo nome nell'utente
      utente.eta = nuova_eta;                                                     // Copio la nuova età nell'utente

      // Torno indietro di una posizione per sovrascrivere i dati dell'utente
      fseek(file, -sizeof(Utente), SEEK_CUR);                                      // Mi posiziono sull'utente da modificare
      fwrite(&utente, sizeof(Utente), 1, file);                                    // Scrivo i dati dell'utente nel file binario.

      printf("Utente con ID %d modificato con successo!\n", id_da_modificare);
      break;
    }
  }

  fclose(file);                                                                     // Chiudo il file
}


/* Funzione per visualizzare la lista degli utenti presenti nel database
   La funzione legge il file riga per riga e stampa i dati degli utenti
*/

void mostra_utenti() {
  FILE *file = fopen(DB_FILE, "rb");                                   // Apro il file in modalità lettura binaria, quindi posso solo leggere i dati.  
  if (file == NULL) {
    printf("Errore nell'apertura del file\n");
    return;
  }

  Utente utente;                                                      // Creo una variabile di tipo Utente per leggere i dati di ciascun utente   
  printf("\nLista utenti:\n");

  while (fread(&utente, sizeof(Utente), 1, file) == 1) {                              // Leggo i dati di ciascun utente finchè non arrivo alla fine del file. Uso sizeof per ottenere la dimensione della struttura Utente, 1 per indicare che sto leggendo un solo utente.
    printf("ID: %d, Nome: %s, Età: %d\n", utente.id, utente.nome, utente.eta);        // Stampo i dati dell'utente
  }
  printf("\n");

  fclose(file);                                                                       // Chiudo il file          
}


/* Funzione per rimuovere un utente dal database
   La tecnica utilizzata qui per rimuovere un utente è quella di copiare tutti gli utenti tranne quello da rimuovere in un file temporaneo.
*/

void rimuovi_utente(int id_da_rimuovere) {
  
  // Prima di fare qualsiasi modifica, verifichiamo se l'ID esiste
  if (!utente_esiste(id_da_rimuovere)) {
    printf("Errore: l'utente con ID %d non esiste!\n", id_da_rimuovere);
    return;  // Interrompo tutto se l'utente non esiste
  }

  FILE *file = fopen(DB_FILE, "rb");                                                          // Apro il file in modalità lettura binaria, quindi posso solo leggere i dati.  
  FILE *temp = fopen("temp.bin", "wb");                                                       // Apro un file temporaneo in modalità scrittura binaria, quindi posso solo scrivere i dati.

  if (file == NULL) {
    printf("Errore nell'apertura del file\n");
    return;
  }

  if (temp == NULL) {
    printf("Errore nell'apertura del file temporaneo\n");
    fclose(file);                                                                             // Chiudo il file
    return;
  }

  Utente utente;

  while (fread(&utente, sizeof(Utente), 1, file) == 1) {                                      // Leggo i dati di ciascun utente finchè non arrivo alla fine del file. 
    if (utente.id != id_da_rimuovere) {                                                       // Copio tutti gli utenti tranne quello da rimuovere.
      fwrite(&utente, sizeof(Utente), 1, temp);
    }
  }  

  fclose(file);                                                                               // Chiudo il file
  fclose(temp);                                                                               // Chiudo il file temporaneo

  remove(DB_FILE);
  rename("temp.bin", DB_FILE);

  printf("Utente con ID %d eliminato!\n", id_da_rimuovere);
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
        char nuovo_nome[MAX_NOME];

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