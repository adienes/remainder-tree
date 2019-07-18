#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <vector>
#include <assert.h>
#include "../elements/Element.hpp"

using std::vector;

bool is_power2(int x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

int flatten(int i , int j) {
	return (1<<i)+j;
}

/*
 * TODO: this returns a smaller vector of just the subtree rooted at that node.
 * There is probably a way to refactor the code such that you can just put
 * a given node as the root and it considers all the subsequent indices relative
 * to the root.
 * Also, beware that this really only works on full binary trees; I'm not sure
 * how it interacts with incomplete trees.
 */
template <typename T>
vector<T> get_subtree (const vector<T>& tree, int k) {
	int N = tree.size();

	vector<T> subtree(N);

	int i = 0;
	while ((1<<i)*(k+1) <= N) { //up through the leaves 
		for (int j = 0; j < (1<<i); ++j) {

			int node = flatten(i,j);
			subtree[node] = tree[node];
		}
		++i;
	}

	subtree.shrink_to_fit();
	return subtree;
}


template<typename T>
vector<T> elt_to_base(const vector<Elt<T>> &e) {
	int N = e.size();

	vector<T> b(N);

	for(int i = 0; i < N; ++i) {
		b[i] = e[i].t;
	}

	return b;
}

template<typename T>
vector<Elt<T>> base_to_elt(const vector<T>& b) {
	int N = b.size();

	vector<Elt<T>> e(N);

	for(int i = 0; i < N; ++i) {
		e[i] = b[i];
	}

	return e;
}

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_