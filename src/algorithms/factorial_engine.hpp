#ifndef REMAINDERTREE_SRC_ALGORITHMS_FACTORIALENGINE_H_
#define REMAINDERTREE_SRC_ALGORITHMS_FACTORIALENGINE_H_


// included by rem_factorial.tpp

#include <functional>
#include <vector>

#include <NTL/ZZ_pX.h>
#include <NTL/matrix.h>

//TODO: make these all const
template<typename T, typename U>
T naive_factorial(long n, const U& m, const std::function<std::vector<T> (long, long)>& get_A);

NTL::ZZ_p poly_factorial(long n, const NTL::ZZ& m, const NTL::ZZ_pX& poly);

NTL::Mat<NTL::ZZ_p> matrix_factorial(long n, const NTL::ZZ& m, const NTL::Mat<NTL::ZZ_pX>& matrix);


#include "factorial_engine.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_FACTORIALENGINE_H_
