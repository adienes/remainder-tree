#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREE_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREE_H_

#include<vector>
using std::vector;

//The most bare-bones remainder tree possible.
//No space-time tradeoffs implemented, no parallelization, no hard drive mem, etc.
//It assumes the types are Elt<>, so wrapping your object in Elt is strongly recommended.
template <typename T, typename U>
vector<T> remainder_tree_basic(const vector<T>&, const vector<U>&, const T& V = T(1));


//TODO
//This adds the ability to call remtree on arrays which are not size a power of 2.
//it does this by padding the arrays with zeros up to a power of 2, so will be a tiny
//bit less efficient than just calling it on a power of 2 originally.
//Additionally, it passes by value in order to pad. For best performance, just
//call remainder_tree_power2
//I'm not convinced this will ever be useful
template <typename T, typename U>
vector<T> remainder_tree_padded(vector<T>, vector<U>, const T& V = T(1));



#include "rem_tree.tpp"

#endif // REMAINDERTREE_SRC_ALGORITHMS_REMTREE_H_