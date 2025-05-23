#include <iostream>
#include <memory>    // Pour std::unique_ptr
#include <vector>
#include <cmath>

#include "FiniteDifference.h"
#include "Mesh.h"
#include "Option.h"
#include "PDE.h"
#include "Payoff.h"
#include "Sdl.h"
#include "functions.h"

/**
 * @brief Point d'entrée du programme
 * Ce programme résout numériquement l'équation de Black-Scholes pour des options put/call
 * en utilisant deux schémas : implicite (IMFD) et Crank-Nicholson (CNFD).
 */
int main() {
    std::cout << "-------------------------------------------------------\n"
              << "\n\tProjet PAP - Equation de Black et Scholes\n\n"
              << "\t Nicolas Pelletier\n"
              << "-------------------------------------------------------\n";

    // --- Paramètres de discrétisation et du modèle ---
    const int M = 1000, N = 1000;  // Maillage (espace x temps)
    const double T = 1.0, L = 300.0;
    const double K = 100.0, r = 0.1, sigma = 0.1;

    // --- Initialisation des options et payoffs ---
    auto payoff_put = std::make_unique<Put>(K);
    auto payoff_call = std::make_unique<Call>(K);

    auto option_put = std::make_unique<Option>(payoff_put.get(), T, r, sigma, K, L);
    auto option_call = std::make_unique<Option>(payoff_call.get(), T, r, sigma, K, L);

    // --- Construction des EDPs associées ---
    auto pde_r_put = std::make_unique<ReducedPDE>(option_put.get());
    auto pde_c_put = std::make_unique<CompletePDE>(option_put.get());
    auto pde_r_call = std::make_unique<ReducedPDE>(option_call.get());
    auto pde_c_call = std::make_unique<CompletePDE>(option_call.get());

    // --- Résolution numérique ---
    IMFD imfd_put(pde_r_put.get(), M, N, L, T);
    CrankNicholsonFD cnfd_put(pde_c_put.get(), M, N, L, T);
    IMFD imfd_call(pde_r_call.get(), M, N, L, T);
    CrankNicholsonFD cnfd_call(pde_c_call.get(), M, N, L, T);

    imfd_put.compute_solution();
    imfd_call.compute_solution();
    cnfd_put.compute_solution();
    cnfd_call.compute_solution();

    // --- Calcul des écarts entre solutions ---
    std::vector<double> diff_put = compute_diff_vector(imfd_put.C, cnfd_put.C);
    std::vector<double> diff_call = compute_diff_vector(imfd_call.C, cnfd_call.C);

    // --- Visualisation graphique ---
    auto gui = std::make_unique<Sdl>();

    gui->w1 = new Window(L, "PUT - Superposition des solutions");
    gui->w2 = new Window(L, "PUT - Erreur numérique entre les 2 méthodes");
    gui->w3 = new Window(L, "CALL - Superposition des solutions");
    gui->w4 = new Window(L, "CALL - Erreur numérique entre les 2 méthodes");

    // Affichage PUT - solutions
    gui->w1->set_x(imfd_put.s->get_vector());
    gui->w1->set_y1(cnfd_put.C); // CNFD
    gui->w1->set_y2(imfd_put.C); // IMFD

    // Texte descriptif PUT
    gui->w1->set_text(450, 40, "EDP complète", 255, 0, 0);
    gui->w1->set_text(300, 40, "EDP réduite", 0, 255, 0);

    // Erreur PUT
    gui->w2->set_x(imfd_put.s->get_vector());
    gui->w2->set_y2(diff_put);
    gui->w2->set_text(410, 40, "Erreur numérique", 255, 255, 0);

    // Affichage CALL - solutions
    gui->w3->set_x(imfd_call.s->get_vector());
    gui->w3->set_y1(cnfd_call.C); // CNFD
    gui->w3->set_y2(imfd_call.C); // IMFD

    // Texte descriptif CALL
    gui->w3->set_text(100, 40, "EDP complète", 255, 0, 0);
    gui->w3->set_text(300, 40, "EDP réduite", 0, 255, 0);

    // Erreur CALL
    gui->w4->set_x(imfd_call.s->get_vector());
    gui->w4->set_y2(diff_call);
    gui->w4->set_text(45, 40, "Erreur numérique", 255, 255, 0);

    // Lancement de la visualisation SDL
    gui->show();
    gui->quit();

    return 0;
}
