/* 


  Define.c è il file che racchiude le funzioni relative al comando DEFINE.
  Le funzioni descritte in questo file sono:
    - execute_define: si occupa di eseguire il comando DEFINE.
    - print_schema: si occupa di leggere il contenuto dello Schema in modo più leggibile.
    - validate_define: si occupa di validare il comando DEFINE.

  Il comando DEFINE permette di definire una tabella con i relativi campi e tipi di dati.
  Ad esempio, DEFINE Utenti nome:char eta:int

  Il comando DEFINE accetta almeno 3 token:
    - Il primo token deve essere DEFINE
    - Il secondo token deve essere il nome della tabella
    - I token successivi devono essere nella forma campo:tipo

  Definire una tabella significa stabilire che da quel momento in poi si potrà crearla e popolarla con oggetti che rispettino la struttura definita.
  Questo comando non crea la tabella fisicamente, ma la definisce in memoria.
  Il file _defines.bin può essere considerato come il "dizionario" delle tabelle, ovvero lo "schema" del database.


*/

#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fread, fwrite, fseek, remove, rename
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <string.h>                 // Funzioni per la manipolazione delle stringhe: strcpy
#include <stdbool.h>                // Definisce il tipo di dato bool e le costanti true e false
#include <ctype.h>                  // Funzioni per la manipolazione dei caratteri: isalpha, isdigit
#include <sys/stat.h>               // Funzioni per la gestione dei file: stat, mkdir

#include "define.h"
#include "../schema.h"
#include "../utils.h"

/*
  * Questo metodo si occupa di eseguire il comando DEFINE.
    - Apre il file _defines.bin
    - Aggiunge la definizione della tabella descritta dal comando
*/
void execute_define(char *tokens[], int token_count) {

  TableDefinition new_table;

  // Attenzione qui. Strncpy copia al massimo n caratteri, quindi non c'è rischio di buffer overflow
  // Infatti, come terzo parametro, passiamo sizeof(table.nome_tabella) - 1, ovvero la dimensione massima del campo - 1
  // table.nome_tabella è un array di caratteri di 50 elementi, quindi la dimensione massima è 49
  // -1 perchè dobbiamo lasciare spazio per il terminatore di stringa \0
  strncpy(new_table.nome_tabella, tokens[1], sizeof(new_table.nome_tabella) - 1);   // Copia il nome della tabella. Es. "Utenti"
  new_table.nome_tabella[sizeof(new_table.nome_tabella) - 1] = '\0';                // Assicuriamoci che sia null-terminated
  new_table.num_colonne = 0;

  for (int i = DEFINE_INIT_TOKENS; i < token_count; i++) {

    ColumnDefinition column = parse_column_definition(tokens[i]);

    // Verifica che la colonna sia stata estratta correttamente
    if (strlen(column.nome_colonna) == 0 || strlen(column.tipo) == 0) {
      printf("Errore: formato colonna non valido per %s\n", tokens[i]);
      return;
    }

    new_table.colonne[new_table.num_colonne] = column;
    new_table.num_colonne++;
  }

  if (add_table_to_schema(&new_table) == SUCCESS) {
    printf("Tabella '%s' aggiunta con successo!\n", new_table.nome_tabella);
  } else {
    printf("Errore nell'aggiunta della tabella '%s'\n", new_table.nome_tabella);
  }
}


/*
  * Questo metodo si occupa di validare il comando DEFINE.
    - Controlla che il comando abbia almeno 3 token
    - Controlla che il primo token sia DEFINE
    - Controlla che il nome della tabella sia una stringa valida
    - Controlla che i token successivi siano nella forma campo:tipo
*/
int validate_define(char *tokens[], int token_count) {
  if (token_count < 3) {
    printf("❌ Errore: sintassi non valida. Usa DEFINE <NomeTabella> <campo>:<tipo> <campo>:<tipo> …\n");
    return FALSE;
  }

  if (strcmp(tokens[0], "DEFINE") != 0) {                 // Il primo token deve essere DEFINE
    printf("Errore: comando non riconosciuto\n");
    return FALSE;
  }

  char *table_name = tokens[1];
  if (!verify_is_only_letters(table_name)) {                       // Il secondo token deve essere una stringa che rappresenta il nome della tabella
    printf("Errore: Il nome della tabella non è valido\n");
    return FALSE;
  }

  for (int i = 2; i < token_count; i++) {                 // Tutti gli altri token dopo, devono essere nella forma nome:tipo
    if (!validate_define_column_format_token(tokens[i])) { return FALSE; }
  }

  printf("✅ Comando DEFINE valido\n");
  return TRUE;
}


/**
 * Funzione per validare il formato di una colonna nel DEFINE.
 * Il formato deve essere nome:tipo
 * Esempio: id:int
 * 
 * @param column La stringa da validare
 * @return 1 se il formato è valido, 0 altrimenti
 */
int validate_define_column_format_token(char *column) {
  ColumnDefinition col = parse_column_definition(column);

  if (col.nome_colonna[0] == '\0') { return FALSE; }    // Se il nome è vuoto, il parsing è fallito

  if (!verify_is_only_letters(col.nome_colonna)) {      // Il nome della colonna deve essere una stringa fatta di sole lettere
    printf("Errore: il nome della colonna %s non è valido\n", col.nome_colonna);
    return FALSE;
  }

  if (!valid_column_type(col.tipo)) {                   // Il tipo della colonna deve essere di una tipologia valida (int o char)
    printf("Errore: il tipo %s non è supportato. Usa int o char\n", col.tipo);
    return FALSE;
  }

  return TRUE;    // Se tutto è valido
}