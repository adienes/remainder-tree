#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEBASIC_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEBASIC_H_

#include<vector>
using std::vector;

//Of the product tree of vals, returns the subtree rooted at P
//Primarily intended for internal use. The default (1) returns the entire product tree.
template <typename T>
vector<T> subproduct_tree(const vector<T>& vals, int P = 1);

//Same as above, but for the askew product tree (with modulus!)
//Recall the skew product tree ONLY contains those values which are a right
//child, so any leftward values (including the root!) are not actually stored
template <typename T, typename U>
vector<T> subproduct_tree_askew(const vector<T>& vals, const U& modulus, int P = 1);

//notation
template <typename T>
vector<T> subproduct_tree_askew(const vector<T>& vals, const T& modulus, int P = 1);

//The most bare-bones remainder tree possible.
//No space-time tradeoffs implemented, no parallelization, no hard drive mem, etc.
//It assumes the types are Elt<>, so wrapping your object in Elt is strongly recommended.
//WARNING: array length is stored in int, so very large trees may overflow.
template <typename T, typename U>
vector<T> remainder_tree(const vector<T>&, const vector<U>&, const T& V = T(1));


//This only exists for convenient notation
template <typename T>
vector<T> remainder_tree(const vector<T>&, const vector<T>&, const T& V = T(1));


//TODO
//This adds the ability to call remtree on arrays which are not size a power of 2.
//it does this by padding the arrays with zeros up to a power of 2, so will be a tiny
//bit less efficient than just calling it on a power of 2 originally.
//Additionally, it passes by value in order to pad. For best performance, just
//call remainder_tree_power2
//I'm not convinced this will ever be useful
template <typename T, typename U>
vector<T> remainder_tree_padded(vector<T>, vector<U>, const T& V = T(1));


//This only exists for convenient notation
template <typename T>
vector<T> remainder_tree_padded(const vector<T>&, const vector<T>&, const T& V = T(1));


#include "RemTree_basic.tpp"

#endif // REMAINDERTREE_SRC_ALGORITHMS_REMTREEBASIC_H_