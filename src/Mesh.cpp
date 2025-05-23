#include "Mesh.h"
#include <stdexcept>

/**
 * @brief Construit un maillage [0, a] avec size + 1 points
 */
Mesh::Mesh(double a_, int size_) {
        if (size_ <= 0)
                throw std::invalid_argument("Taille invalide pour le maillage.");

        a = a_;
        size = size_;
        data.resize(size + 1);

        double step = a / size;
        for (int i = 0; i <= size; ++i)
                data[i] = i * step;
}

/**
 * @brief Accès à la i-ème valeur du maillage
 */
double Mesh::operator[](int i) const {
        if (i < 0 || i > size)
                throw std::out_of_range("Index hors limites dans Mesh.");
        return data[i];
}

/**
 * @brief Affichage du maillage
 */
std::ostream& operator<<(std::ostream& os, const Mesh& m) {
        for (int i = 0; i <= m.size; ++i)
                os << m.data[i] << " ";
        return os;
}
