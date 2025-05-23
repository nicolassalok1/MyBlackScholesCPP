#include <fstream>
#include <iostream>
#include <vector>

#include "FiniteDifference.h"
#include "Mesh.h"
#include "Option.h"
#include "PDE.h"
#include "Payoff.h"
#include "Sdl.h"
#include "functions.h"
#include "math.h"

/**
 * @brief Fonction principale du programme permettant de réaliser les tests
 * @return int
 */
int main() {
        std::cout << "-------------------------------------------------------\n"
                  << "\n\tProjet PAP - Equation de Black et Scholes\n\n"
                  << "\n\t Lucas RODRIGUEZ\n"
                  << "-------------------------------------------------------"
                  << std::endl;

        // Paramètres de discrétisation
        const int M = 1000;
        const int N = 1000;
        // Paramètre espace-temps
        double T = 1.0;
        double L = 300.0;
        // Paramètre de l'EDP de Black-Scholes
        double K = 100.0;
        double r = 0.1;
        double sigma = 0.1;

        // Déclaration du payoff utilisé et de l'option
        Payoff *payoff_put = new Put(K);
        Payoff *payoff_call = new Call(K);
        Option *option_put = new Option(payoff_put, T, r, sigma, K, L);
        Option *option_call = new Option(payoff_call, T, r, sigma, K, L);

        // Déclaration des EDP réduite et complète à simuler
        ReducedPDE *pde_r_put = new ReducedPDE(option_put);
        CompletePDE *pde_c_put = new CompletePDE(option_put);
        ReducedPDE *pde_r_call = new ReducedPDE(option_call);
        CompletePDE *pde_c_call = new CompletePDE(option_call);

        // Déclaration des résolveurs via différences finies pour le PUT
        IMFD imfd_put(pde_r_put, M, N, L, T);
        CrankNicholsonFD cnfd_put(pde_c_put, M, N, L, T);
        // Déclaration des résolveurs via différences finies pour le CALL
        IMFD imfd_call(pde_r_call, M, N, L, T);
        CrankNicholsonFD cnfd_call(pde_c_call, M, N, L, T);

        // Calcul des solutions respectives
        imfd_call.compute_solution();
        imfd_put.compute_solution();
        cnfd_call.compute_solution();
        cnfd_put.compute_solution();

        // Sauvegarde des données produites pour chacune des solutions numériques
        /*
        imfd_call.safe_csv("data_imfd_call.csv");
        imfd_put.safe_csv("data_imfd_put.csv");
        cnfd_call.safe_csv("data_cnfd_call.csv");
        cnfd_put.safe_csv("data_cnfd_put.csv");
        */

        // Détermination des vecteurs de différences
        std::vector<double> diff_put = compute_diff_vector(imfd_put.C, cnfd_put.C);
        std::vector<double> diff_call = compute_diff_vector(imfd_call.C, cnfd_call.C);

        // Affichage graphique
        Sdl *g = new Sdl();
        g->w1 = new Window(L, "PUT - Superposition des solutions");
        g->w2 = new Window(L, "PUT - Erreur numérique entre les 2 méthodes");
        g->w3 = new Window(L, "CALL - Superposition des solutions");
        g->w4 = new Window(L, "CALL - Erreur numérique entre les 2 méthodes");

        // Déclaration des données pour le graphique 1 -- Méthodes des différences finies pour le PUT
        g->w1->set_x(imfd_put.s->get_vector());
        g->w1->set_y1(cnfd_put.C);   // Rouge
        g->w1->set_text(450, 40, "EDP complète", 255, 0, 0);
        g->w1->set_text(450, 70, "MIN = " + std::to_string(g->w1->y_min_1), 255, 0, 0, 11);
        g->w1->set_text(450, 80, "MAX = " + std::to_string(g->w1->y_max_1), 255, 0, 0, 11);
        g->w1->set_y2(imfd_put.C);   // Vert
        g->w1->set_text(300, 40, "EDP réduite", 0, 255, 0);
        g->w1->set_text(300, 70, "MIN = " + std::to_string(g->w1->y_min_2), 0, 255, 0, 11);
        g->w1->set_text(300, 80, "MAX = " + std::to_string(g->w1->y_max_2), 0, 255, 0, 11);

        // Déclaration des données pour le graphique 2 -- Erreur numérique sur les méthodes pour le PUT
        g->w2->set_x(imfd_put.s->get_vector());
        g->w2->set_y2(diff_put);
        g->w2->set_text(410, 40, "Erreur numérique", 255, 255, 0);
        g->w2->set_text(410, 70, "MIN = " + std::to_string(g->w2->y_min_2), 255, 255, 0, 11);
        g->w2->set_text(410, 80, "MAX = " + std::to_string(g->w2->y_max_2), 255, 255, 0, 11);

        // Déclaration des données pour le graphique 3 -- Méthodes des différences finies pour le CALL
        g->w3->set_x(imfd_call.s->get_vector());
        g->w3->set_y1(cnfd_call.C);
        g->w3->set_text(100, 40, "EDP complète", 255, 0, 0);
        g->w3->set_text(100, 70, "MIN = " + std::to_string(g->w3->y_min_1), 255, 0, 0, 11);
        g->w3->set_text(100, 80, "MAX = " + std::to_string(g->w3->y_max_1), 255, 0, 0, 11);
        g->w3->set_y2(imfd_call.C);
        g->w3->set_text(300, 40, "EDP réduite", 0, 255, 0);
        g->w3->set_text(300, 70, "MIN = " + std::to_string(g->w3->y_min_2), 0, 255, 0, 11);
        g->w3->set_text(300, 80, "MAX = " + std::to_string(g->w3->y_max_2), 0, 255, 0, 11);

        // Déclaration des données pour le graphique 4 -- Erreur numérique sur les méthodes pour le CALL
        g->w4->set_x(imfd_call.s->get_vector());
        g->w4->set_y2(diff_call);
        g->w4->set_text(45, 40, "Erreur numérique", 255, 255, 0);
        g->w4->set_text(45, 70, "MIN = " + std::to_string(g->w4->y_min_2), 255, 255, 0, 11);
        g->w4->set_text(45, 80, "MAX = " + std::to_string(g->w4->y_max_2), 255, 255, 0, 11);

        // Fermeture & Destruction des affichages
        g->show();
        g->quit();
        return 0;
}