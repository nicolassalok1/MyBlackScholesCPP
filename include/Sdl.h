#ifndef _H_Sdl
#define _H_Sdl

#include <iostream>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Window.h"

/**
 * @brief Classe Sdl permettant de gérer l'affichage simultané des 4 fenêtres graphiques
 */
class Sdl {
    public:   // Pointeurs sur les différentes fenêtres à afficher
        Window* w1;
        Window* w2;
        Window* w3;
        Window* w4;

    public:
        Sdl();
        void wait_for_close();
        void show();
        void close(Window* w_);
        void quit();
};

#endif