#ifndef _H_FINITE_DIFFERENCE
#define _H_FINITE_DIFFERENCE

#include <vector>

#include "Mesh.h"
#include "PDE.h"
#include "math.h"
#define THRESHOLD_MIN 1e-8

/**
 * @brief Alias de type pour manipuler plus efficacement des matrices sous forme de double**
 */
typedef double** Matrix;

/**
 * @brief Classe abstraite déclarant les méthodes principales qui seront implémentées 
 */
class FiniteDifference {
        virtual void set_mesh() = 0;
        virtual void set_matrix_coefficients() = 0;
        virtual void set_coefficients_M1() = 0;
        virtual void set_terminal_condition() = 0;
        virtual void compute_solution() = 0;
        virtual void compute_vector_k(int m) = 0;
        virtual void compute_RHS_member(Matrix M, std::vector<double> v) = 0;
        virtual std::vector<double> thomas_algo(std::vector<double> a_, std::vector<double> b_, std::vector<double> c_, std::vector<double> d_) = 0;
        virtual void safe_csv(const char* file_title) = 0;
};

/**
 * @brief Classe implémentant la Méthode Implicite des différences finies
 */
class IMFD : public FiniteDifference {
    public:                // data-members
        ReducedPDE* pde;   // EDP réduite
        int M;             // Nombre d'intervalles de discrétisation temporelle
        int N;             // Nombre d'intervalles de discrétisation spatiale
        double T;          // Largeur de l'intervalle temporel
        double L;          // Largeur de l'intervalle spatial

    public:
        IMFD(ReducedPDE* pde_, int M_, int N_, double L_, double T_);

    public:
        double r;
        double sigma;

        // Discrétisation de l'espace-temps
        Mesh* t;
        Mesh* s;

        // Pas de discrétisation
        double dt;
        double ds;
        double mu;

        // Coefficients des matrices
        std::vector<double> a;
        std::vector<double> b;
        std::vector<double> c;
        std::vector<double> C;
        Matrix M1;

        // Vecteur k pour les conditions initiales
        std::vector<double> k;

        // Vecteur RHS représentant le membre de droite
        std::vector<double> RHS;

    public:
        void set_mesh();
        void set_matrix_coefficients();
        void set_coefficients_M1();
        void set_terminal_condition();
        void compute_solution();
        void compute_vector_k(int m);
        void compute_RHS_member(Matrix M, std::vector<double> v);
        std::vector<double> thomas_algo(std::vector<double> a_, std::vector<double> b_, std::vector<double> c_, std::vector<double> d_);
        void safe_csv(const char* file_title);
};

/**
 * @brief Classe implémentant la méthode de Crank-Nicholson des différences finies
 */
class CrankNicholsonFD : public FiniteDifference {
    public:                 // data-members
        CompletePDE* pde;   // EDP complète
        int M;              // Nombre d'intervalles de discrétisation temporelle
        int N;              // Nombre d'intervalles de discrétisation spatiale
        double T;           // Largeur de l'intervalle temporel
        double L;           // Largeur de l'intervalle spatial

    public:
        CrankNicholsonFD(CompletePDE* pde_, int M_, int N_, double L_, double T_);

    public:
        double r;
        double sigma;

        // Discrétisation de l'espace-temps
        Mesh* t;
        Mesh* s;

        // Pas de discrétisation
        double dt;
        double ds;

        // Coefficients des matrices
        std::vector<double> a;
        std::vector<double> b;
        std::vector<double> c;
        std::vector<double> d;
        std::vector<double> e;   // = - a
        std::vector<double> f;   // = - c
        std::vector<double> C;
        Matrix M1;
        Matrix M2;

        // Vecteur k pour les conditions initiales
        std::vector<double> k;

        // Vecteur RHS représentant le membre de droite
        std::vector<double> RHS;

    public:
        void set_mesh();
        void set_matrix_coefficients();
        void set_coefficients_M1();
        void set_coefficients_M2();
        void set_terminal_condition();
        void compute_solution();
        void compute_vector_k(int m);
        void compute_RHS_member(Matrix M, std::vector<double> v);
        std::vector<double> thomas_algo(std::vector<double> a_, std::vector<double> b_, std::vector<double> c_, std::vector<double> d_);
        void safe_csv(const char* file_title);
};

#endif