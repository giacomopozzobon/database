#ifndef UTILS_H
#define UTILS_H

// Config Header
#include "../config.h"


// Functions Available including the Utils
bool convert_char_to_int(const char *input, void *output);
bool convert_char_to_float(const char *input, void *output);
bool convert_char_to_double(const char *input, void *output);
bool convert_char_to_bool(const char *input, void *output);
bool convert_char_to_timestamp(const char *input, void *output);
bool convert_char_to_string(const char *input, void *output);

ColumnDefinition parse_column_definition(const char *token);
ColumnType parse_column_type(const char *tipo_colonna);
ColumnValueDefinition parse_column_value_definition(TableDefinition *table, const char *token);

int get_next_id_for_table(const char *table_name);
const void *get_null_value(ColumnType tipo);

int split_token(const char *token, char separatore, char *prima, char *dopo);

int verify_is_only_letters(const char *s);
long get_current_timestamp();

void fix_conversion_functions();
FILE* open_table_file(const char* table_name, const char* mode);


#endif