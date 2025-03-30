#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fread, fwrite, fseek, remove, rename
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <string.h>                 // Funzioni per la manipolazione delle stringhe: strcpy
#include <stdbool.h>                // Definisce il tipo di dato bool e le costanti true e false
#include <ctype.h>                  // Funzioni per la manipolazione dei caratteri: isalpha, isdigit

#include "schema.h"


Schema schema = { .tabelle = { 0 }, .num_tabelle = 0, .mutex = PTHREAD_MUTEX_INITIALIZER };   // Inizializzo la variabile globale schema

/**
  Questo metodo si occupa di caricare il file che contiene lo schema di tutte le tabelle definite dall'utente.
  Se il file non esiste, viene creato uno schema vuoto.
  Se il file esiste, carica lo schema dal file.
  @return 1 se il caricamento è avvenuto con successo, 0 altrimenti
*/
int load_schema() {
  printf("carico lo schema...\n");

  FILE *file = fopen(SCHEMA_FILE, "rb");
  if (file == NULL) {                                                               // Se il file Schema non esiste
    printf("File schema non trovato. Creazione di un nuovo file vuoto...\n");

    memset(&schema, 0, sizeof(Schema));                                             // Inizializzo la variabile schema a 0
    schema.num_tabelle = 0;

    return write_schema_to_file();                                                  // Scrivo lo schema vuoto nel file
  } else {
    printf ("Caricamento schema esistente...\n");                                   // Se il file esiste, carica lo schema

    if (fread(&schema, sizeof(Schema), 1, file) != TRUE) {                          // Leggo lo schema dal file
      printf("Errore: dati corrotti nel file schema\n");
      fclose(file);
      return FAILURE;
    }
  }

  return SUCCESS;
}


/** 
  Questo metodo si occupa di cercare una tabella nello schema.
  @param table_name Nome della tabella da cercare
  @return Puntatore alla tabella se trovata, NULL altrimenti
*/
TableDefinition* get_table_from_schema(const char* table_name) {
  for (int i = 0; i < schema.num_tabelle; i++) {
    if (strcmp(schema.tabelle[i].nome_tabella, table_name) == SUCCESS) {
      return &schema.tabelle[i];
    }
  }
  return NULL;  // Tabella non trovata
}


/** 
  Questo metodo si occupa di aggiungere una tabella allo schema.
  @param new_table Puntatore alla tabella da aggiungere
  @return 1 se la tabella è stata aggiunta con successo, 0 altrimenti
*/
int add_table_to_schema(TableDefinition* new_table) {

  if (get_table_from_schema(new_table->nome_tabella) != NULL) {
    printf("❌ Errore: la tabella '%s' esiste già.\n", new_table->nome_tabella);
    return FAILURE;
  }

  if (schema.num_tabelle >= MAX_TABLES) {
    printf("Errore: numero massimo di tabelle raggiunto\n");
    return FAILURE;
  }

  pthread_mutex_lock(&schema.mutex);                              // Blocco l'accesso alla variabile schema per poterla modificare

  schema.tabelle[schema.num_tabelle] = *new_table;                // Aggiungo la nuova tabella allo schema
  schema.num_tabelle++;

  pthread_mutex_unlock(&schema.mutex);

  int result = write_schema_to_file();                            // Scrivo lo schema nel file

  if(result != SUCCESS) {

    pthread_mutex_lock(&schema.mutex);                                        // Se la scrittura fallisce, riacquisisco il blocco per ripristinare lo schema

    memset(&schema.tabelle[schema.num_tabelle], 0, sizeof(TableDefinition));  // Se c'è stato un errore, resetto la tabella
    schema.num_tabelle--;                                                     // Decremento il numero di tabelle

    pthread_mutex_unlock(&schema.mutex);                                      // Sblocco l'accesso

    printf("❌ Errore: scrittura dello schema su file fallita. Tabella non aggiunta.\n");
  }

  return result;
}


