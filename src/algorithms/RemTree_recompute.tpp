#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <vector>
#include <assert.h>
#include "utils.hpp"

/*
 * TODO: the get_accumulated_remainder will recompute EVERYTHING
 * 		most likely is more efficient (time wise) to be slightly smarter about what it saves.
 */

template <typename T>
T compute_product_node(const std::vector<T>& vals, int k) { //typically used to get the value for the m tree
	int N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		return vals[k-N];
	}

	else {
		return compute_product_node<T>(vals, 2*k)*compute_product_node<T>(vals, 2*k+1);
	}
}


template <typename T, typename U>
T compute_product_node_askew(const std::vector<T>& vals, const U& modulus, int k) { //typically used to get the value for the A tree
	if (modulus == 0) { //DEBUG: default value only, 0 should never be passed in!
		return T();
	}

	int N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		return vals[k-N]%modulus;
	}

	else if (is_power2(k)) {
		return vals[0]%modulus;
	}

	else {
		T C = compute_product_node_askew<T,U>(vals, modulus, 2*k);
		C.mulmod(compute_product_node_askew<T,U>(vals, modulus, 2*k-1), modulus);
		return C;
	}	
}


template <typename T, typename U>
std::vector<T> compute_remainder_tree_layer(const std::vector<T>& A, const std::vector<U>& m, int l) {
	//level is 0-indexed, so the left most is (1<<l)
	//thus, l = 1 corresponds to the first children of the root

	assert (A.size() == m.size())

	if (l == 0) {
		std::vector<T> root = {A[0]%compute_product_node<U>(m,1)};
		return root;
	}


	//TODO: might be marinally faster to do it all in place rather than recursively, but this is
	//pretty easy to understand. It will not be difficult to make it in place later if we
	//deem it to be useful. Here is what it might look like done iteratively
	/*
	 * Everything is done 'in place' on one vector. For example, if I want the third layer,
	 * It is computed as follows (0-indexing the vector) where + computes and - erases
	 * [+1]
	 * [+2, +3, -1]
	 * [+4, +5, -2, +6, +7, -3]
	 * [+0, +1, -4, +2, +3, -5,  +4, +5, -6, +6, +7, -7]
	 * 
	 * The values are captured for modulus and skew products using the above functions to recompute
	 */

	std::vector<T> parent_layer = compute_remainder_tree_layer<T,U>(A, m, l-1);

	int N = parent_layer.size();

	std::vector<T> child_layer (2*N);

	for (int j = 0; j < N; ++j) {

		//left child is parent mod the mtree val
		child_layer[2*j] = parent_layer[j]%compute_product_node<U>(m, flatten(l, 2*j));

		//right child is parent times the atree mod the mtree
		U node_mod = compute_product_node<U>(m, flatten(2*j + 1));

		child_layer[2*j + 1] = parent_layer[j];
		child_layer[2*j + 1].mulmod(compute_product_node_askew<T,U>(A, node_mod, flatten(l, 2*j + 1)), node_mod);

		parent_layer[j] = T(); //Always delete parent after it is not needed
	}

	return child_layer
}




//This is only to test "chunks". It will be replaced, although shouldn't typically be a bottleneck.
template <typename T, typename U>
T compute_remainder_tree_value(const std::vector<T>& A, const std::vector<U>& m, int k) {
	U modulus = get_product_node<U> (m, k);

	return _compute_remainder_tree_value<T,U>(A, modulus, k);
}


//For internal use only, to implement the above.
template <typename T, typename U>
T _compute_remainder_tree_value(const std::vector<T>& A, const U& modulus, int k) {
	assert (A.size() == m.size());

	if (k == 1) { //return A[0] mod mProd
		return (A[0]%modulus);
	}

	//else, it is not the root of the Ctree
	bool dir = (k%2);
	if (dir) { //right child is parent times the atree mod the mtree
		T C = _compute_remainder_tree_value<T,U>(A, modulus, (k >> 1));
		C.mulmod(compute_product_node_askew<T,U>(A, modulus, k), modulus);
		return C;
	}

	else { //left child is parent mod the mtree val

		//%modulus; is not necessary because parent is already reduced!
		return _compute_remainder_tree_value<T,U>(A, modulus, (k >> 1));
	}
}


#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_