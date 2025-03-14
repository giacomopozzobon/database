/* 


  Define.c è il file che racchiude le funzioni relative al comando DEFINE.
  Le funzioni descritte in questo file sono:
    - execute_define: si occupa di eseguire il comando DEFINE.
    - validate_define: si occupa di validare il comando DEFINE.

  Il comando DEFINE permette di definire una tabella con i relativi campi e tipi di dati.
  Ad esempio, DEFINE Utenti nome:char eta:int

  Il comando DEFINE accetta dai 3 token in su:
    - Il primo token deve essere DEFINE
    - Il secondo token deve essere il nome della tabella
    - I token successivi devono essere nella forma campo:tipo

  Definire una tabella significa stabilire che da quel momento in poi si potrà crearla e popolarla con oggetti che rispettino la struttura definita.
  Questo comando non crea la tabella fisicamente, ma la definisce in memoria.
  In questo modo mi assicuro la consistenza tra i record delle tabelle e mi facilito le operazioni per manipolare i dati.

  Il file schema.bin può essere considerato come il "dizionario" delle tabelle, ovvero lo "schema" del database.

  Attenzione: per fornire le basi di un sistema efficiente, ogni tabella ha dei campi bonus comuni: ID, created_at, updated_at
  Questi campi vengono aggiunti in automatico allo schema, e sono compilati automaticamente dal sistema.
  Non è necessario includerli nella definizione della tabella e non bisogna valorizzarli in fase di CREATE o UPDATE.

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

/**
 * Funzione che esegue il comando DEFINE.
 * Come prima cosa crea la tabella attraverso la struttura di sistema, per assicurarsi che ogni comando sia valido.
 * Valorizza la struttura della tabella con i campi definiti dai token.
 * 
 * Aggiunge la tabella allo schema.
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

  ColumnDefinition column;

  column = parse_column_definition("id:int");
  new_table.colonne[new_table.num_colonne] = column;
  new_table.num_colonne++;

  for (int i = DEFINE_INIT_TOKENS; i < token_count; i++) {

    column = parse_column_definition(tokens[i]);

    // Verifica che la colonna sia stata estratta correttamente
    if (strlen(column.nome_colonna) == 0) {
      printf("Errore: formato colonna non valido per %s\n", tokens[i]);
      return;
    }

    // Verifica che il nome della colonna non sia tra quelli riservati: ID, created_at, updated_at
    if (strcasecmp(column.nome_colonna, "id") == SUCCESS || strcmp(column.nome_colonna, "created_at") == SUCCESS || strcmp(column.nome_colonna, "updated_at") == SUCCESS) {
      printf("❌ Errore: Il campo '%s' è già aggiunto automaticamente dal sistema e non può essere ridefinito.\n", column.nome_colonna);
      return;
    }

    new_table.colonne[new_table.num_colonne] = column;
    new_table.num_colonne++;
  }

  column = parse_column_definition("created_at:timestamp");
  new_table.colonne[new_table.num_colonne] = column;
  new_table.num_colonne++;

  column = parse_column_definition("updated_at:timestamp");
  new_table.colonne[new_table.num_colonne] = column;
  new_table.num_colonne++;

  if (add_table_to_schema(&new_table) == SUCCESS) {
    printf("Tabella '%s' aggiunta con successo!\n", new_table.nome_tabella);
  } else {
    printf("Errore nell'aggiunta della tabella '%s'\n", new_table.nome_tabella);
  }
}


/**
 * Funzione che valida i token del comando DEFINE
 * Devono essere almeno DEFINE_INIT_TOKENS + 1 tokens
 * Il primo token deve essere sempre DEFINE
 * Il nome della tabella deve essere una stringa valida (solo lettere)
 * I token successivi devono essere nella forma <campo>:<tipo>
 * 
 * @param i token del comando DEFINE
 * @result valido: 1, errato: 0
*/
int validate_define(char *tokens[], int token_count) {
  if (token_count < DEFINE_INIT_TOKENS + 1) {
    printf("❌ Errore: sintassi non valida. Usa DEFINE <NomeTabella> <campo>:<tipo> <campo>:<tipo> …\n");
    return FALSE;
  }

  if (strcmp(tokens[0], "DEFINE") != SUCCESS) {
    printf("Errore: comando non riconosciuto\n");
    return FALSE;
  }

  char *table_name = tokens[1];
  if (!verify_is_only_letters(table_name)) {
    printf("Errore: Il nome della tabella non è valido\n");
    return FALSE;
  }

  ColumnDefinition col;
  for (int i = DEFINE_INIT_TOKENS; i < token_count; i++) { 
    col = parse_column_definition(tokens[i]);

    if (col.nome_colonna[0] == '\0') {                    // Se il nome è vuoto, il tipo di colonna non è una tipologia valida.
      printf("Errore: il token %s non è valido\n", tokens[i]);
      return FALSE; 
    }

    if (!verify_is_only_letters(col.nome_colonna)) {      // Il nome della colonna deve essere una stringa fatta di sole lettere
      printf("Errore: il nome della colonna %s non è valido\n", col.nome_colonna);
      return FALSE;
    }
  }

  printf("✅ Comando DEFINE valido\n");
  return TRUE;
}