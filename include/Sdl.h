#ifndef _H_Sdl
#define _H_Sdl

#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Window.h"

/**
 * @brief Classe gérant 4 fenêtres SDL pour visualiser les résultats
 */
class Sdl {
public:
    Window* w1 = nullptr;
    Window* w2 = nullptr;
    Window* w3 = nullptr;
    Window* w4 = nullptr;

    Sdl();
    void show();               ///< Affiche les 4 fenêtres
    void wait_for_close();     ///< Boucle d'attente d'événements SDL
    void close(Window* w);     ///< Ferme une fenêtre spécifique
    void quit();               ///< Ferme SDL et libère la mémoire
};

#endif
