#ifndef _H_MESH
#define _H_MESH

#include <iostream>
#include <vector>

/**
 * @brief Classe représentant un maillage uniforme sur un intervalle [0, a]
 */
class Mesh {
private:
    std::vector<double> data;  ///< Valeurs discrètes
    double a;                  ///< Longueur de l'intervalle
    int size;                  ///< Nombre de subdivisions (points - 1)

public:
    /**
     * @brief Construit un maillage uniforme
     * @param a_ Longueur de l'intervalle
     * @param size_ Nombre de subdivisions (M ⇒ M+1 points)
     */
    Mesh(double a_, int size_);

    double get_interval() const { return a; }
    int get_size() const { return size; }

    /**
     * @brief Retourne le pas de discrétisation h = a / size
     */
    double get_step() const { return a / size; }

    /**
     * @brief Retourne toutes les valeurs sous forme de std::vector
     */
    std::vector<double> get_vector() const { return data; }

    /**
     * @brief Accès direct aux points via operator[]
     */
    double operator[](int i) const;

    /**
     * @brief Affichage du maillage
     */
    friend std::ostream& operator<<(std::ostream& os, const Mesh& m);
};

#endif
