#ifndef DEFINE_H
#define DEFINE_H

#include "../../config.h"




void execute_define(char *tokens[], int token_count);
int validate_define(char *tokens[], int token_count);
int validate_define_column_format_token(char *column);



#endif