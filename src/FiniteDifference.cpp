#include "FiniteDifference.h"

#include <fstream>

/*
====================================================================
                Schéma implicite
====================================================================
*/
/**
 * @brief Constructeur du résolveur par méthode implicite
 * @param pde_ EDP réduite
 * @param M_ M : nombre d'intervalles temporels
 * @param N_ N : nombre d'intervalles spatiaux
 * @param L_ L : longueur du domaine spatial
 * @param T_ T : longueur du domaine temporel
 */
IMFD::IMFD(ReducedPDE* pde_, int M_, int N_, double L_, double T_) : pde(pde_), M(M_), N(N_), T(T_), L(L_) {
        // Récupération des paramètres de l'EDP
        r = pde->get_option()->r;
        sigma = pde->get_option()->sigma;

        // Discrétisation de l'espace et du temps + Récupération des pas
        set_mesh();   // OK
        dt = t->get_step();
        ds = s->get_step();
        mu = -pde->get_coeff_b();

        // Dimensionnement des vecteurs des coefficients
        a.resize(N - 1, 0.0);
        b.resize(N - 1, 0.0);
        c.resize(N - 1, 0.0);

        // Remplissage des vecteurs a, b, c, d
        set_matrix_coefficients();   // OK

        /*
            Afin de calculer le membre de droite à chaque étape, il faut réaliser un produit matrice-vecteur
            Il faut donc créer la matrice M_1 que l'on rempli avec les coefficients a, c et d.
        */
        // Création de la matrice M1 (structure)
        M1 = new double*[N - 1];
        for (int i = 0; i < N - 1; i++)
                M1[i] = new double[N - 1];

        // Remplissage provisoire de la matrice
        for (int i = 0; i < N - 1; i++) {
                for (int j = 0; j < N - 1; j++) {
                        M1[i][j] = 0.0;
                }
        }

        // Ajout des coefficients sur les diagonales de M1
        set_coefficients_M1();   // OK

        // Dimensionnement du vecteur des solutions
        C.resize(N - 1, 0.0);

        // Remplissage du vecteur solution (C) avec la condition terminale
        set_terminal_condition();   // OK

        // Dimensionnement du vecteur k pour les conditions initiales
        k.resize(N - 1, 0.0);

        // Dimensionnement du vecteur RHS pour le membre de droite
        RHS.resize(N - 1, 0.0);
}

/**
 * @brief Fonction calculant la solution numérique de l'EDP en suivant le schéma des différences finies implicites
 */
void IMFD::compute_solution() {
        // Pour chaque instant t entre M - 1 et 0
        for (int m = 0; m < M; m++) {
                // Calcul du vecteur k^{(m + 1)} représentant les conditions aux bords
                compute_vector_k(m);

                // Calcul du terme de droite (= RHS) (produit matrice-vecteur (M_1 \times C ) + addition avec le vecteur k^{(m + 1)} nouvellement créé)
                compute_RHS_member(M1, C);   // le résultat  est stocké dans RHS

                // Application de l'algorithme de Thomas pour résolution du système M_2 \times C = RHS ==> on obtient la solution C
                C = thomas_algo(a, b, c, RHS);   // Pas de pb ici
        }
        // En fin de boucle, le vecteur C contient C(0, M_1) ==> C(0, t_{M - 1})

        // Il faut lui rajouter C(0, 0) et C(0, N) (les conditions aux bords) afin d'obtenir le vecteur final
        // A la fin, le vecteur C contient C(0, t_0) ==> C(0, t_{M})
        C.insert(C.begin(), pde->get_cdt_bord_b((*t)[0]));
        //C.push_back(pde->get_cdt_bord_h(0, 1));
        // Fin algorithme == FIN ALGO !!
}

/**
 * @brief Fonction réalisant la discrétisation en temps et en espace
 */
void IMFD::set_mesh() {
        // Discrétisations de l'espace-temps
        t = new Mesh(T, M);   // Discrétisation temporelle
        s = new Mesh(L, N);   // Discrétisation spatiale
}

/**
 * @brief Fonction calculant et stockant les coefficients a, b et c
 */
void IMFD::set_matrix_coefficients() {
        double alpha = (mu * dt) / (double)pow(ds, 2.0);
        for (int i = 0; i < N - 1; i++) {
                // Méthode pure CN

                a[i] = -alpha;
                b[i] = 1 + 2 * alpha;
                c[i] = -alpha;
        }
}

