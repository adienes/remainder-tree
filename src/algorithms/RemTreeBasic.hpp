#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEBASIC_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEBASIC_H_

#include<vector>
using std::vector;

//Simple macro to print the vector. Maybe I will format it someday
template <typename T>
void print_tree(vector<T>&);

inline int flatten(int i, int j) {
	return ((1 << i) + j);
}

//The most bare-bones remainder tree possible.
//No space-time tradeoffs implemented, no parallelization, no hard drive mem, etc.
//It assumes the types are Elt<>, so wrapping your object in Elt is strongly recommended.
template <typename T, typename U>
vector<T> remainder_tree(const vector<T>&, const vector<U>&, const T& V = T(1));


//This only exists for convenient notation
template <typename T>
vector<T> remainder_tree(const vector<T>&, const vector<T>&, const T& V = T(1));


#include "RemTreeBasic.tpp"

#endif // REMAINDERTREE_SRC_ALGORITHMS_REMTREEBASIC_H_