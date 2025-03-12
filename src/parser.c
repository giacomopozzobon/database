/* 

  Parser.c è il file che si occupa di interpretare i comandi inseriti dall'utente.
  Il parser è responsabile di:

  * Analizzare il comando inserito dall'utente.
  * Identificare l'azione richiesta (CREATE, DELETE, ecc.).
  * Estrarre il nome della tabella e i dati da inserire.
  * Chiamare la funzione corrispondente (insert_object(), delete_object(), ecc.).
  * 
  * 
  * ✅ Il parser non è case-sensitive.
  * ✅ Se il primo token non è un comando valido, il programma esce subito.
  * ✅ Se il comando è valido, chiama la funzione di validazione corrispondente.
  * ✅ Ogni comando ha la sua funzione di validazione.
  * 
  * Ecco la lista dei comandi che il nostro sistema supporta:

  1️⃣ DEFINE <NomeTabella> <campo>:<tipo> <campo>:<tipo> …
  ➝ Crea una nuova tabella con dei campi specificati.

  2️⃣ READ <NomeTabella>
  ➝ Legge i dati di una tabella specificata.

  3️⃣ CREATE <NomeTabella> <campo>:<valore> <campo>:<valore> …
  ➝ Crea un nuovo oggetto nella tabella specificata.

  4️⃣ UPDATE <NomeTabella> <ID> <campo>:<valore> <campo>:<valore> …
  ➝ Modifica un oggetto esistente.

  5️⃣ SEARCH <NomeTabella> <campo>:<valore>
  ➝ Cerca un oggetto in base a un criterio.

  6️⃣ DELETE <NomeTabella> <ID>
  ➝ Elimina un oggetto specifico tramite ID.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schema.h"                          // Include il file config.h per le costanti
#include "parser.h"                          // Include il file parser.h per le dichiarazioni delle funzioni & il file config.h per le costanti
#include "commands/define.h"                 // Include il file define.h per le funzioni di definizione delle tabelle
#include "commands/create.h"                 // Include il file create.h per le funzioni di creazione di oggetti


// Funzione per processare il comando inserito dall'utente
void process_command(char *input) {
  char *tokens[MAX_TOKENS];                 // Array per memorizzare i token del comando
  int token_count = 0;

  // Tokenizzo la stringa (divide l'input in parole separate dagli spazi)
  // Per tokenizzare la stringa, utilizzo la funzione strtok_r invece di strtok.
  // La funzione strtok_r è una versione thread-safe di strtok, che utilizza un puntatore a char (saveptr) per mantenere lo stato.
  char *token;
  char *saveptr;  // Variabile per mantenere lo stato

  // La funzione strtok_r() divide una stringa in token (parole) utilizzando un delimitatore specificato e si salva il puntatore alla posizione successiva in saveptr.
  token = strtok_r(input, " ", &saveptr);         // Punta ora alla prima parola del comando

  while(token != NULL & token_count < MAX_TOKENS) {    // Continua finchè non arriva alla fine della stringa o raggiunge il numero massimo di token
    tokens[token_count] = token;                       // Salva il token nell'array
    token_count++;                                     // Incrementa il contatore dei token

    // Attenzione qui: il primo parametro di strtok_r è NULL per indicare che voglio continuare a tokenizzare la stessa stringa.
    token = strtok_r(NULL, " ", &saveptr);             // Passa al token successivo.
  }

  // Controlliamo che ci sia almeno un comando
  if (token_count < 1) {
    printf("❌ Errore: comando non valido.\n");
    return;
  }

  CommandType command = get_command_type(tokens[0]);    // Ottengo il tipo di comando e mi assicuro che sia valido

  // Se il comando non è riconosciuto, esce
  if (command == CMD_UNKNOWN) {
    printf("❌ Errore: comando non valido.\n");
    return;
  }

  // Esegue il comando corrispondente
  switch (command) {
    case CMD_DEFINE:
      if (validate_define(tokens, token_count)) { execute_define(tokens, token_count); }
      break;
    case CMD_READ:
      // validate_read(tokens, token_count);
      print_schema();
      break;
    case CMD_CREATE:
      if (validate_create(tokens, token_count)) {
        //execute_create(tokens, token_count);
        print_schema();
      }
      break;
    case CMD_UPDATE:
      // validate_update(tokens, token_count);
      break;
    case CMD_SEARCH:
      // validate_search(tokens, token_count);
      break;
    case CMD_DELETE:
      // validate_delete(tokens, token_count);
      break;
    default:
      printf("❌ Errore interno.\n");
  }

}



// Funzione per determinare il tipo di comando
CommandType get_command_type(char *command) {
  if (strcmp(command, "DEFINE") == SUCCESS) return CMD_DEFINE;
  if (strcmp(command, "READ")   == SUCCESS) return CMD_READ;
  if (strcmp(command, "CREATE") == SUCCESS) return CMD_CREATE;
  if (strcmp(command, "UPDATE") == SUCCESS) return CMD_UPDATE;
  if (strcmp(command, "SEARCH") == SUCCESS) return CMD_SEARCH;
  if (strcmp(command, "DELETE") == SUCCESS) return CMD_DELETE;

  return CMD_UNKNOWN;
}