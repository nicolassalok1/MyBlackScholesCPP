# Black-Scholes PDE Solver

INSTALLER WSL ET LANCER TOUTES CES COMMANDES DEPUIS UBUNTU DANS LE TERMINAL CLION



Ce projet implémente la résolution numérique de l'équation de Black-Scholes pour options européennes via deux méthodes de différences finies :

- **Méthode implicite (IMFD)** : appliquée à une forme réduite de l'EDP
- **Méthode de Crank-Nicholson (CNFD)** : appliquée à la version complète

## Structure du projet

```
project/
├── include/         # Fichiers d'en-tête (.h)
├── src/             # Fichiers source (.cpp)
├── build/           # Exécutable et fichiers objets (générés automatiquement)
├── font/            # Contient la police arial.ttf pour SDL_ttf
├── Makefile         # Compilation + exécution
├── Doxyfile         # Pour documentation Doxygen
├── README.md        # Ce fichier
└── IMFD_CN_Documentation.tex  # Explication mathématique LaTeX
```

## Dépendances

- `g++` (support C++17)
- `libsdl2-dev`
- `libsdl2-ttf-dev`
- `doxygen` (pour la documentation)
- `pdflatex` (pour compiler le document LaTeX)

### Ubuntu :
```bash
sudo apt install g++ libsdl2-dev libsdl2-ttf-dev doxygen texlive-full
```

## Compilation

```bash
make         # Compilation
make run     # Exécution
make clean   # Nettoyage
```

## Visualisation
Le programme affiche 4 fenêtres :
1. Solutions PUT : IMFD vs CNFD
2. Erreur numérique entre les deux (PUT)
3. Solutions CALL : IMFD vs CNFD
4. Erreur numérique entre les deux (CALL)

## Documentation

- Génération de la documentation Doxygen :
```bash
doxygen Doxyfile
```
- Le résultat est dans le dossier `doc/`

## Auteurs
Projet PAP - Simulation numérique pour options européennes


## Pour générer le pdf :
sudo apt install texlive-full
sudo apt install epstopdf
pdflatex docs/IMFD_CN_Documentation.tex