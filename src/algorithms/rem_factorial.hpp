#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMFACTORIAL_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMFACTORIAL_H_

#include <vector>
#include <functional>

#include "utils.hpp"


/*
 * Inputs: 
 * long n: the upper bound of the factorial calculation, 
 * U m: the mod we're working in,
 * function<T (long)> get_A: a function to return the value of A[i] given the input i,
 * PolyMatrix formula = {}: formula of the general term, set by default to an empty array.
 *
 * Output:
 * The value of the factorial A[0]A[1]...A[n-1] mod m
 *
 */
template <typename T, typename U>
T calculate_factorial(long, const U&, const std::function<std::vector<T> (long, long)>&, const PolyMatrix& formula = {});


#include "rem_factorial.tpp"

#endif // REMAINDERTREE_SRC_ALGORITHMS_REMFACTORIAL_H_
