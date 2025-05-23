#include "Sdl.h"

Sdl::Sdl() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("Erreur SDL_Init : %s\n", SDL_GetError());
        }
        TTF_Init();
}

void Sdl::show() {
        if (w1) w1->show();
        if (w2) w2->show();
        if (w3) w3->show();
        if (w4) w4->show();
        wait_for_close();
}

void Sdl::wait_for_close() {
        SDL_Event e;
        bool displayed = true;
        while (displayed) {
                while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
                                displayed = false;
                        }
                }
        }
}

void Sdl::close(Window* w) {
        if (w) delete w;
}

void Sdl::quit() {
        if (w1) close(w1);
        if (w2) close(w2);
        if (w3) close(w3);
        if (w4) close(w4);
        SDL_Quit();
        TTF_Quit();
}