/**
 * @brief Fonction remplissant les coefficients a, b, c dans la matrice M1
 */
void IMFD::set_coefficients_M1() {
        // Remplissage de la diagonale principale
        for (int i = 0; i < N - 1; i++)
                M1[i][i] = b[i];

        // Remplissage des diagonales autour de la diagonale principale
        for (int i = 0; i < N - 2; i++) {
                M1[i][i + 1] = c[i];
                M1[i + 1][i] = a[i + 1];
        }
}

/**
 * @brief Fonction implémentant la condition terminale sur le vecteur C
 */
void IMFD::set_terminal_condition() {
        for (int j = 0; j < N - 1; j++) {
                C[j] = pde->get_cdt_term((*s)[j + 1]);   // TODO : enlever le paramètre en T
        }
}

/**
 * @brief Fonction calculant la valeur du vecteur k à l'instant t_{m}
 * @param m Entier m correspondant à l'instant t_m
 */
void IMFD::compute_vector_k(int m) {
        // Calcul du premier terme du vecteur k
        k[0] = a[0] * (pde->get_cdt_bord_b((*t)[m]));

        // Calcul du dernier terme du vecteur k
        k[N - 2] = c[N - 2] * (pde->get_cdt_bord_h((*t)[m], (*s)[N - 2]));
}

/**
 * @brief Fonction calculant le terme de droite de l'expression
 */
void IMFD::compute_RHS_member(Matrix M, std::vector<double> v) {
        for (int i = 0; i < N - 1; i++) {
                double tmp = 0.0;
                for (int j = 0; j < N - 1; j++) {
                        tmp = tmp + M[i][j] * v[j];
                }
                RHS[i] = tmp + k[i];   // Ajout de R à M*v
        }
}

/**
 * @brief Fonction résolvant le système M2 * C = RHS ==> solution C dans le vecteur C
 * @param a_ Bande diagonale inférieure
 * @param b_ Bande diagonale
 * @param c_ Bande diagonale supérieure
 * @param d_ RHS
 */
std::vector<double> IMFD::thomas_algo(std::vector<double> a_, std::vector<double> b_, std::vector<double> c_, std::vector<double> d_) {
        int n = d_.size();
        n--;
        c_[0] /= b_[0];
        d_[0] /= b_[0];

        for (int i = 1; i < n; i++) {
                c_[i] /= b_[i] - a_[i] * c_[i - 1];
                d_[i] = (d_[i] - a_[i] * d_[i - 1]) / (b_[i] - a_[i] * c_[i - 1]);
        }

        d_[n] = (d_[n] - a_[n] * d_[n - 1]) / (b_[n] - a_[n] * c_[n - 1]);

        for (int i = n; i-- > 0;) {
                d_[i] -= c_[i] * d_[i + 1];
        }
        return d_;
}

/**
 * @brief Fonction enregistrant sur un fichier CSV les solutions trouvées par le solveur
 * @param file_title Titre du fichier
 */
void IMFD::safe_csv(const char* file_title) {
        std::ofstream f_out(file_title);
        f_out << "s;c" << std::endl;
        for (int j = 0; j < N; j++) {   // On supprime les premier et dernier éléments (car ceux sont les cdts aux bords)
                f_out << (*s)[j] << ";" << C[j] << std::endl;
        }
        f_out.close();
}

/*
====================================================================
                Schéma de Crank-Nicholson
====================================================================
*/

/**
 * @brief Constructeur du résolveur par méthode de Crank-Nicholson
 * @param pde_ EDP réduite
 * @param M_ M : nombre d'intervalles temporels
 * @param N_ N : nombre d'intervalles spatiaux
 * @param L_ L : longueur du domaine spatial
 * @param T_ T : longueur du domaine temporel
 */
