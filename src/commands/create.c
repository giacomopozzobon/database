#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fread, fwrite, fseek, remove, rename
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <string.h>                 // Funzioni per la manipolazione delle stringhe: strcpy
#include <stdbool.h>                // Definisce il tipo di dato bool e le costanti true e false
#include <ctype.h>                  // Funzioni per la manipolazione dei caratteri: isalpha, isdigit
#include <sys/stat.h>

#include "create.h"
#include "../schema.h"
#include "../utils.h"







/**
 * Questo metodo si occupa di validare il comando CREATE.
 * - Controlla che il comando abbia almeno 3 token
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
  if (token_count < 3) {
    printf("❌ Errore: sintassi non valida. Usa CREATE <NomeTabella> <campo>:<valore> <campo>:<valore> …\n");
    return FALSE;
  }

  // Il primo token deve essere CREATE
  if (strcmp(tokens[0], "CREATE") != 0) {
    printf("Errore: comando non riconosciuto\n");
    return FALSE;
  }

  // Il secondo token deve essere una stringa che rappresenta il nome della tabella
  char *table_name = tokens[1];
  if (!verify_is_only_letters(table_name)) {
    printf("Errore: Il nome della tabella non è valido\n");
    return FALSE;
  }

  // Cerco la tabella nello schema
  TableDefinition *table = get_table_from_schema(table_name);

  // Se la tabella non è trovata
  if (table == NULL) {
    printf("❌ Errore: La tabella '%s' non esiste nello schema\n", table_name);
    return FALSE;
  }

  // Verifica che tutti i token rimanenti siano nella forma campo:valore
  for (int i = 2; i < token_count; i++) {
    char *campo, *valore;
    char buffer[100];

    strncpy(buffer, tokens[i], sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char *separator = strchr(buffer, ':');
    if (!separator) {
      printf("❌ Errore: il campo '%s' non è nel formato corretto (campo:valore)\n", tokens[i]);
      return 0;
    }

    *separator = '\0';  // Spezza la stringa in due parti
    campo = buffer;
    valore = separator + 1;

    // Controlla se il campo esiste nello schema della tabella
    int field_found = FALSE;

    for (int j = 0; j < table->num_colonne; j++) {
      if (strcmp(table->colonne[j].nome_colonna, campo) == SUCCESS) {
        field_found = TRUE;

        // Assicuriamoci che il valore non sia vuoto
        if (valore == NULL || strlen(valore) == 0) {
          printf("❌ Errore: il campo '%s' deve avere un valore dopo ':'\n", campo);
          return 0;
        }

        // Se il tipo è char, controlla la lunghezza del valore
        if (strcmp(table->colonne[j].tipo, "char") == SUCCESS && strlen(valore) > (size_t)table->colonne[j].lunghezza) {
          printf("❌ Errore: il valore per il campo '%s' supera la lunghezza massima di %d caratteri\n", campo, table->colonne[j].lunghezza);
          return 0;
        }

        // Se il tipo è int, verifica che sia un numero valido
        if (strcmp(table->colonne[j].tipo, "int") == SUCCESS) {
          char *endptr;
          strtol(valore, &endptr, 10);
          if (*endptr != '\0') {
            printf("❌ Errore: il valore '%s' per il campo '%s' non è un numero valido\n", valore, campo);
            return 0;
          }
        }

        break;
      }
    }

    if (!field_found) {
      printf("❌ Errore: il campo '%s' non esiste nella tabella '%s'\n", campo, table_name);
      return FALSE;
    }
  }


  printf("✅ Comando CREATE valido\n");
  return TRUE;
}