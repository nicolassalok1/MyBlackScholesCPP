#include "Window.h"
#include <algorithm>
// Initialisation
int Window::window_id = 0;

/**
 * @brief Constructeur de la classe Window
 * Permet de créer une instance de la classe Window correspondant à une nouvelle fenêtre
 * @param L_ Nombre d'éléments 
 * @param window_title_  Titre de la fenêtre
 * @param window_width_  Largeur de la fenêtre
 * @param window_height_ Hauteur de la fenêtre
 */
Window::Window(double L_, const char* window_title_, int window_width_, int window_height_) : window_title(window_title_), window_width(window_width_), window_height(window_height_), L(L_) {
        // Création de l'application fenêtre + Paramétrisation
        window = SDL_CreateWindow(
            window_title,              // Titre de la fenêtre
            SDL_WINDOWPOS_UNDEFINED,   // Position initiale en x
            SDL_WINDOWPOS_UNDEFINED,   // Position initiale en y
            window_width,              // Largeur de la fenêtre (en pixel)
            window_height,             // Hauteur de la fenêtre (en pixel)
            SDL_WINDOW_OPENGL |        // Drapeaux (flags)
                SDL_WINDOW_RESIZABLE);

        // Vérification si la fenêtre a bien été créée et paramétrée
        if (window == nullptr) {   // Attention, ne pas utiliser le pointeur NULL
                // La fenêtre n'a pas pu être créée
                printf("Erreur de création de fenêtre : %s\n", SDL_GetError());
                throw "Erreur";
        }

        // Initialisation de la librairie pour la gestion du texte
        TTF_Init();

        // Création d'un Renderer
        renderer = SDL_CreateRenderer(window,
                                      0,
                                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        // Coefficient de dimensionnement du cadre
        alpha = 0.06;

        // Identification automatique de la fenêtre
        window_id++;

        // Suppression des anciennes textures
        clear();
}

/**
 * @brief Destruction de la fenêtre actuelle
 */
Window::~Window() {
        window_id--;
}

/**
 * @brief Fonction supprimant les anciennes inscriptions sur la fenêtre
 */
void Window::clear() {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
}

/**
 * @brief Fonction ajoutant les données des abscisses
 * @param x_  vecteur de double contenant les données d'abscisse
 */
void Window::set_x(std::vector<double> x_) { x = x_; };

/**
 * @brief Fonction ajoutant les données de la première série en ordonnée
 * @param y1_ vecteur de double
 */
void Window::set_y1(std::vector<double> y1_) {
        y1 = y1_;
        y_min_1 = *std::min_element(y1.begin(), y1.end());
        y_max_1 = *std::max_element(y1.begin(), y1.end());
};

/**
 * @brief Fonction ajoutant les données de la deuxième série en ordonnée
 * @param y2_ vecteur de double
 */
void Window::set_y2(std::vector<double> y2_) {
        y2 = y2_;
        y_min_2 = *std::min_element(y2.begin(), y2.end());
        y_max_2 = *std::max_element(y2.begin(), y2.end());
};

/**
 * @brief Fonction map utile pour le rescaling pour plot
 * @param x donnée d'entrée
 * @param in_min borne inférieure du domaine de la valeur d'entrée
 * @param in_max borne supérieure du domaine de la valeur d'entrée
 * @param out_min borne inférieure du domaine de la valeur de sortie
 * @param out_max borne supérieure du domaine de la valeur de sortie
 * @return double 
 */
double Window::map(double x, double in_min, double in_max, double out_min, double out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief Fonction déterminant les frontières du graphique
 * (limites maximales et minimales pour l'axe Y)
 */
void Window::get_graph_boundaries() {
        Y_MIN = std::min(y_min_1, y_min_2);
        if (Y_MIN < THRESHOLD_MIN)
                Y_MIN = 0;
        Y_MAX = std::max(y_max_1, y_max_2);
}

/**
 * @brief Fonction créant les points pour le nuage de points 1
 */
void Window::plot_y1() {
        std::cout << "Affichage de la courbe 1" << std::endl;
        if (x.size() - 2 != y1.size()) {
                std::cout << "Tailles différentes de vecteur" << std::endl;
                throw "mauvaise taille";
        }
        int n_points = y1.size();
        for (int i = 0; i < n_points; i++) {
                int p_x = map(x[i], 0.0, L, alpha * window_width, (1 - alpha) * window_width);
                int p_y = map(y1[i], Y_MIN, Y_MAX, (1 - alpha) * window_height, alpha * window_height);

                // Affichage d'un cercle
                draw_circle(p_x, p_y);
        }
}

/**
 * @brief Fonction créant les points pour le nuage de points 2
 */
void Window::plot_y2() {
        std::cout << "Affichage de la courbe 2" << std::endl;
        if (x.size() - 2 != y2.size()) {
                std::cout << "Tailles différentes de vecteur" << std::endl;
                throw "mauvaise taille";
        }
        int n_points = y2.size();
        for (int i = 0; i < n_points; i++) {
                int p_x = map(x[i], 0.0, L, alpha * window_width, (1 - alpha) * window_width);
                int p_y = map(y2[i], Y_MIN, Y_MAX, (1 - alpha) * window_height, alpha * window_height);

                // Affichage d'un cercle
                draw_circle(p_x, p_y);
        }
}

/**
 * @brief Fonction affichant le cadre du graphique (cadre rectangulaire paramétré grâce au paramètre $\alpha \in \mathbb{R}^+$) 
 */
void Window::plot_frame() {
        // Paramétrage des points du rectangle
        //std::cout << "Affichage du cadre" << std::endl;
        const int n_points = 5;
        static SDL_Point points[n_points] = {
            {alpha * window_width, alpha * window_height},
            {alpha * window_width, (1 - alpha) * window_height},
            {(1 - alpha) * window_width, (1 - alpha) * window_height},
            {(1 - alpha) * window_width, alpha * window_height},
            {alpha * window_width, alpha * window_height},
        };
        // Réglage de la couleur des lignes
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Traçage des objets
        SDL_RenderDrawLines(renderer, points, n_points);
}

/**
 * @brief Fonction affichant un cercle faisant office de points aux coordonnées souhaitées
 * @param n_cx Abscisse du point
 * @param n_cy Ordonnée du point
 * @param radius Rayon du cercle
 */
void Window::draw_circle(int n_cx, int n_cy, int radius) {
        double error = (double)-radius;
        double x = (double)radius - 0.5;
        double y = (double)0.5;
        double cx = n_cx - 0.5;
        double cy = n_cy - 0.5;
        while (x >= y) {
                SDL_RenderDrawPoint(renderer, (int)(cx + x), (int)(cy + y));
                SDL_RenderDrawPoint(renderer, (int)(cx + y), (int)(cy + x));
                if (x != 0) {
                        SDL_RenderDrawPoint(renderer, (int)(cx - x), (int)(cy + y));
                        SDL_RenderDrawPoint(renderer, (int)(cx + y), (int)(cy - x));
                }
                if (y != 0) {
                        SDL_RenderDrawPoint(renderer, (int)(cx + x), (int)(cy - y));
                        SDL_RenderDrawPoint(renderer, (int)(cx - y), (int)(cy + x));
                }
                if (x != 0 && y != 0) {
                        SDL_RenderDrawPoint(renderer, (int)(cx - x), (int)(cy - y));
                        SDL_RenderDrawPoint(renderer, (int)(cx - y), (int)(cy - x));
                }
                error += y;
                ++y;
                error += y;
                if (error >= 0) {
                        --x;
                        error -= x;
                        error -= x;
                }
        }
}

/**
 * @brief Fonction indiquant si les données ont été indiquées ou non
 * @return true si des données ont été soumises pour la courbe 1
 * @return false si aucune donnée n'a été soumise pour la courbe 1
 */
bool Window::has_y1() { return !y1.empty(); }

/**
 * @brief Fonction indiquant si les données ont été indiquées ou non
 * @return true si des données ont été soumises pour la courbe 2
 * @return false si aucune donnée n'a été soumise pour la courbe 2
 */
bool Window::has_y2() { return !y2.empty(); }

/**
 * @brief Fonction réalisant l'affichage des points sur le graphique définitif
 */
void Window::plot() {
        clear();
        plot_frame();
        get_graph_boundaries();
        print_labels();
        if (has_y1() && has_y2()) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                plot_y1();
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
                plot_y2();
        } else if (has_y1() && !has_y2()) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
                plot_y1();
        } else {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
                plot_y2();
        }
}

/**
 * @brief Fonction enregistrant un nouveau label pour l'afficher ultérieurement
 * @param x Abscisse du texte
 * @param y Ordonnée du texte
 * @param text Texte à afficher
 * @param r Niveau de rouge
 * @param g Niveau de vert
 * @param b Niveau de bleu
 * @param size Taille de police utilisé
 */
void Window::set_text(int x, int y, std::string text, int r, int g, int b, int size) {
        Label l;

        // Paramétrage d'un nouveau label
        l.x = x;
        l.y = y;
        l.text = text;
        l.r = r;
        l.g = g;
        l.b = b;
        l.size = size;

        // Ajout d'un label au vecteur
        labels.push_back(l);
}

/**
 * @brief Fonction affichant un label prédéfini
 */
void Window::print_label(Label l) {
        // Déclaration de la couleur et de la police
        SDL_Color col = {l.r, l.g, l.b, 255};
        TTF_Font* font = TTF_OpenFont(FONT_PATH, l.size);

        // Déclaration de la surface de texte
        const char* t = l.text.c_str();
        SDL_Surface* text_surface = TTF_RenderUTF8_Solid(font, t, col);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        // Query de la texture + Rectangle
        int W = 0;
        int H = 0;
        SDL_QueryTexture(texture, NULL, NULL, &W, &H);
        SDL_Rect rect = {l.x, l.y, W, H};

        // Affichage
        SDL_RenderCopy(renderer, texture, NULL, &rect);
}

/**
 * @brief Fonction affichant les labels enregistrés
 */
void Window::print_labels() {
        int s = labels.size();
        for (int i = 0; i < s; i++) {
                //std::cout << "Affichage du label n°" << i + 1 << std::endl;
                print_label(labels[i]);
        }
}