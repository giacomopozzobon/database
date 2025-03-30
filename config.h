#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
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
#define CREATE_INIT_TOKENS      2               // Numero di token iniziali per il comando DEFINE


#define MAX_TABLES      100                     // Numero massimo di tabelle che possono essere definite
#define MAX_FIELDS      10                      // Numero massimo di campi che può contenere una tabella
#define TABLES_DIR      "./tables"               // Cartella in cui verranno salvate le tabelle
#define SCHEMA_FILE     "schema.bin"            // File in cui verranno salvate le definizioni delle tabelle


typedef enum {                                  // Lista di tutti i comandi supportati dal nostro sistema
  CMD_INFO,
  CMD_SCHEMA,
  CMD_DEFINE,
  CMD_CREATE,
  CMD_READ,
  CMD_UPDATE,
  CMD_FIND,
  CMD_DELETE,
  CMD_UNKNOWN
} CommandType;

typedef bool (*ConvertFunc)(const char *input, void *output);

/** 
 * Attenzione qui: Le Struct qui sotto sono le principali strutture che compongono gli oggetti del sistema
 * Bisogna fare attenzione a come vengono definite soprattutto quelle struct che vengono usate per scrivere dei dati sui file.
 * Struct che vengono usate per questo scopo infatti devono contenere solo campi di tipo primitivo, e non puntatori.
 * 
 * Infatti, se scrivessi per sbaglio un puntatore in un file, sarebbe un problema, in quanto all'avvio successivo del programma,
 * il puntatore che ho scritto non avrebbe più senso visto che la memoria si è liberata.
 * */

typedef struct {                                // ColumnType: struct per definire il tipo di campo
  char name[50];                                // name: ad esempio "int" o "char"
  int length;                                   // length: lunghezza
  ConvertFunc convert;                          // Funzione che converte una stringa nel tipo relativo
} ColumnType;

typedef struct {                                // ColumnDefinition: struct per definire un campo di una tabella
  char nome_colonna[50];                        // nome_colonna: ad esempio "nome"
  ColumnType tipo;                              // tipo: ColumnType
} ColumnDefinition;

typedef struct {                                // ColumnValueDefinition: struct per definire una coppia <campo>:<valore>
  ColumnDefinition campo;                       // nome_colonna: ad esempio: "nome"
  void *valore;                                 // valore: valore del campo (puntatore perchè può essere di diverso tipo)
} ColumnValueDefinition;

typedef struct {                                // TableDefinition: struct per definire una tabella
  char nome_tabella[50];                        // nome_tabella: ad esempio "Utenti"
  int num_colonne;                              // num_colonne: indica quanti campi ha
  ColumnDefinition colonne[MAX_FIELDS];         // colonne: array di ColumnDefinition
} TableDefinition;

typedef struct {                                // Schema: struct per definire lo schema delle tabelle
  TableDefinition tabelle[MAX_TABLES];          // Array di TableDefinition
  int num_tabelle;                              // Numero di tabelle
  pthread_mutex_t mutex;                        // Mutex per proteggere l'accesso
} Schema;


#endif