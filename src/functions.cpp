#include "functions.h"
#include <cmath>        // pour std::abs
#include <stdexcept>    // pour std::invalid_argument

void print_vector(const std::vector<double>& v) {
        for (const auto& val : v)
                std::cout << val << " ";
        std::cout << "\nfin" << std::endl;
}

std::vector<double> compute_diff_vector(const std::vector<double>& a, const std::vector<double>& b) {
        if (a.size() != b.size())
                throw std::invalid_argument("compute_diff_vector: tailles incompatibles.");

        std::vector<double> diff;
        diff.reserve(a.size());

        for (size_t i = 0; i < a.size(); ++i)
                diff.push_back(std::abs(a[i] - b[i]));

        return diff;
}
