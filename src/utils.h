#ifndef UTILS_H
#define UTILS_H


#include "../config.h"


int valid_column_type(const char *tipo_colonna);
ColumnDefinition parse_column_definition(const char *token);
ColumnType get_column_type(const char *tipo_colonna);
int verify_is_only_letters(const char *s);


#endif