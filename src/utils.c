/* 


  Utils.c è il file che si occupa di gestire le operazioni comuni a uno o più componenti.
  E' composto da funzioni che velocizzano il lavoro del programma, come la validazione del formato delle colonne.

  Le funzioni descritte in questo file sono:
    - parse_column_definition:                analizza un token e se è valido, restituisce una ColumnDefinition.
    - parse_column_type:                      analizza una tipologia di campo e se è valida restituisce una ColumnType.
    - get_next_id_for_table:                  ottiene il prossimo ID Univoco disponibile per una tabella. 
    - long get_current_timestamp:             ottiene il Timestamp di questo preciso momento.
    - get_null_value                          ottiene il valore NULL per una tipologia di dato
    - verify_is_only_letters:                 verifica che una variabile contenga solo caratteri alfabetici.

  In questo file è anche definito l'array di column_types, ovvero la lista di tutti i tipi di campi disponibili a sistema.
  E' definito qui come utils perchè l'unico modo per accedervi è utilizzando il parse_column_type per ottenere un ColumnType valido

  Riutilizzando queste funzioni, si velocizza il lavoro e si riducono gli errori.
  E soprattutto, si scrive codice migliore.


*/

#include <stdio.h>                  // Funzioni per la gestione di input/output: printf, scanf, fopen, fclose, fread, fwrite, fseek, remove, rename
#include <stdlib.h>                 // Funzioni per la gestione della memoria: malloc, free, exit
#include <string.h>                 // Funzioni per la manipolazione delle stringhe: strcpy
#include <stdbool.h>                // Definisce il tipo di dato bool e le costanti true e false
#include <ctype.h>                  // Funzioni per la manipolazione dei caratteri: isalpha, isdigit
#include <sys/stat.h>

#include "utils.h"
#include "schema.h"


/** TIPI DI CAMPI UTILIZZABILI A SISTEMA */
/** Attenzione, qui sono definite tutte le tipologie di campi utilizzabili dalle tabelle  */

const ColumnType column_types[] = {
  {"int", sizeof(int), convert_char_to_int},                // Intero
  {"char", 255, convert_char_to_string},                    // Stringa
  {"float", sizeof(float), convert_char_to_float},          // Float
  {"double", sizeof(double), convert_char_to_double},       // Double
  {"bool", sizeof(bool), convert_char_to_bool},             // Bool
  {"timestamp", sizeof(long), convert_char_to_timestamp}    // Timestamp
};

#define COLUMN_TYPES_COUNT (sizeof(column_types) / sizeof(column_types[0]))    // Contatore dei Tipi

/**  ********** *********** *********** *********** *********** *********** ***********   */





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

  ColumnType col_type = parse_column_type(tipo_colonna);
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
 * Funzione per ottenere il tipo di colonna in formato ColumnType, da una stringa
 * Questo è l'unico modo per accedere all'array column_types.
 * Tutte le tipologie di campi utilizzabili sono valorizzati li e da nessun'altra parte
 * Ogni campo ha il suo nome e la sua dimensione.
 * 
 * @param tipo_colonna Il tipo di colonna da cercare
 * @return Il puntatore alla struttura ColumnType se il tipo è valido, NULL altrimenti
 */
ColumnType parse_column_type(const char *tipo_colonna) {
  for (size_t i = 0; i < COLUMN_TYPES_COUNT; i++) {
    if (strcmp(tipo_colonna, column_types[i].name) == 0) {
      return column_types[i];  // Restituisce l'oggetto ColumnType
    }
  }
  return (ColumnType){NULL, 0};  // Tipo non valido, restituisce un oggetto con valori di default
}

