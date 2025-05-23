#include "PDE.h"
#include <cmath>  // Pour std::pow

// --- Coefficients de l'EDP complète ---
double CompletePDE::get_coeff_a() const {
        return 1.0;
}

double CompletePDE::get_coeff_b(double s) const {
        return 0.5 * std::pow(option->sigma, 2.0) * std::pow(s, 2.0);
}

double CompletePDE::get_coeff_c(double s) const {
        return option->r * s;
}

double CompletePDE::get_coeff_d() const {
        return -option->r;
}

// --- Coefficients de l'EDP réduite ---
double ReducedPDE::get_coeff_a() const {
        return 1.0;
}

double ReducedPDE::get_coeff_b() const {
        return 0.5 * std::pow(option->sigma, 2.0);
}

// --- Conditions aux bords et terminales ---
double CompletePDE::get_cdt_bord_b(double t) const {
        if (option->payoff->get_payoff_type() == PAYOFF_TYPE_CALL) return 0.0;
        else return option->K * std::exp(-option->r * (option->T - t));
}

double CompletePDE::get_cdt_bord_h(double t, double s) const {
        if (option->payoff->get_payoff_type() == PAYOFF_TYPE_CALL) return s - option->K * std::exp(-option->r * (option->T - t));
        else return 0.0;
}

double CompletePDE::get_cdt_term(double s) const {
        return (*option->payoff)(s);
}

double ReducedPDE::get_cdt_bord_b(double t) const {
        return 0.0;
}

double ReducedPDE::get_cdt_bord_h(double t, double s) const {
        return 0.0;
}

double ReducedPDE::get_cdt_term(double s) const {
        return (*option->payoff)(s);
}
