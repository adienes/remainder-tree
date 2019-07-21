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