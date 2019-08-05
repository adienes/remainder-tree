#include <cassert>

#include "factorial_algs.hpp"

#include <NTL/matrix.h>
#include <NTL/ZZ_pX.h> // includes ZZ_p.h and ZZ.h

using NTL::Mat;
using NTL::ZZ_p;
using NTL::ZZ_pX;


template <typename T, typename U>
T calculate_factorial(long n, const U& m, const std::function<vector<T> (long, long)>& get_A, const PolyMatrix& formula = {}) {
    return naive_factorial(n, m, get_A); // TODO: replace temporary measure with something better
}

#include "factorial_custom.tpp"