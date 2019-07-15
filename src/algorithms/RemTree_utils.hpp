#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <vector>
#include <assert.h>

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
std::vector<T> get_subtree (std::vector<T>& tree, int k) {
	int N = tree.size();

	std::vector<T> subtree(N);

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
void elt_to_base(vector<T> &b, vector<Elt<T>> &e){
	assert(b.size() == e.size());
	for(int i = 0; i < b.size(); i++){
		b[i] = e[i].t;
	}
}

template<typename T>
void base_to_elt(vector<Elt<T>> &e, vector<T> &b){
	assert(e.size() == b.size());
	for(int i = 0; i < e.size(); i++){
		e[i] = b[i];
	}
}

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_