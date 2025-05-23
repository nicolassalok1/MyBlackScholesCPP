#ifndef _H_PDE
#define _H_PDE

#include "Option.h"

/**
 * @brief Classe abstraite représentant une équation aux dérivées partielles
 *
 * Elle définit une interface commune pour les EDP réduites et complètes.
 */
class PDE {
protected:
    Option* option;  ///< Paramètres de l'option

public:
    virtual ~PDE() = default;

    // Coefficient principal (souvent constant)
    virtual double get_coeff_a() const = 0;

    // Conditions aux bords
    virtual double get_cdt_bord_b(double t) const = 0;
    virtual double get_cdt_bord_h(double t, double s) const = 0;

    // Condition terminale
    virtual double get_cdt_term(double s) const = 0;
};

/**
 * @brief Implémentation de l’EDP de Black-Scholes complète
 */
class CompletePDE : public PDE {
public:
    explicit CompletePDE(Option* option_) { option = option_; }
    ~CompletePDE() = default;

    Option* get_option() const { return option; }

    // Coefficients de l'EDP
    double get_coeff_a() const override;
    double get_coeff_b(double s) const;
    double get_coeff_c(double s) const;
    double get_coeff_d() const;

    // Conditions aux bords et terminale
    double get_cdt_bord_b(double t) const override;
    double get_cdt_bord_h(double t, double s) const override;
    double get_cdt_term(double s) const override;
};

/**
 * @brief Implémentation de l’EDP réduite de Black-Scholes
 */
class ReducedPDE : public PDE {
public:
    explicit ReducedPDE(Option* option_) { option = option_; }
    ~ReducedPDE() = default;

    Option* get_option() const { return option; }

    double get_coeff_a() const override;
    double get_coeff_b() const;

    double get_cdt_bord_b(double t) const override;
    double get_cdt_bord_h(double t, double s) const override;
    double get_cdt_term(double s) const override;
};

#endif