CrankNicholsonFD::CrankNicholsonFD(CompletePDE* pde_, int M_, int N_, double L_, double T_) : pde(pde_), M(M_), N(N_), T(T_), L(L_) {
        // Récupération des paramètres de l'EDP
        r = pde->get_option()->r;
        sigma = pde->get_option()->sigma;

        // Discrétisation de l'espace et du temps + Récupération des pas
        set_mesh();   // OK
        dt = t->get_step();
        ds = s->get_step();

        // Dimensionnement des vecteurs des coefficients
        a.resize(N - 1, 0.0);
        b.resize(N - 1, 0.0);
        c.resize(N - 1, 0.0);
        d.resize(N - 1, 0.0);
        e.resize(N - 1, 0.0);
        f.resize(N - 1, 0.0);

        // Remplissage des vecteurs a, b, c, d
        set_matrix_coefficients();   // OK

        /*
            Afin de calculer le membre de droite à chaque étape, il faut réaliser un produit matrice-vecteur
            Il faut donc créer la matrice M_1 que l'on rempli avec les coefficients a, c et d.
        */
        // Création de la matrice M1 (structure)
        M1 = new double*[N - 1];
        for (int i = 0; i < N - 1; i++)
                M1[i] = new double[N - 1];

        // Remplissage provisoire de la matrice
        for (int i = 0; i < N - 1; i++) {
                for (int j = 0; j < N - 1; j++) {
                        M1[i][j] = 0.0;
                }
        }

        // Création de la matrice M2 (structure)
        M2 = new double*[N - 1];
        for (int i = 0; i < N - 1; i++)
                M2[i] = new double[N - 1];

        // Remplissage provisoire de la matrice
        for (int i = 0; i < N - 1; i++) {
                for (int j = 0; j < N - 1; j++) {
                        M2[i][j] = 0.0;
                }
        }

        // Ajout des coefficients sur les diagonales de M1
        set_coefficients_M1();   // OK

        // Ajout des coefficients sur les diagonales de M2
        set_coefficients_M2();   // OK

        // Dimensionnement du vecteur des solutions
        C.resize(N - 1, 0.0);

        // Remplissage du vecteur solution (C) avec la condition terminale
        set_terminal_condition();   // OK

        // Dimensionnement du vecteur k pour les conditions initiales
        k.resize(N - 1, 0.0);

        // Dimensionnement du vecteur RHS pour le membre de droite
        RHS.resize(N - 1, 0.0);
}

/**
 * @brief Fonction calculant la solution numérique de l'EDP en suivant le schéma de Crank-Nicholson
 */
void CrankNicholsonFD::compute_solution() {
        // Pour chaque instant t entre M - 1 et 0
        for (int m = M; m > 0; m--) {
                // Calcul du vecteur k^{(m + 1)} représentant les conditions aux bords
                compute_vector_k(m);

                // Calcul du terme de droite (= RHS) (produit matrice-vecteur (M_1 \times C ) + addition avec le vecteur k^{(m + 1)} nouvellement créé)
                compute_RHS_member(M1, C);   // le résultat  est stocké dans RHS

                // Application de l'algorithme de Thomas pour résolution du système M_2 \times C = RHS ==> on obtient la solution C
                C = thomas_algo(e, d, f, RHS);   // Pas de pb ici
                // Algorithme Crout pour résolution système M_2 * C = RHS
                //C = crout(M2, RHS);   // Pas de pb ici
        }
        // En fin de boucle, le vecteur C contient C(0, M_1) ==> C(0, t_{M - 1})

        // Il faut lui rajouter C(0, 0) et C(0, N) (les conditions aux bords) afin d'obtenir le vecteur final
        // A la fin, le vecteur C contient C(0, t_0) ==> C(0, t_{M})
        C.insert(C.begin(), pde->get_cdt_bord_b((*t)[0]));
        //C.push_back(pde->get_cdt_bord_h(0, 1));
        // Fin algorithme == FIN ALGO !!
}

/**
 * @brief Fonction réalisant la discrétisation en temps et en espace
 */
void CrankNicholsonFD::set_mesh() {
        // Discrétisations de l'espace-temps
        t = new Mesh(T, M);   // Discrétisation temporelle
        s = new Mesh(L, N);   // Discrétisation spatiale
}

/**
 * @brief Fonction calculant et stockant les coefficients a, b, c et d
 */
void CrankNicholsonFD::set_matrix_coefficients() {
        for (int i = 0; i < N - 1; i++) {
                int j = i + 1;
                // Méthode pure CN

                a[i] = 0.25 * j * dt * (pow(sigma, 2) * j - r);
                b[i] = 1 - 0.5 * (pow(sigma, 2) * pow(j, 2) * dt);
                c[i] = 0.25 * j * dt * (pow(sigma, 2) * j + r);
                d[i] = 1 + 0.5 * (pow(sigma, 2) * pow(j, 2) * dt) + r * dt;
                e[i] = -a[i];
                f[i] = -c[i];
        }
}

