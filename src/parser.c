/* 

  Parser.c è il file che si occupa di interpretare i comandi inseriti dall'utente.
  Il parser è responsabile di:

  * Analizzare il comando inserito dall'utente.
  * Identificare l'azione richiesta (CREATE, DELETE, ecc.).
  * Estrarre il nome della tabella e i dati da inserire.
  * Chiamare la funzione corrispondente (CREATE, UPDATE, ecc.).
  * 
  * 
  * ✅ Il parser non è case-sensitive. I comandi vanno scritti in MAIUSCOLO, così è deciso.
  * ✅ Se il primo token non è un comando valido, il programma esce subito.
  * ✅ Se il comando è valido, chiama la funzione di validazione corrispondente.
  * ✅ Ogni comando ha la sua funzione di validazione.
  * ✅ Se anche il comando è validato rispetto alla funzione che deve eseguire, allora esegue il comando.
  * 
  * Ecco la lista dei comandi che il nostro sistema supporta:

  1️⃣ INFO <Comando>
  ➝ Ritorna le informazioni su come utilizzare ogni comando del programma. Se non si specifica il comando, ritorna la lista di comandi disponibili

  2️⃣ SCHEMA
  ➝ Mostra lo schema delle tabelle. 

  3️⃣ DEFINE <NomeTabella> <campo>:<tipo> <campo>:<tipo> …
  ➝ Aggiunge una nuova tabella allo schema. I campi della tabella vengono specificati secondo la dicitura <campo>:<tipo>.

  4️⃣ CREATE <NomeTabella> <campo>:<valore> <campo>:<valore> …
  ➝ Crea un nuovo oggetto nella tabella specificata. La Tabella deve essere prima definita nello schema. Non è necessario specificare tutti i campi, solo quelli che si vuole valorizzare.

  5️⃣ READ <NomeTabella>
  ➝ Legge tutti i record di una tabella specificata. Mostra i dati in modo formattato.

  6️⃣ UPDATE <NomeTabella> <ID> <campo>:<valore> <campo>:<valore> …
  ➝ Aggiorna un record esistente di una tabella specificata. Non è necessario specificare tutti i campi, solo quelli che si vuole aggiornare.

  7️⃣ FIND <NomeTabella> <campo>:<valore>
  ➝ Cerca un record in una tabella in base a un criterio.

  8️⃣ DELETE <NomeTabella> <ID>
  ➝ Elimina un oggetto specifico tramite ID.

*/

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#include "schema.h"
#include "commands/define.h"
#include "commands/create.h"

/**
 * Questa funzione processa il comando inserito dall'utente.
 * Partendo da una stringa di input, si ottiene una lista di token, suddividendo la stringa per gli spazi
 * Infatti, ogni sintassi dei comandi del nostro sistema, si basa sulla suddivisione per spazi.
 * Una volta ottenuta la lista di token, il PRIMO deve essere sempre il comando
 * Se non c'è un comando valido nella stringa, si ritorna subito l'errore
 * Se il comando è valido, viene validato e in caso, eseguito
 * 
 * Definire la lista di comandi che l'utente può inserire tramite un elenco chiuso mi permette di avere il controllo su tutto quello che succede. 
 */
void process_command(char *input) {
  char *tokens[MAX_TOKENS];                            // Array per memorizzare i token del comando
  int token_count = 0;

  // Tokenizzo la stringa (divide l'input in parole separate dagli spazi)
  // Per tokenizzare la stringa, utilizzo la funzione strtok_r invece di strtok.
  // La funzione strtok_r è una versione thread-safe di strtok, che utilizza un puntatore a char (saveptr) per mantenere lo stato.
  char *token;
  char *saveptr;  // Variabile per mantenere lo stato

  // La funzione strtok_r() divide una stringa in token (parole) utilizzando un delimitatore specificato e si salva il puntatore alla posizione successiva in saveptr.
  token = strtok_r(input, " ", &saveptr);              // Punta ora alla prima parola del comando

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

  if (command == CMD_UNKNOWN) {                         // Se il comando non è riconosciuto, esco
    printf("❌ Errore: comando non valido.\n");
    return;
  }

  switch (command) {                                    // Eseguo il comando corrispondente
    case CMD_INFO:
      // validate_info(tokens, token_count);
      break;
    case CMD_SCHEMA:
      print_schema();
      break;
    case CMD_DEFINE:
      if (validate_define(tokens, token_count)) { execute_define(tokens, token_count); }
      break;
    case CMD_CREATE:
      if (validate_create(tokens, token_count)) { execute_create(tokens, token_count); }
      break;
    case CMD_READ:
      // validate_read(tokens, token_count);
      break;
    case CMD_UPDATE:
      // validate_update(tokens, token_count);
      break;
    case CMD_FIND:
      // validate_find(tokens, token_count);
      break;
    case CMD_DELETE:
      // validate_delete(tokens, token_count);
      break;
    default:
      printf("❌ Errore interno.\n");
  }

}



/**
 * Questa funzione determina se una stringa corrisponde a un comando del sistema.
 * Se si, ritorna il comando corrispondente.
 * In questo modo, mi assicuro che vengano utilizzati solo i comandi che io ho definito.
 */
CommandType get_command_type(char *command) {
  if (strcmp(command, "INFO")   == SUCCESS) return CMD_INFO;
  if (strcmp(command, "SCHEMA") == SUCCESS) return CMD_SCHEMA;
  if (strcmp(command, "DEFINE") == SUCCESS) return CMD_DEFINE;
  if (strcmp(command, "CREATE") == SUCCESS) return CMD_CREATE;
  if (strcmp(command, "READ")   == SUCCESS) return CMD_READ;
  if (strcmp(command, "UPDATE") == SUCCESS) return CMD_UPDATE;
  if (strcmp(command, "FIND")   == SUCCESS) return CMD_FIND;
  if (strcmp(command, "DELETE") == SUCCESS) return CMD_DELETE;

  return CMD_UNKNOWN;
}