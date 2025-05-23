#include "Payoff.h"

/**
 * @brief Évaluation du payoff d’un call : max(S - K, 0)
 */
double Call::operator()(const double& S) const {
        return std::max(0.0, S - K);
}

/**
 * @brief Évaluation du payoff d’un put : max(K - S, 0)
 */
double Put::operator()(const double& S) const {
        return std::max(0.0, K - S);
}
