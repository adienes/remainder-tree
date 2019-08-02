#include <functional>

#include "utils.h"

#include <NTL/ZZ_pX.h>
#include <NTL/matrix.h>

using NTL::ZZ_pX;
using NTL::ZZ_p;
using NTL::ZZ;
using NTL::Mat;

/*
 * Finds the product A[0]A[1]...A[n-1] using:
 * naive_factorial: naive product tree calculation
 * poly_factorial: calculation that works when A is a polynomial
 * matrix_factorial: calculation that works when A is a matrix with linear entries
 */

template<typename T, typename U>
T naive_factorial(long n, U& m, std::function<vector<T> (long, long)>& get_A);

ZZ_p poly_factorial(long n, ZZ& m, ZZ_pX& poly);

Mat<ZZ_p> matrix_factorial(long n, ZZ& m, Mat<ZZ_pX>& matrix);

template<typename T, typename U>
T naive_factorial(long n, U m, std::function<vector<T> (long, long)>& get_A){
    // use get node method in utils.h
    return compute_product_node<T, U>(get_A(0, n), m, 1);
}

ZZ_p poly_factorial(long n, ZZ m, ZZ_pX poly){
    
}

Mat<ZZ_p> matrix_factorial(long n, ZZ& m, Mat<ZZ_pX>& matrix){

}

