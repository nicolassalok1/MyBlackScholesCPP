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

# Nettoyage des fichiers de documentation (Doxygen + LaTeX)
doc-clean:
	rm -rf doc
	find . -type f -name "*.aux" -delete
	find . -type f -name "*.log" -delete
	find . -type f -name "*.toc" -delete
	find . -type f -name "*.out" -delete
	find . -type f -name "*.lof" -delete
	find . -type f -name "*.lot" -delete
	find . -type f -name "*.pdf" -delete
	find . -type f -name "*.synctex.gz" -delete
	find . -type f -name "*.bbl" -delete
	find . -type f -name "*.blg" -delete
	find . -type f -name "*_formulas.tex" -delete
	find . -type f -name "*_formulas_dark.tex" -delete

# Lancement du programme
run: $(BIN)
	./$(BIN)

.PHONY: all clean run doc-clean