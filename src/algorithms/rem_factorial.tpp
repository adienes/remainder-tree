#include "factorial_engine.hpp"

#include "utils.hpp"

#include <NTL/ZZ_pX.h>
#include <NTL/matrix.h>
#include "../elements/element.hpp"

using NTL::ZZ;
using NTL::ZZ_p;
using NTL::ZZ_pX;
using NTL::Mat;


template <typename T, typename U>
T calculate_factorial(long n, const U& m, const std::function<vector<T> (long, long)>& get_A, const PolyMatrix& formula) {
    // return compute_product_node<T, U>(get_A(0, n), m, 1);
    return naive_factorial(n, m, get_A); // TODO: replace temporary measure with something better
}


#include "rem_factorial_custom.tpp"
