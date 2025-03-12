#ifndef PARSER_H
#define PARSER_H

#include "../config.h"



CommandType get_command_type(char *command);
void process_command(char *input);

#endif