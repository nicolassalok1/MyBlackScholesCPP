# Makefile pour le projet C++ : résolution numérique de l'équation de Black-Scholes

# Compilateur et options
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 `sdl2-config --cflags` -Iinclude -I/usr/include/SDL2
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf

# Répertoires
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN = $(BUILD_DIR)/simulateur

# Liste des fichiers source
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Cible par défaut
all: $(BIN)

# Édition des liens
$(BIN): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Création du dossier build si nécessaire
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Nettoyage des fichiers intermédiaires
clean:
	rm -rf $(BUILD_DIR)

# Lancement du programme
run: $(BIN)
	./$(BIN)

.PHONY: all clean run