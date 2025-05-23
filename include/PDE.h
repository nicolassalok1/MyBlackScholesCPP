#ifndef _H_PDE
#define _H_PDE

#include "Option.h"

/**
 * @brief Classe abstraite numérisant une EDP à partir d'une option
 * 
 */
class PDE {
    protected:
        Option* option;

    public:
        virtual double get_coeff_a() const = 0;
        virtual double get_cdt_bord_b(double t) const = 0;
        virtual double get_cdt_bord_h(double t, double s) const = 0;
        virtual double get_cdt_term(double s) const = 0;
};

/**
 * @brief Classe CompletePDE numérisant l'EDP de Black-Scholes complète, ses coefficients et ses conditions aux bords
 */
class CompletePDE : public PDE {
    private:
        Option* option;

    public:
        CompletePDE(Option* option_) : option(option_){};
        ~CompletePDE();
        Option* get_option() const { return option; }

    public:
        // Getters pour les coefficients
        double get_coeff_a() const;
        double get_coeff_b(double s) const;
        double get_coeff_c(double s) const;
        double get_coeff_d() const;

        // Getters pour les conditions aux bords
        double get_cdt_bord_b(double t) const;
        double get_cdt_bord_h(double t, double s) const;
        double get_cdt_term(double s) const;
};

/**
 * @brief Classe ReducedPDE numérisant l'EDP de Black-Scholes réduite, ses coefficients et ses conditions aux bords
 */
class ReducedPDE : public PDE {
    private:
        Option* option;

    public:
        ReducedPDE(Option* option_) : option(option_){};
        ~ReducedPDE();
        Option* get_option() const { return option; }

    public:
        // Getters pour les coefficients
        double get_coeff_a() const;
        double get_coeff_b() const;

        // Getters pour les conditions aux bords
        double get_cdt_bord_b(double t) const;
        double get_cdt_bord_h(double t, double s) const;
        double get_cdt_term(double s) const;
};

#endif