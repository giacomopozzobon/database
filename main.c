/* 

  Benvenuto, questo è il mio primo codice in C.

  * Si tratta di un piccolo database in cui è possibile fare operazioni limitate.
  * In questa versione i database di ogni oggetto sono dei file binari. In questo modo i dati sono salvati in modo permanente e più veloce rispetto ad un file di testo.
  * Ogni file rappresenta un oggetto e ogni riga del file rappresenta un record.
  * 
  
  Main.c è il file principale del programma. Si occupa di:
  * Dare il benvenuto all'utente: Spiega anche i comandi che può usare.
  * Genera il Loop principale: Continua a chiedere comandi fino a che l'utente non digita EXIT.
  * Riceve l'input: Usa fgets() per leggere il comando e rimuove il \n finale, in modo da lavorare sulla stringa pulita.
  * Passa il comando al parser: Chiama process_command(input); per elaborare il comando inserito dall'utente.

*/

#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fread, fwrite, fseek, remove, rename
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <string.h>                 // Funzioni per la manipolazione delle stringhe: strcpy

#include "src/schema.h"
#include "src/parser.h"
#include "src/utils.h"


/* Funzione principale del programma
  * Mostra un messaggio di benvenuto all'utente.
  * Prende in input il comando dell'utente e lo processa.
*/
int main () {
  printf("...\n");
  printf("avvio il progetto...\n");

  if(load_schema() != SUCCESS) {                // Carica lo schema delle tabelle
    printf("Chiusura del programma...\n");
    return FAILURE;
  } else {
    /** Attenzione qui. Siccome quando scrivo e leggo lo schema sul file, scrivo i ColumnType,
     *  I ColumnType contengono i puntatori alle funzioni di conversione.
     *  Quando leggo lo schema dopo un riavvio del programma, i puntatori alle funzioni di conversione non sono più validi.
     *  Quindi, devo riassegnare i puntatori alle funzioni di conversione.
    */

    fix_conversion_functions();                  // Corregge le funzioni di conversione per i tipi di dati
  }

  char input[MAX_INPUT_SIZE];                   // Dichiaro un array di caratteri per contenere l'input dell'utente

  printf("\n");
  printf("📂 Benvenuto nel database!\n");       // Messaggio di benvenuto
  printf("🔥 Versione %d\n", VERSIONE);         // Mostra la versione del programma
  printf("\n");
  printf("📜 Comandi supportati:\n");
  printf("▪️ DEFINE Utente nome:char eta:int ...\n");
  printf("▪️ READ DEFINES\n");
  printf("▪️ CREATE Utente nome:'Luca' eta:32 ...\n");
  printf("▪️ READ Utente\n");
  printf("▪️ UPDATE Utente 1 nome:'Mario'\n");
  printf("▪️ FIND Utente nome:'Luca'\n");
  printf("▪️ DELETE Utente 1\n");
  printf("\n");
  printf("Inserisci un comando oppure 'EXIT' per uscire.\n");

  while(TRUE) {                                    // Ciclo infinito. Il programma termina solo se l'utente inserisce 'EXIT'
    printf("👉 ");

    // Leggo l'input dell'utente
    if(fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
      printf("Errore durante la lettura dell'input\n");
      continue;
    }


    // Rimuovo il carattere \n dalla fine dell'input
    // strcspn restituisce la posizione del primo carattere \n nell'array di caratteri.
    // Una volta trovata la posizione del \n, lo sostituisce con 0 (che in C è equivalente a '\0', il terminatore di stringa).

    // Se input è una stringa letta da fgets(), questa riga assicura che il \n non venga incluso, poiché fgets() inserisce il carattere di nuova riga se c'è spazio nel buffer.
    input[strcspn(input, "\n")] = 0;


    // Se l'utente ha inserito 'EXIT' esco dal programma
    if (strcmp(input, "EXIT") == SUCCESS) {
      printf("👋 Chiusura del database... Arrivederci!\n");
      break;
    }

    process_command(input);                   // Processo il comando dell'utente
  }

  return SUCCESS;       // Ritorno 0 per indicare che il programma è terminato correttamente
}