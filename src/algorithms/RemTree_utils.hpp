#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <vector>
#include <assert>

inline int flatten(int i , int j) {
	return (1<<i)+j;
}

template <typename T>
T get_product_node(std::vector<T>& vals, int k) {
	int N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		return vals[k-N];
	}

	else {
		return get_product_node(vals, 2*k)*get_product_node(vals, 2*k+1);
	}
}

template <typename T>
T get_product_node(std::vector<T>& vals, int i, int j) {
	assert (j < (1<<i));
	return get_product_node(vals, flatten(i,j));
}

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_