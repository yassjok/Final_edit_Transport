# Nom de l'exécutable
EXEC = projet_transport

# Dossiers
SRC_DIR = source
INC_DIR = include

# Fichiers source
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/transport.c $(SRC_DIR)/itineraires.c

# Fichiers objets
OBJ = $(SRC:.c=.o)

# Options de compilation
CFLAGS = -Wall -Wextra -std=c99 -I$(INC_DIR)

# Compilation par défaut
$(EXEC): $(OBJ)
	@gcc $(CFLAGS) -o $(EXEC) $(OBJ)
	@echo "Compilation terminée avec succès !"

# Compilation des fichiers .c en .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@gcc $(CFLAGS) -c $< -o $@
	@echo "Compilation de $< terminée."

# Nettoyage des fichiers objets et de l'exécutable
clean:
	@rm -f $(OBJ) $(EXEC)
	@echo "Nettoyage terminé."

# Nettoyage complet
clean_all: clean
	@rm -f *.csv
	@echo " Nettoyage complet terminé."

# Règles spéciales
.PHONY: clean clean_all
