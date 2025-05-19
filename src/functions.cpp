#include "functions.h"

/**
 * @brief Fonction affichant sur la sortie standard le contenu d'un std::vector<double>
 * @param v : std::vector<double> à afficher
 */
void print_vector(std::vector<double> v) {
        int size = v.size();
        for (int i = 0; i < size; i++)
                std::cout << v[i] << ' ';
        std::cout << "fin" << std::endl;
}

/**
 * @brief Fonction calculant et retournant le vecteur de différences entre 2 vecteurs de doubles
 * - Utilisation de la fonction de valeur absolue
 * - Utilisée pour calculer puis afficher l'erreur entre la résolution de l'EDP complète et de l'EDP réduite
 * @param a vecteur de double
 * @param b vecteur de double
 * @return std::vector<double> 
 */
std::vector<double> compute_diff_vector(std::vector<double> a, std::vector<double> b) {
        //std::cout << "size a : " << a.size() << "\t b : " << b.size() << std::endl;
        if (a.size() != b.size())
                throw "Taille invalide";
        int n = b.size();
        std::vector<double> diff(n, 0.0);   // vecteur de différence
        for (int i = 0; i < n; i++)
                diff[i] = abs(a[i] - b[i]);
        return diff;
}