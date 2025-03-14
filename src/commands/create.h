#ifndef CREATE_H
#define CREATE_H

// Config Header
#include "../../config.h"


// Functions Available including the Define
void execute_create(char *tokens[], int token_count);
int validate_create(char *tokens[], int token_count);

void create_tables_directory_if_not_exists();



#endif