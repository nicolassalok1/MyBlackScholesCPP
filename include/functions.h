#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>

/**
 * @brief Affiche les éléments d’un vecteur sur la sortie standard
 * @param v Vecteur de réels à afficher
 */
void print_vector(const std::vector<double>& v);

/**
 * @brief Calcule la différence absolue élément par élément entre deux vecteurs
 * @param a Premier vecteur
 * @param b Deuxième vecteur
 * @return Vecteur des différences absolues |a_i - b_i|
 * @throws std::invalid_argument si les tailles diffèrent
 */
std::vector<double> compute_diff_vector(const std::vector<double>& a, const std::vector<double>& b);

#endif
