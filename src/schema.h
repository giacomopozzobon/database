#ifndef SCHEMA_H
#define SCHEMA_H

#include "../config.h"

extern Schema schema;

int load_schema();
TableDefinition* get_table_from_schema(const char* table_name);
int add_table_to_schema(TableDefinition* new_table);
int remove_table_from_schema(const char* table_name);
void print_schema();
int write_schema_to_file();


#endif