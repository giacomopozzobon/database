#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"
#include "../utils.h"


// Funzione per stampare il contenuto di un file binario in base allo schema
void print_table(const char *table_name) {
  // Ottenere la definizione della tabella dallo schema
  TableDefinition *table = get_table_from_schema(table_name);
  if (!table) {
      printf("Tabella %s non trovata nello schema.\n", table_name);
      return;
  }

  FILE* file = open_table_file(table_name, "rb");

  if (!file) {
    printf("Errore nell'apertura del file tables/%s\n", table_name);
    return;
  }

  // Determinare la dimensione di un record
  size_t record_size = get_record_size(table_name);
  void *record = create_table_record_struct(table_name); // Allocazione memoria per un record

  // Stampare le intestazioni delle colonne
  printf("Tabella: %s\n", table_name);
  for (int i = 0; i < table->num_colonne; i++) {
      printf("%s\t", table->colonne[i].nome_colonna);
  }
  printf("\n");

  // Leggere e stampare ogni record
  while (fread(record, record_size, 1, file)) {
    char *ptr = (char *)record; // Puntatore generico al record
    for (int i = 0; i < table->num_colonne; i++) {
      ColumnDefinition col = table->colonne[i];

      // Stampare il valore in base al tipo
      if (strcmp(col.tipo.name, "int") == 0) {
        int value;
        memcpy(&value, ptr, sizeof(int));
        printf("%d\t", value);
        ptr += sizeof(int);
      } else if (strcmp(col.tipo.name, "char") == 0) {
        char value[255];  // Supponiamo che la lunghezza max sia 255
        memcpy(value, ptr, col.tipo.length);
        value[col.tipo.length] = '\0'; // Terminatore stringa
        printf("%s\t", value);
        ptr += col.tipo.length;
      } else if (strcmp(col.tipo.name, "float") == 0) {
        float value;
        memcpy(&value, ptr, sizeof(float));
        printf("%.2f\t", value);
        ptr += sizeof(float);
      } else if (strcmp(col.tipo.name, "double") == 0) {
        double value;
        memcpy(&value, ptr, sizeof(double));
        printf("%.2f\t", value);
        ptr += sizeof(double);
      } else if (strcmp(col.tipo.name, "timestamp") == 0) {
        long value;
        memcpy(&value, ptr, sizeof(long));
        printf("%ld\t", value);
        ptr += sizeof(long);
      } else if (strcmp(col.tipo.name, "bool") == 0) {
        bool value;
        memcpy(&value, ptr, sizeof(bool));
        printf("%s\t", value ? "true" : "false");
        ptr += sizeof(bool);
      } else {
          printf("??\t"); // Tipo sconosciuto
      }
    }
    printf("\n");
  }

  // Pulizia
  free(record);
  fclose(file);
}
