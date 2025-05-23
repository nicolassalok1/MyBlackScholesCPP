#include "Payoff.h"

/**
 * @brief Fonction retournant le payoff d'un Call européen
 * @param S 
 * @return double 
 */
double Call::operator()(const double& S) const {
        return std::max(0.0, S - K);
}

/**
 * @brief Fonction retournant le payoff d'un Put européen
 * @param S 
 * @return double 
 */
double Put::operator()(const double& S) const {
        return std::max(0.0, K - S);
}