/**
 * @brief Fonction remplissant les coefficients a, b, c  dans la matrice M1
 */
void CrankNicholsonFD::set_coefficients_M1() {
        // Remplissage de la diagonale principale
        for (int i = 0; i < N - 1; i++)
                M1[i][i] = b[i];

        // Remplissage des diagonales autour de la diagonale principale
        for (int i = 0; i < N - 2; i++) {
                M1[i][i + 1] = c[i];
                M1[i + 1][i] = a[i + 1];
        }
}

/**
 * @brief Fonction remplissant les coefficients d, e et f dans la matrice M2
 */
void CrankNicholsonFD::set_coefficients_M2() {
        // Remplissage de la diagonale principale
        for (int i = 0; i < N - 1; i++)
                M2[i][i] = d[i];

        // Remplissage des diagonales autour de la diagonale principale
        for (int i = 0; i < N - 2; i++) {
                M2[i][i + 1] = -c[i];
                M2[i + 1][i] = -a[i + 1];
        }
}
/**
 * @brief Fonction implémentant la condition terminale sur le vecteur C
 */
void CrankNicholsonFD::set_terminal_condition() {
        for (int j = 0; j < N - 1; j++) {
                C[j] = pde->get_cdt_term((*s)[j + 1]);   // TODO : enlever le paramètre en T
        }
}

/**
 * @brief Fonction calculant la valeur du vecteur k à l'instant t_{m}
 * @param m Entier m correspondant à l'instant t_m
 */
void CrankNicholsonFD::compute_vector_k(int m) {
        // Calcul du premier terme du vecteur k
        k[0] = a[0] * (pde->get_cdt_bord_b((*t)[m]) + pde->get_cdt_bord_b((*t)[m + 1]));

        // Calcul du dernier terme du vecteur k
        k[N - 2] = c[N - 2] * (pde->get_cdt_bord_h((*t)[m], (*s)[N - 2]) + pde->get_cdt_bord_h((*t)[m + 1], (*s)[N - 2]));
}

/**
 * @brief Fonction calculant le terme de droite de l'expression
 */
void CrankNicholsonFD::compute_RHS_member(Matrix M, std::vector<double> v) {
        for (int i = 0; i < N - 1; i++) {
                double tmp = 0.0;
                for (int j = 0; j < N - 1; j++) {
                        tmp = tmp + M[i][j] * v[j];
                }
                RHS[i] = tmp + k[i];   // Ajout de R à M*v
        }
}

/**
 * @brief Fonction résolvant le système M2 * C = RHS ==> solution C dans le vecteur C
 * @param a_ Bande diagonale inférieure
 * @param b_ Bande diagonale
 * @param c_ Bande diagonale supérieure
 * @param d_ RHS
 */
std::vector<double> CrankNicholsonFD::thomas_algo(std::vector<double> a_, std::vector<double> b_, std::vector<double> c_, std::vector<double> d_) {
        int n = d_.size();
        n--;
        c_[0] /= b_[0];
        d_[0] /= b_[0];

        for (int i = 1; i < n; i++) {
                c_[i] /= b_[i] - a_[i] * c_[i - 1];
                d_[i] = (d_[i] - a_[i] * d_[i - 1]) / (b_[i] - a_[i] * c_[i - 1]);
        }

        d_[n] = (d_[n] - a_[n] * d_[n - 1]) / (b_[n] - a_[n] * c_[n - 1]);

        for (int i = n; i-- > 0;) {
                d_[i] -= c_[i] * d_[i + 1];
        }
        return d_;
}

/**
 * @brief Fonction enregistrant sur un fichier CSV les solutions trouvées par le solveur
 * @param file_title Titre du fichier
 */
void CrankNicholsonFD::safe_csv(const char* file_title) {
        std::ofstream f_out(file_title);
        f_out << "s;c" << std::endl;
        for (int j = 0; j < N; j++) {   // On supprime les premier et dernier éléments (car ceux sont les cdts aux bords)
                f_out << (*s)[j] << ";" << C[j] << std::endl;
        }
        f_out.close();
}