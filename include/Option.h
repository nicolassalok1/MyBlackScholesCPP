#ifndef _H_OPTION
#define _H_OPTION

#include "Payoff.h"

/**
 * @brief Classe représentant une option financière européenne
 *
 * Cette classe encapsule les paramètres nécessaires à la résolution numérique
 * de l'équation de Black-Scholes.
 */
class Option {
public:
    Payoff* payoff;   ///< Pointeur vers le payoff associé (Call ou Put)
    double r;         ///< Taux sans risque
    double sigma;     ///< Volatilité
    double K;         ///< Strike
    double T;         ///< Maturité
    double L;         ///< Borne supérieure pour l'espace (ex. : S_max)

    /**
     * @brief Constructeur principal
     * @param payoff_  Pointeur vers le payoff
     * @param T_       Maturité
     * @param r_       Taux sans risque
     * @param sigma_   Volatilité
     * @param K_       Strike
     * @param L_       Limite supérieure en espace (souvent S_max)
     */
    Option(Payoff* payoff_, double T_, double r_, double sigma_, double K_, double L_)
        : payoff(payoff_), r(r_), sigma(sigma_), K(K_), T(T_), L(L_) {}
};

#endif
