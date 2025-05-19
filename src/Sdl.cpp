#include "Sdl.h"

/**
 * @brief Constructeur de la classe Sdl
 */
Sdl::Sdl() {
        // Initialisation de la librairie + Vérification
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("L'initialisation a échoué.");
        }
        // Initialisation de la librairie pour la gestion du texte
        TTF_Init();

        // Les 4 fenêtres sont intialisées au pointeur nul (== aucune fenêtre)
}

/**
 * @brief Fonction faisant office de waiter (attente) avant la fermeture des fenêtres
 * (programmation évènementielle)
 */
void Sdl::wait_for_close() {
        SDL_Event e;

        bool displayed = true;
        while (displayed) {
                // En attente d'un event
                while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
                                //On ne peut pas faire de switch ici car l'évaluation de fonction n'est pas autorisée dans les expressions constantes
                                if (SDL_GetWindowID(w1->window) == e.window.windowID) {
                                        close(w1);
                                }
                                if (SDL_GetWindowID(w2->window) == e.window.windowID) {
                                        close(w2);
                                }
                                if (SDL_GetWindowID(w3->window) == e.window.windowID) {
                                        close(w3);
                                }
                                if (SDL_GetWindowID(w4->window) == e.window.windowID) {
                                        close(w4);
                                }
                        }
                        // Si l'évènement correspond à une demande de fermeture d'application par l'utilisateur
                        if (e.type == SDL_QUIT)
                                displayed = false;
                }
        }
}

/**
 * @brief Fonction affichant les différentes fenêtres et les données correspondantes
 */
void Sdl::show() {
        w1->plot();
        SDL_RenderPresent(w1->renderer);

        w2->plot();
        SDL_RenderPresent(w2->renderer);

        w3->plot();
        SDL_RenderPresent(w3->renderer);

        w4->plot();
        SDL_RenderPresent(w4->renderer);

        // Attente de fermeture de la fenêtre
        wait_for_close();
}

/**
 * @brief Fonction fermant la fenêtre en question
 * @param w_ Pointeur d'une fenêtre
 */
void Sdl::close(Window* w_) {
        // Destruction de la fenêtre
        SDL_DestroyWindow(w_->window);

        // Destruction du renderer
        SDL_DestroyRenderer(w_->renderer);
}

/**
 * @brief Fonction pour quitter le procesus d'affichage
 */
void Sdl::quit() {
        TTF_Quit();
        SDL_Quit();
        std::cout << "Fermeture de tous les affichages" << std::endl;
}