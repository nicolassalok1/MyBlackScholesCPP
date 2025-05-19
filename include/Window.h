#ifndef _H_WINDOW
#define _H_WINDOW

#include <iostream>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#define THRESHOLD_MIN 1e-8
#define POINT_RADIUS 1
#define FONT_PATH "font/arial.ttf"
#define FONT_SIZE 22   // 25

/**
 * @brief Structure Label permettant d'afficher un texte sur une fenêtre graphique
 */
struct Label {
        int x;
        int y;
        std::string text;
        int r;
        int g;
        int b;
        int size;
};

/**
 * @brief Classe Window gérant la création d'une fenêtre d'affichage graphique avec la
 * possibilité d'allouer jusqu'à 2 courbes par fenêtre
 */
class Window {
    private:
        // Paramètres de la fenêtre à afficher
        static int window_id;
        const char* window_title;
        int window_width;
        int window_height;
        float alpha;
        double L;

    public:
        // Variables de fenêtre et de renderer
        SDL_Window* window;
        SDL_Renderer* renderer;

        std::vector<double> x;
        std::vector<double> y1;
        std::vector<double> y2;

        double y_min_1;
        double y_min_2;
        double y_max_1;
        double y_max_2;
        double Y_MIN;
        double Y_MAX;

        // Vecteur contenant les différents textes à afficher sur la fenêtre actuelle
        std::vector<Label> labels;

    public:
        Window(double L_, const char* window_title_ = "Option européenne [PAP]", int window_width_ = 640, int window_height_ = 480);
        ~Window();

        void clear();   // Suppression des anciennes textures sur le graphique

        // Afficheur du cadre
        void plot_frame();
        double map(double x, double in_min, double in_max, double out_min, double out_max);

        // Setters des coordonnées
        void set_x(std::vector<double> x_);     // Ajout des données en abscisse
        void set_y1(std::vector<double> y1_);   // Ajout des données en ordonnées pour la première série de points
        void set_y2(std::vector<double> y2_);   // Ajout des données en ordonnées pour la deuxième série de points
        void get_graph_boundaries();

        // Checkers des données
        bool has_y1();
        bool has_y2();

        // Afficheurs
        void plot_y1();
        void plot_y2();
        void draw_circle(int n_cx, int n_cy, int radius = POINT_RADIUS);
        void plot();

        // Affichage de tous les labels
        void print_labels();
        // Affichage d'un label
        void print_label(Label l);
        // Ajout d'un texte dans la pile des textes
        void set_text(int x, int y, std::string text, int r = 255, int g = 255, int b = 255, int size = FONT_SIZE);
};

#endif