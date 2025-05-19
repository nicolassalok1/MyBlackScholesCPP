#ifndef _H_OPTION
#define _H_OPTION

#include "Payoff.h"

/**
 * @brief Classe Option numérisant une option par ses paramètres numériques
 */
class Option {
    public:
        Payoff* payoff;
        double r;
        double sigma;
        double K;
        double T;
        double L;

    public:
        Option(Payoff* payoff_, double T_, double r_, double sigma_, double K_, double L_) : payoff(payoff_), r(r_), sigma(sigma_), K(K_), T(T_), L(L_){};
};

#endif