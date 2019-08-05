#ifndef REMAINDERTREE_SRC_ALGORITHMS_FACTORIALALGS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_FACTORIALALGS_H_


// included by rem_factorial.tpp

#include <functional>
#include <vector>

#include <NTL/ZZ_pX.h>
#include <NTL/matrix.h>


template<typename T, typename U>
T naive_factorial(long n, U& m, std::function<std::vector<T> (long, long)>& get_A);

NTL::ZZ_p poly_factorial(long n, NTL::ZZ& m, NTL::ZZ_pX& poly);

NTL::Mat<NTL::ZZ_p> matrix_factorial(long n, NTL::ZZ& m, NTL::Mat<NTL::ZZ_pX>& matrix);


#endif //REMAINDERTREE_SRC_ALGORITHMS_FACTORIALALGS_H_