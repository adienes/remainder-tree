#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <vector>
#include <assert.h>
#include "RemTree_utils.hpp"

/*
 * TODO: the get_accumulated_remainder will recompute EVERYTHING
 * 		most likely is more efficient (time wise) to be slightly smarter about what it saves.
 */

template <typename T>
T get_product_node(const std::vector<T>& vals, int k) { //typically used to get the value for the m tree
	int N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		return vals[k-N];
	}

	else {
		return get_product_node(vals, 2*k)*get_product_node(vals, 2*k+1);
	}
}


template <typename T, typename U>
T get_product_node_askew(const std::vector<T>& vals, const U& modulus, int k) { //typically used to get the value for the A tree
	if (modulus == 0) { //DEBUG: default value only, should never be passed in!
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
		T C = get_product_node_askew(vals, 2*k, modulus);
		C.mulmod(get_product_node_askew(vals, 2*k-1), modulus);
		return C;
	}	
}


template <typename T, typename U>
T get_accumulated_remainder(const std::vector<T>& A, const std::vector<U>& m, int k) {
	assert (A.size() == m.size());

	if (k == 1) { //return A[0] mod mProd
		return (A[0]%get_product_node(m, 1));
	}

	//else, it is not the root of the Ctree
	bool dir = (k%2);
	if (dir) { //right child is parent times the atree mod the mtree
		T C = get_accumulated_remainder(A, m, (k >> 1));
		C.mulmod(get_product_node_askew(A, k), get_product_node(m, k));
		return C;
	}

	else { //left child is parent mod the mtree val
		return get_accumulated_remainder(A, m, (k >> 1))%get_product_node(m, k);
	}
}


#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_