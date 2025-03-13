/* 


  Utils.c è il file che si occupa di gestire le operazioni comuni.
  E' composto da funzioni che velocizzano il lavoro del programma, come la validazione del formato delle colonne.

  Le funzioni descritte in questo file sono:
    - validate_define_column_format_token:    si occupa di validare un token del comando DEFINE
    - valid_column_type:                      si occupa di validare il tipo di una colonna. Per ora sono supportati solo int e char
    - parse_column_definition:                si occupa di analizzare un token e restituire una struttura ColumnDefinition
    - verify_is_only_letters:                 si occupa di verificare che una variabile contenga solo caratteri alfabetici

  Riutilizzando queste funzioni, si velocizza il lavoro e si riducono gli errori.


*/

#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fread, fwrite, fseek, remove, rename
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <string.h>                 // Funzioni per la manipolazione delle stringhe: strcpy
#include <stdbool.h>                // Definisce il tipo di dato bool e le costanti true e false
#include <ctype.h>                  // Funzioni per la manipolazione dei caratteri: isalpha, isdigit

#include "utils.h"


/**
 * Funzione per verificare se il tipo di colonna è valido.
 * I tipi supportati sono definiti in column_types.
 * 
 * @param tipo_colonna Il tipo di colonna da verificare
 * @return 1 se il tipo è valido, 0 altrimenti
 */

int valid_column_type(const char *tipo_colonna) {
  for (size_t i = 0; i < NUM_TYPES; i++) {
    if (strcmp(tipo_colonna, column_types[i].name) == SUCCESS) {
      return TRUE;
    }
  }

  printf("Errore: il tipo %s non è supportato.\n", tipo_colonna);
  return FALSE;
}


/**
 * Funzione per analizzare una definizione di colonna e restituire una struttura ColumnDefinition.
 * Il formato della stringa deve essere nome:tipo
 * 
 * @param token La stringa da analizzare
 * @return La struttura ColumnDefinition
 */
ColumnDefinition parse_column_definition(const char *token) {
  ColumnDefinition column;

  memset(&column, 0, sizeof(ColumnDefinition));                 // Inizializza la struttura a 0. è importante per evitare valori non inizializzati

  char token_copy[100];                                         // Creiamo una copia per evitare di modificare il token originale
  strncpy(token_copy, token, sizeof(token_copy) - 1);
  token_copy[sizeof(token_copy) - 1] = '\0';                    // Dopo ogni strncopy devo assicurarmi che sia null-terminated


  char *separatore = strchr(token_copy, ':');                   // Cerca il carattere ':'
  if (!separatore) {
    printf("Errore: il campo %s non è definito correttamente.\n", token);
    return column;
  }

  *separatore = '\0';                                           // Divide la stringa in due parti
  char *nome_colonna = token_copy;                              // La parte prima del separatore è il nome della colonna
  char *tipo_colonna = separatore + 1;                          // La parte dopo il separatore è il tipo della colonna

  ColumnType col_type = get_column_type(tipo_colonna);
  if (col_type.name == NULL) {
    printf("Errore: il tipo \"%s\" non è supportato.\n", tipo_colonna);
    return column;
  }

  strncpy(column.nome_colonna, nome_colonna, sizeof(column.nome_colonna) - 1);
  column.nome_colonna[sizeof(column.nome_colonna) - 1] = '\0';

  column.tipo = col_type;                                       // Assegna il puntatore alla struttura ColumnType

  return column;
}

/** 
 * Funzione per ottenere il tipo di colonna.
 * 
 * @param tipo_colonna Il tipo di colonna da cercare
 * @return Il puntatore alla struttura ColumnType se il tipo è valido, NULL altrimenti
 */
ColumnType get_column_type(const char *tipo_colonna) {
  for (size_t i = 0; i < NUM_TYPES; i++) {
    if (strcmp(tipo_colonna, column_types[i].name) == 0) {
      return column_types[i];  // Restituisce l'oggetto ColumnType
    }
  }
  return (ColumnType){NULL, 0};  // Tipo non valido, restituisce un oggetto con valori di default
}


/**
 * Funzione per verificare se una stringa contiene solo caratteri alfabetici.
 * 
 * @param s La stringa da verificare
 * @return 1 se la stringa è valida, 0 altrimenti
 */
int verify_is_only_letters(const char *s) {
  
  for (size_t i = 0; i < strlen(s); i++) {         // Scorri ogni carattere della stringa
    if (!isalpha(s[i])) {                          // Verifica se il carattere non è una lettera
      printf("Errore: il nome della colonna %s non è valido (contenuto non alfabetico)\n", s);
      return FALSE;             // Nome colonna non valido
    }
  }
  return TRUE;                 // Nome colonna valido
}