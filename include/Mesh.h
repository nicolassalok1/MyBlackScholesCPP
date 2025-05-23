#ifndef _H_MESH
#define _H_MESH

#include <iostream>
#include <vector>

/**
 * @brief Classe Mesh permettant de créer une discrétisation d'un domaine spatial ou temporel
 */
class Mesh {
    private:
        double* data;   // Tableau contenant la discrétisation temporelle
        double a;       // Largeur de l'intervalle
        int size;       // Nombre de subdivisions
    public:
        Mesh(double a_, int size_);
        ~Mesh();

    public:
        double get_interval() const { return a; }
        int get_size() const { return size; }
        double get_step() const { return (double)(a / size); }
        std::vector<double> get_vector();

    public:
        double operator[](int i) const;

    public:
        friend std::ostream& operator<<(std::ostream& st, const Mesh& m);
};

#endif