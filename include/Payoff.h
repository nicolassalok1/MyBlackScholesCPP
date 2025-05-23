#ifndef _H_PAYOFF
#define _H_PAYOFF

#include <algorithm>  // Pour std::max

// Identifiants pour les types de payoff
#define PAYOFF_TYPE_CALL 0
#define PAYOFF_TYPE_PUT  1

/**
 * @brief Classe abstraite représentant un payoff
 */
class Payoff {
public:
    virtual ~Payoff() = default;

    // Retourne l'identifiant du type de payoff (CALL/PUT)
    virtual int get_payoff_type() const = 0;

    // Évalue le payoff au niveau d'un sous-jacent donné
    virtual double operator()(const double& S) const = 0;
};

/**
 * @brief Classe représentant un payoff de type Call européen
 */
class Call : public Payoff {
private:
    double K;         // Strike
    int payoff_type;

public:
    explicit Call(const double& K_) : K(K_), payoff_type(PAYOFF_TYPE_CALL) {}
    int get_payoff_type() const override { return payoff_type; }
    double operator()(const double& S) const override;
};

/**
 * @brief Classe représentant un payoff de type Put européen
 */
class Put : public Payoff {
private:
    double K;         // Strike
    int payoff_type;

public:
    explicit Put(const double& K_) : K(K_), payoff_type(PAYOFF_TYPE_PUT) {}
    int get_payoff_type() const override { return payoff_type; }
    double operator()(const double& S) const override;
};

#endif
