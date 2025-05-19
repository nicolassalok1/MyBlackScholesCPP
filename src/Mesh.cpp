#include "Mesh.h"

#include <stdexcept>

/**
 * @brief Construction d'un objet Mesh (création d'une discrétisation)
 * 
 * @param a_ 
 * @param size_ 
 */
Mesh::Mesh(double a_, int size_) {
        if (size_ <= 0)
                throw std::invalid_argument("Taille invalide");
        // Initialisation des paramètres
        size = size_ + 1;   // M intervalles ==> M + 1 bornes
        a = a_;
        data = new double[size];

        // Vérification de l'allocation mémoire
        if (!data)
                throw "Echec de l'allocation de mémoire";

        // Remplissage du tableau avec la discrétisation souhaitée
        for (int i = 0; i < size; i++)
                data[i] = i * ((double)a / (double)size);

        // Message de statut
        //std::cout << "Discrétisation ok" << std::endl;
}

/**
 * @brief Destructeur de la classe Mesh
 */
Mesh::~Mesh() {
        // Suppression du tableau de données
        delete[] data;
}

/**
 * @brief Surcharge de l'opérateur [] (getter par index)
 * @param i Indice de l'objet à retourner
 * @return double
 */
double Mesh::operator[](int i) const {
        if ((i < 0) || (i > size))
                throw std::invalid_argument("Index invalide");
        return data[i];
}

/**
 * @brief Surcharge de l'opérateur <<
 * @param st Flux de sortie standard
 * @param m Objet de Mesh (discrétisation)
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& st, const Mesh& m) {
        st << "[";
        for (int i = 0; i < m.get_size(); i++) {
                st << m[i];
                if (i != (m.get_size() - 1))
                        st << ";";
        }
        st << "]";
        return st;
}

/**
 * @brief Fonction retournant une version std::vector<double> de la discrétisation
 * @return std::vector<double> 
 */
std::vector<double> Mesh::get_vector() {
        int n = size;
        std::vector<double> res(n + 1, 0.0);
        for (int i = 0; i < n; i++)
                res[i] = data[i];
        return res;
}