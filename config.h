#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <pthread.h>                            // Per il mutex multithreading. Questo permette di bloccare l'accesso a una risorsa condivisa tra più thread.

/**
 * Questo file contiene tutte le strutture dati utilizzate nel nostro sistema.
 * Possiamo considerarlo un file di configurazione.
 * Qui dentro troverai tutte le variabili globali, costanti e strutture dati di cui avrai bisogno.
 */

#define VERSIONE 3                              // Versione del programma

// BOOLEANS
#define SUCCESS 0                               // Codice di successo
#define FAILURE 1                               // Codice di errore
#define TRUE 1                                  // Costante per il vero
#define FALSE 0                                 // Costante per il falso

#define MAX_INPUT_SIZE 256                      // Non puoi scrivere comandi più lunghi di 256 caratteri.

#define MAX_TOKENS      20                      // Numero massimo di parole che può contenere un comando

#define DEFINE_INIT_TOKENS      2               // Numero di token iniziali per il comando DEFINE



#define MAX_TABLES      100                     // Numero massimo di tabelle che possono essere definite
#define MAX_FIELDS      10                      // Numero massimo di campi che può contenere una tabella
#define TABLES_DIR      "tables/"               // Cartella in cui verranno salvate le tabelle
#define SCHEMA_FILE     "schema.bin"            // File in cui verranno salvate le definizioni delle tabelle


typedef enum {                                  // Lista di tutti i comandi supportati dal nostro sistema
  CMD_DEFINE,
  CMD_READ,
  CMD_CREATE,
  CMD_UPDATE,
  CMD_SEARCH,
  CMD_DELETE,
  CMD_UNKNOWN
} CommandType;

typedef struct {                                // Questa struct mi serve per specificare il nome, il tipo e la lunghezza di una colonna
  char nome_colonna[50];
  char tipo[10];                                // Ad ora si possono inserire solo campi "char" o "int"
  int lunghezza;                                // Se "char", avrà un valore (es. 100). Se "int", sarà 0.
} ColumnDefinition;

typedef struct {                                // Questa struct mi serve per specificare quante colonne ha una tabella
  char nome_tabella[50];
  int num_colonne;
  ColumnDefinition colonne[MAX_FIELDS];         // Array di colonne 
} TableDefinition;

typedef struct {
  TableDefinition tabelle[MAX_TABLES];          // Array di tabelle
  int num_tabelle;                              // Numero di tabelle
  pthread_mutex_t mutex;                        // Mutex per proteggere l'accesso
} Schema;


#endif