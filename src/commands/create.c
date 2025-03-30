#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fread, fwrite, fseek, remove, rename
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <string.h>                 // Funzioni per la manipolazione delle stringhe: strcpy
#include <stdbool.h>                // Definisce il tipo di dato bool e le costanti true e false
#include <ctype.h>                  // Funzioni per la manipolazione dei caratteri: isalpha, isdigit
#include <unistd.h>                 // Funzioni per access(), F_OK, R_OK, W_OK
#include <time.h>
#include <sys/stat.h>

#include "create.h"
#include "../schema.h"
#include "../utils.h"





void execute_create(char *tokens[], int token_count) {
  if (token_count < 3) {
    printf("Errore: comando CREATE incompleto\n");
    return;
  }

  const char *table_name = tokens[1];                                         // Nome della tabella
  TableDefinition* table = get_table_from_schema(table_name);                 // Ottengo lo schema della tabella

  void *record = create_table_record_struct(table_name);                      // Step 1: Creo una nuova struct per il record
  if (record == NULL) {
    printf("Errore: impossibile creare la struct del record\n");
    return;
  }

  size_t offset = 0;

  ColumnDefinition col;
  ColumnValueDefinition col_val;

  for (int i = 0; i < table->num_colonne; i++) {                              // Step 2: Scorro le colonne della tabella per scrivere i valori nei campi corrispondenti
    col = table->colonne[i];

    // ID, CreatedAt e UpdatedAt sono i campi che vengono valorizzati in modo automatico
    // Non voglio che l'utente si preoccupi minimamente di aggiungere questi campi alle sue tabelle
    if (strcmp(col.nome_colonna, "id") == SUCCESS) {                  // Imposto l'ID
      int next_id = get_next_id_for_table(table_name);
      memcpy((char*)record + offset, &next_id, col.tipo.length);
    } else if (strcmp(col.nome_colonna, "created_at") == SUCCESS) {   // Imposto CreatedAt con il timestamp di creazione
      long timestamp = get_current_timestamp();
      memcpy((char*)record + offset, &timestamp, col.tipo.length);
      continue;
    } else if (strcmp(col.nome_colonna, "updated_at") == SUCCESS) {   // UpdatedAt è nullo perchè sarà inserito a ogni UPDATE
      memcpy((char*)record + offset, get_null_value(col.tipo), col.tipo.length);
      continue;
    } else {


      // Cerco tra i token il valore per questa colonna
      int found = FALSE;

      for (int j = CREATE_INIT_TOKENS; j < token_count; j++) {
        col_val = parse_column_value_definition(table, tokens[j]);

        if (col_val.valore && strcmp(col_val.campo.nome_colonna, col.nome_colonna) == SUCCESS) {
          memcpy((char*)record + offset, col_val.valore, col.tipo.length);
          free(col_val.valore); // Libera la memoria allocata in parse_column_value_definition
          found = TRUE;
          break;
        }
      }

      if (!found) {                                                           // Per tutti gli altri campi, metto il loro NULL
        memcpy((char*)record + offset, get_null_value(col.tipo), col.tipo.length);
      }
    }

    offset += col.tipo.length;                                                // Aggiorno l'offset con la lunghezza della colonna attuale
  }

  // Step 3: Scrivo il record nella tabella corrispondente
  FILE* file = open_table_file(table_name, "a+b");

  if (file) {
    fwrite(record, get_record_size(table_name), 1, file);
    printf("Record aggiunto alla tabella %s\n", table_name);

    free_table_record_struct(record);
    fclose(file);
  }
  
}



/**
 * Funzione che valida i token del comando CREATE.
 * Devono essere almeno CREATE_INIT_TOKENS + 1 token
 * - Controlla che il primo token sia CREATE
 * - Controlla che il nome della tabella sia una stringa valida
 * - Controlla che i token successivi siano nella forma campo:valore
 * - Controlla che il campo esista nello schema della tabella
 * - Controlla che il valore non sia vuoto
 * - Controlla che il valore non superi la lunghezza massima
 * - Controlla che il valore sia un numero valido se il tipo è int
 *
 * @param tokens Array di token
 * @param token_count Numero di token
 * @return 1 se il comando è valido, 0 altrimenti
 */
int validate_create(char *tokens[], int token_count) {
  if (token_count < CREATE_INIT_TOKENS + 1) {
    printf("❌ Errore: sintassi non valida. Usa CREATE <NomeTabella> <campo>:<valore> <campo>:<valore> …\n");
    return FALSE;
  }

  if (strcmp(tokens[0], "CREATE") != SUCCESS) {
    printf("Errore: comando non riconosciuto\n");
    return FALSE;
  }

  char *table_name = tokens[1];
  if (!verify_is_only_letters(table_name)) {
    printf("Errore: Il nome della tabella non è valido\n");
    return FALSE;
  }

  // Cerco la tabella nello schema
  TableDefinition *table = get_table_from_schema(table_name);
  if (table == NULL) {
    printf("❌ Errore: La tabella '%s' non esiste nello schema\n", table_name);
    return FALSE;
  }

  // Verifica che tutti i token rimanenti siano nella forma campo:valore
  ColumnValueDefinition couple;

  for (int i = CREATE_INIT_TOKENS; i < token_count; i++) {
    printf("Step hihi dentro for: %i %s\n", i, tokens[i]);
    couple = parse_column_value_definition(table, tokens[i]);

    if (strlen(couple.campo.nome_colonna) == 0) {
      printf("Errore: token non valido per %s\n", tokens[i]);
      return FALSE;
    }
  }


  printf("✅ Comando CREATE valido\n");
  return TRUE;
}


// Funzione di utilità per creare la cartella se non esiste
void create_tables_directory_if_not_exists() {
  if (access(TABLES_DIR, F_OK) == -1) { // Controlla se esiste
    if (mkdir(TABLES_DIR, 0700) == -1) {
      perror("Errore nella creazione della cartella");
    }
  }
}