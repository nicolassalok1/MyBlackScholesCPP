#ifndef _H_WINDOW
#define _H_WINDOW

#include <iostream>
#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#define FONT_PATH "font/arial.ttf"
#define FONT_SIZE 22
#define POINT_RADIUS 1

struct Label {
    int x, y;
    std::string text;
    int r, g, b;
    int size;
};

/**
 * @brief Fenêtre SDL permettant l'affichage de deux courbes
 */
class Window {
private:
    static int window_id;
    const char* window_title;
    int window_width;
    int window_height;
    double L;

public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    std::vector<double> x;
    std::vector<double> y1;
    std::vector<double> y2;

    std::vector<Label> labels;

    double y_min_1 = 0.0;
    double y_max_1 = 0.0;
    double y_min_2 = 0.0;
    double y_max_2 = 0.0;

    Window(double L_, const char* window_title_, int width = 800, int height = 600);
    ~Window();

    void set_x(const std::vector<double>& x_);
    void set_y1(const std::vector<double>& y_);
    void set_y2(const std::vector<double>& y_);
    void set_text(int x, int y, const std::string& txt, int r, int g, int b, int size = FONT_SIZE);

    void show();

private:
    void draw_curve(const std::vector<double>& y, int r, int g, int b);
    void draw_labels();
    void draw_axes();
};

#endif