/** 
 * Funzione per ottenere la coppia Colonna:Valore in formato ColumnValueDefinition, da un token.
 * Questo è l'unico modo per assicurarsi che il token <campo>:<valore> sia effettivamente un token valido per la tabella.
 * Questo parsing comprende anche il controllo sulla validità del campo, se il campo appartiene alla tabella e se il valore è valido.
 * Se il valore del token è valido, lo converte già nel tipo corrispondente del campo.
 * 
 * @param table: la tabella di riferimento, il token
 * @return Il puntatore alla struttura ColumnValueDefinition se valido, NULL altrimenti
*/
ColumnValueDefinition parse_column_value_definition(TableDefinition *table, const char *token) {
  ColumnValueDefinition couple;

  memset(&couple, 0, sizeof(ColumnValueDefinition));            // Inizializza la struttura a 0. è importante per evitare valori non inizializzati

  char token_copy[100];                                         // Creiamo una copia per evitare di modificare il token originale
  strncpy(token_copy, token, sizeof(token_copy) - 1);
  token_copy[sizeof(token_copy) - 1] = '\0';                    // Dopo ogni strncopy devo assicurarmi che sia null-terminated


  char *separatore = strchr(token_copy, ':');                   // Cerca il carattere ':'
  if (!separatore) {
    printf("Errore: il campo %s non è definito correttamente.\n", token);
    return couple;
  }

  *separatore = '\0';                                           // Divide la stringa in due parti
  char *nome_colonna = token_copy;                              // La parte prima del separatore è il nome della colonna
  char *valore = separatore + 1;                                // La parte dopo il separatore è il valore del campo


  // Cerco la colonna nella tabella
  ColumnDefinition *col_def = NULL;
  for (int i = 0; i < table->num_colonne; i++) {
    if (strcmp(table->colonne[i].nome_colonna, nome_colonna) == 0) {
      col_def = &table->colonne[i];
      break;
    }
  }

  if (!col_def) { return couple; }

  ColumnType tipo = col_def->tipo;
  if (tipo.name == NULL) { return couple; }                     // Se il tipo è nullo, ritorniamo un oggetto "non valido"
  couple.valore = malloc(tipo.length);                          // Alloco spazio per il valore e lo converto
  if (!couple.valore) { return couple; }                        // Se la memoria non può essere allocata, ritorna un oggetto "non valido"

  if (!tipo.convert(valore, couple.valore)) {                   // Converto il valore
    free(couple.valore);
    couple.valore = NULL;
    return couple;
  }

  couple.campo = col_def;

  return couple;
}



/**
 * Funzione per ottenere il prossimo ID disponibile per una tabella.
 * Come prima cosa, cerca il file binario corrispondente al nome della tabella.
 * Se il file non esiste o è vuoto, ritorno 1, non essendoci record salvati a database.
 * Se il file non è vuoto, mi leggo l'ultimo record del file.
 * Avendo lo schema della tabella in questione, posso facilmente leggere un record intero.
 * Aggiungo 1 all'id dell'ultimo record.
 * 
 * @param table_name: il nome della tabella
 * @return id (int). In caso di errore ritorna -1
 */
int get_next_id_for_table(const char *table_name) {
  char filename[100];
  snprintf(filename, sizeof(filename), "tables/%s.bin", table_name);                // Creo il nome del file relativo alla tabella

  FILE *file = fopen(filename, "rb");
  if (!file) { return 1; }                                                          // Se il file non esiste, ritorno 1

  fseek(file, 0, SEEK_END);                                                         // Se esiste, posiziono il cursore alla fine del file

  long file_size = ftell(file);                                                     // Verifica che il file non sia vuoto
  if (file_size == 0) {
    fclose(file);
    return 1;                                                                       // Se il file è vuoto, il primo ID sarà 1
  }

  size_t record_size = get_record_size(table_name);                                 // Se il file non è vuoto, ottengo la dimensione del record relativo alla tabella
  if (record_size == 0) {
    printf("Errore nella determinazione della dimensione del record\n");
    return -1;
  }

  void* record = create_table_record_struct(table_name);                            // Se il file non è vuoto, creo una struct per la tabella per poter leggere un record
  if (!record) {
    printf("Errore nella creazione del record per la tabella %s\n", table_name);
    return -1;
  }

  size_t last_record_offset = file_size - record_size;                              // Calcolo l'offset dell'ultimo record, così posso andare all'inizio del record
  fseek(file, last_record_offset, SEEK_SET);                                        // Vado all'inizio dell'ultimo record
  fread(record, record_size, 1, file);                                              // Leggo l'ultimo record

  int last_id = *((int*)record);                                                    // Estraggo l'ID dal record (è sempre il primo campo)

  fclose(file);
  free_table_record_struct(record, table_name);                                     // Libero la memoria

  return last_id + 1;                                                               // Ritorno l'ID
}



