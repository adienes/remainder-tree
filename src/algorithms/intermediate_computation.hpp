#ifndef REMAINDERTREE_SRC_ALGORITHMS_INTERMEDIATECOMPUTATION_H_
#define REMAINDERTREE_SRC_ALGORITHMS_INTERMEDIATECOMPUTATION_H_

#include <vector>

//Of the product tree of vals, returns the subtree rooted at P
//Primarily intended for internal use. The default (1) returns the entire product tree.
template <typename T>
std::vector<T> subproduct_tree(const std::vector<T>&, int P = 1);

//Same as above, but for the askew product tree (with modulus!)
//Recall the skew product tree ONLY contains those values which are a right
//child, so any leftward values (including the root!) are not actually stored
template <typename T, typename U>
std::vector<T> subproduct_tree_askew(const std::vector<T>&, const std::vector<U>&, int P = 1);


//This function will let you retrieve the value of a product tree
//at some index using minimal memory. Usually used to compute the modulus.
template <typename T>
T compute_product_node(const std::vector<T>&, int);

//In case we need product with a modulus
template <typename T, typename U>
T compute_product_node(const std::vector<T>&, const U&, int);


//Similar to above, but uses the 'skew-product' tree for the A values.
//A modulus must be given because we don't need these values directly.
template <typename T, typename U>
T compute_product_node_askew(const std::vector<T>&, const U&, int);


//Computes a position of the accumulating remainder tree using the above two functions
//with minimal memory. Will be superseded by V algorithm
//TODO: use Daniel's alg instead of the dumb version. This is for "chunks"
template <typename T, typename U>
T compute_remainder_tree_value(const std::vector<T>&, const std::vector<U>&, int, const T& V = T(1));


#include "intermediate_computation.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_INTERMEDIATECOMPUTATION_H_