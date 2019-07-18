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
T get_product_node(const std::vector<T>&, int);

//Similar to above, but uses the 'skew-product' tree for the A values.
//A modulus must be given because we don't need these values directly.
template <typename T, typename U>
T get_product_node_askew(const std::vector<T>&, const U& modulus, int);

//same as the above, but if the modulus and the multiplicands are the same type
//as will happen frequently, then you don't have to type in both
template <typename T, typename U>
T get_product_node_askew(const std::vector<T>&, const T& modulus, int);

//Computes a position of the accumulating remainder tree using the above two functions
//with minimal memory.
template <typename T, typename U>
T get_accumulated_remainder(const std::vector<T>&, const std::vector<U>&, int);

//like before, this is for shorter notation
template <typename T>
T get_accumulated_remainder(const std::vector<T>&, const std::vector<T>&, int);

#include "RemTree_recompute.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREERECOMPUTE_H_