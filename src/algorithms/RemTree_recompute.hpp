#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREERECOMPUTE_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREERECOMPUTE_H_

#include <vector>

/*
 * TODO: the get_accumulated_remainder will recompute EVERYTHING
 * 		most likely is more efficient (time wise) to be slightly smarter about what it saves.
 * Also, it's not super smart about references vs values and probably copies
 	a bit more than it needs to.
 */


//This function will let you retrieve the value of a product tree
//at some index using minimal memory. Usually used to compute the modulus.
template <typename T>
T compute_product_node(const std::vector<T>&, int);

//Similar to above, but uses the 'skew-product' tree for the A values.
//A modulus must be given because we don't need these values directly.
template <typename T, typename U>
T compute_product_node_askew(const std::vector<T>&, const U& modulus, int);


//This computes the entire layer rather than a specific value, as there are better ways to
//compute single values of a accumulating remainder tree! (aka the V algorithm)
template <typename T, typename U>
std::vector<T> compute_remainder_tree_layer(const std::vector<T>& A, const std::vector<U>& m, int l);


//Computes a position of the accumulating remainder tree using the above two functions
//with minimal memory. Will be superseded by V algorithm
//TODO: use Daniel's alg instead of the dumb version. This is for "chunks"
template <typename T, typename U>
T compute_remainder_tree_value(const std::vector<T>&, const std::vector<U>&, int);

#include "RemTree_recompute.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREERECOMPUTE_H_