/**
 * Funzione per ottenere il valore null corrispondente a un tipo di dato.
 * Questo valore mi serve per distinguere quando voglio salvare un campo vuoto.
 * Senza questo metodo, per salvare un numero intero dovrei salvare 0 a database, ma non è sempre corretto come metodo.
 * 
 * @param ColumnType tipo: tipologia di dato, ovviamente parsato correttamente in ColumnType per assicurarci che sia valido.
 * @return Ritorna il puntatore al valore null del tipo di dato.
 */
const void *get_null_value(ColumnType tipo) {
  static const int null_int         = -1;       // Per "int", consideriamo -1 come valore NULL
  static const char null_char[255]  = "";       // Per "char", consideriamo una stringa vuota come valore NULL
  static const float null_float     = -1.0f;    // Per "float", consideriamo -1.0 come valore NULL
  static const double null_double   = -1.0;     // Per "double", consideriamo -1.0 come valore NULL
  static const long null_timestamp  = 0;        // Per "timestamp", consideriamo 0 come valore NULL
  static const bool null_bool       = false;    // Per "bool", consideriamo false come valore NULL

  if (strcmp(tipo.name, "int")       == SUCCESS) return &null_int;
  if (strcmp(tipo.name, "char")      == SUCCESS) return &null_char;
  if (strcmp(tipo.name, "float")     == SUCCESS) return &null_float;
  if (strcmp(tipo.name, "double")    == SUCCESS) return &null_double;
  if (strcmp(tipo.name, "timestamp") == SUCCESS) return &null_timestamp;
  if (strcmp(tipo.name, "bool")      == SUCCESS) return &null_bool;
  return NULL;
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


/**
 * Funzione per ottenere il valore timestamp "now".
 * @return in secondi
 */
long get_current_timestamp() { return time(NULL); }


/**
 * Funzioni che convertono una stringa nelle varie tipologie di dato di sistema
 * Questi metodi sono utili quando devo validare un token <campo>:<valore> per controllare che il valore passato sia effettivamente di quel tipo.
 *
 * @return true o false
 */
bool convert_char_to_int(const char *input, void *output) {
    char *endptr;
    *(int *)output = strtol(input, &endptr, 10);
    return *endptr == '\0';  // Se non ci sono caratteri extra, è valido
}

bool convert_char_to_float(const char *input, void *output) {
    char *endptr;
    *(float *)output = strtof(input, &endptr);
    return *endptr == '\0';
}

bool convert_char_to_double(const char *input, void *output) {
    char *endptr;
    *(double *)output = strtod(input, &endptr);
    return *endptr == '\0';
}

bool convert_char_to_bool(const char *input, void *output) {
    if (strcmp(input, "true") == 0 || strcmp(input, "1") == 0) {
        *(bool *)output = true;
        return true;
    } else if (strcmp(input, "false") == 0 || strcmp(input, "0") == 0) {
        *(bool *)output = false;
        return true;
    }
    return false;  // Se non corrisponde a valori validi, fallisce
}

bool convert_char_to_timestamp(const char *input, void *output) {
    char *endptr;
    *(long *)output = strtol(input, &endptr, 10);
    return *endptr == '\0';
}

bool convert_char_to_string(const char *input, void *output) {
    if (strlen(input) >= 255) { return false; } // La stringa è troppo lunga
    strcpy((char *)output, input);              // Copia diretta in output
    return true;
}
