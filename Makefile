# Nome dell'eseguibile
TARGET = main

# Directory dei file sorgenti
SRC_DIR = src
CMD_DIR = $(SRC_DIR)/commands

# Lista dei file sorgenti
SRC = main.c \
      $(SRC_DIR)/parser.c $(SRC_DIR)/schema.c $(SRC_DIR)/utils.c \
      $(CMD_DIR)/define.c $(CMD_DIR)/create.c $(CMD_DIR)/read.c

# Lista degli oggetti compilati (ogni .c diventa un .o)
OBJ = $(SRC:.c=.o)

# Compilatore
CC = gcc

# Opzioni di compilazione (-I per includere le cartelle corrette)
CFLAGS = -Wall -Wextra -g -I. -I$(SRC_DIR) -I$(CMD_DIR)

# Regola principale: crea l'eseguibile
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Regola per compilare ogni file .c in .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia (rimuove file temporanei)
clean:
	rm -f $(OBJ) $(TARGET)