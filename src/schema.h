#ifndef SCHEMA_H
#define SCHEMA_H

// Config Header
#include "../config.h"

// Global Variable
extern Schema schema;

// Functions Available including the Schema
int load_schema();
TableDefinition* get_table_from_schema(const char* table_name);
int add_table_to_schema(TableDefinition* new_table);
int remove_table_from_schema(const char* table_name);
void print_schema();
int write_schema_to_file();

void* create_table_record_struct(const char* table_name);
void free_table_record_struct(void* record, const char* table_name);
size_t get_record_size(const char* table_name);

#endif