/** 
  Questo metodo si occupa di rimuovere una tabella dallo schema.
  @param table_name Nome della tabella da rimuovere
  @return 1 se la tabella è stata rimossa con successo, 0 altrimenti
*/
int remove_table_from_schema(const char* table_name) {
  int index_to_remove = -1;

  for (int i = 0; i < schema.num_tabelle; i++) {                                  // num_tabelle = 10
    if (strcmp(schema.tabelle[i].nome_tabella, table_name) == SUCCESS) {          // index_to_remove = 2 (la tabella da rimuovere)
      index_to_remove = i;
      break;
    }
  }

  if (index_to_remove == -1) {
    printf("Errore: tabella non trovata\n");
    return FAILURE;
  }

  // Salva una copia della tabella da rimuovere
  TableDefinition table_removed = schema.tabelle[index_to_remove];

  // Blocca l'accesso per modificare lo schema
  pthread_mutex_lock(&schema.mutex);

  // Sposta tutte le tabelle successive per "compattare"
  for (int i = index_to_remove; i < schema.num_tabelle - 1; i++) {              // da 2 a 9: la 2 diventa la 3, la 3 diventa la 4, ecc. fino alla 9 che diventa la 10
    schema.tabelle[i] = schema.tabelle[i + 1];
  }
  
  // Elimina l'ultima tabella (non necessario se l'array è compatto, ma è una misura precauzionale)
  memset(&schema.tabelle[schema.num_tabelle], 0, sizeof(TableDefinition));      // la 10 ora diventa vuota
  schema.num_tabelle--;                                                         // num_tabelle = 9

  // Sblocca l'accesso prima del write per evitare deadlock
  pthread_mutex_unlock(&schema.mutex);

  // Scrive lo schema nel file
  int result = write_schema_to_file();

  // Se la scrittura non è andata a buon fine, ripristina la tabella
  if (result != SUCCESS) {

    // Se la scrittura fallisce, riacquisisci il blocco per ripristinare lo schema
    pthread_mutex_lock(&schema.mutex);

    schema.num_tabelle++;                                                       // Aumenta il numero di tabelle: num_tabelle = 10

    // Adesso dovrei spostare tutte le tabelle indietro
    for (int i = schema.num_tabelle; i > index_to_remove; i--) {                // da 10 a 2: la 9 diventa la 10, la 8 diventa la 9, ecc. fino alla 2 che diventa la 3
      schema.tabelle[i] = schema.tabelle[i - 1];
    }

    schema.tabelle[index_to_remove] = table_removed;                            // Riaggiungo la tabella nella posizione originale

    pthread_mutex_unlock(&schema.mutex);
    printf("❌ Errore: scrittura dello schema su file fallita. Tabella '%s' non rimossa.\n", table_name);
  }

  return result;  
}

/** 
  Questo metodo si occupa di scrivere lo schema nel file.
  @return 1 se la scrittura è avvenuta con successo, 0 altrimenti
*/
int write_schema_to_file() {
  FILE *file = fopen(SCHEMA_FILE, "wb");
  if (file == NULL) {
    printf("Errore nell'aprire il file schema per la scrittura\n");
    return FAILURE;
  }

  fwrite(&schema, sizeof(Schema), 1, file);
  fclose(file);
  return SUCCESS;
}

/** 
  Questo metodo si occupa di stampare lo schema delle tabelle.
*/
void print_schema() {
  printf("Schema contiene %d tabelle:\n", schema.num_tabelle);

  for (int i = 0; i < schema.num_tabelle; i++) {
    TableDefinition *table = &schema.tabelle[i];
    printf("\nTabella: %s\n", table->nome_tabella);
    printf("Colonne: %d\n", table->num_colonne);

    for (int j = 0; j < table->num_colonne; j++) {
      ColumnDefinition *column = &table->colonne[j];
      printf("- %s (%s, %d, Convert: %p)\n", column->nome_colonna, column->tipo.name, column->tipo.length, column->tipo.convert);
    }
  }
}


void* create_table_record_struct(const char* table_name) {
  TableDefinition* table = get_table_from_schema(table_name);
  if (!table) { return NULL; }

  size_t record_size = get_record_size(table_name);  // Calcolo la dimensione totale
  void* record = malloc(record_size);               // Allocazione unica
  if (!record) { return NULL; }

  memset(record, 0, record_size);  // Inizializza la memoria a 0 (per evitare dati sporchi)

  return record;
}


void free_table_record_struct(void* record) {
  free(record);
}


/** 
 * Questa funzione si occupa di ottenere la dimensione totale di un record dato il nome della tabella 
 * La tabella è descritta da TableDefinition, che contiene informazioni sulle colonne della tabella. 
 * Per ogni colonna della tabella, aggiunge al record_size la dimensione di quel tipo di dato.
 * Alla fine, si avrà la size totale del record completo.
*/
size_t get_record_size(const char* table_name) {
  TableDefinition* table = get_table_from_schema(table_name);
  if (!table) { return 0; }

  size_t record_size = 0;
  for (int i = 0; i < table->num_colonne; i++) {
    ColumnDefinition col = table->colonne[i];
    record_size += col.tipo.length;
  }

  return record_size;
}
