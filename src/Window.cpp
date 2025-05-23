#include "Window.h"
#include <cmath>
#include <algorithm>

int Window::window_id = 0;

Window::Window(double L_, const char* window_title_, int width, int height)
    : L(L_), window_title(window_title_), window_width(width), window_height(height) {
    window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              window_width, window_height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Window::~Window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Window::set_x(const std::vector<double>& x_) {
    x = x_;
}

void Window::set_y1(const std::vector<double>& y_) {
    y1 = y_;
    y_min_1 = *std::min_element(y1.begin(), y1.end());
    y_max_1 = *std::max_element(y1.begin(), y1.end());
}

void Window::set_y2(const std::vector<double>& y_) {
    y2 = y_;
    y_min_2 = *std::min_element(y2.begin(), y2.end());
    y_max_2 = *std::max_element(y2.begin(), y2.end());
}

void Window::set_text(int x, int y, const std::string& txt, int r, int g, int b, int size) {
    labels.push_back({x, y, txt, r, g, b, size});
}

void Window::show() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Fond blanc
    SDL_RenderClear(renderer);

    draw_axes();
    draw_curve(y1, 255, 0, 0);   // Rouge
    draw_curve(y2, 0, 255, 0);   // Vert
    draw_labels();

    SDL_RenderPresent(renderer);
}

void Window::draw_curve(const std::vector<double>& y, int r, int g, int b) {
    if (x.empty() || y.empty()) return;
    double x_scale = window_width / L;
    double y_scale = window_height / (*std::max_element(y.begin(), y.end()) + 1.0);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    for (size_t i = 1; i < y.size(); ++i) {
        int x1 = static_cast<int>(x[i - 1] * x_scale);
        int y1_pix = window_height - static_cast<int>(y[i - 1] * y_scale);
        int x2 = static_cast<int>(x[i] * x_scale);
        int y2_pix = window_height - static_cast<int>(y[i] * y_scale);
        SDL_RenderDrawLine(renderer, x1, y1_pix, x2, y2_pix);
    }
}

void Window::draw_labels() {
    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font) return;

    for (const auto& label : labels) {
        SDL_Color color = {Uint8(label.r), Uint8(label.g), Uint8(label.b)};
        SDL_Surface* surface = TTF_RenderText_Solid(font, label.text.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dstrect = {label.x, label.y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dstrect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    TTF_CloseFont(font);
}

void Window::draw_axes() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // axes en noir
    SDL_RenderDrawLine(renderer, 0, window_height - 1, window_width, window_height - 1);  // axe x
    SDL_RenderDrawLine(renderer, 0, 0, 0, window_height);  // axe y
}
