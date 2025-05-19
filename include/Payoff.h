#ifndef _H_PAYOFF
#define _H_PAYOFF

#include <algorithm>   // Pour la fonction max

#define PAYOFF_TYPE_CALL 0
#define PAYOFF_TYPE_PUT 1

/*
    L'ajout des payoff type permettent dans la class PDE d'implémenter les conditions aux bords et terminale sans avoir à transférer à chaque fois le type de payoff simulé.
    Gain de temps, d'espace et de portabilité
*/

/**
 * @brief Classe abstraite de Payoff
 */
class Payoff {
    public:   // c-tor et d-tor
        virtual int get_payoff_type() const = 0;

    public:
        // Surcharge de l'opérateur ()
        virtual double operator()(const double &S) const = 0;
};

/**
 * @brief Classe implémentant le payoff Call.
 * Hérite de la classe abstraite Payoff
 * 
 */
class Call : public Payoff {
    private:
        double K;   // Strike
        int payoff_type;

    public:   // c-tor et d-tor
        Call(const double &K_) : K(K_) { payoff_type = PAYOFF_TYPE_CALL; };
        int get_payoff_type() const { return payoff_type; }

    public:   // fonction de payoff
        virtual double operator()(const double &S) const;
};

/**
 * @brief Classe implémentant le payoff Put
 * Hérite de la classe abstraire Payoff
 */
class Put : public Payoff {
    private:
        double K;   // Strike
        int payoff_type;

    public:
        Put(const double &K_) : K(K_) { payoff_type = PAYOFF_TYPE_PUT; };
        int get_payoff_type() const { return payoff_type; }

    public:
        virtual double operator()(const double &S) const;
};

#endif