#include <vector>
#include <assert.h>

#include "../elements/Element.hpp"

using std::vector;

bool is_power2(long x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

long flatten(long i , long j) {
	return (1<<i)+j;
}

template <typename T>
vector<T> get_subtree (const vector<T>& tree, long k) {
	long N = tree.size();

	vector<T> subtree(N);

	long i = 0;
	while ((1<<i)*(k+1) <= N) { //up through the leaves 
		for (long j = 0; j < (1<<i); ++j) {

			long node = flatten(i,j);
			subtree[node] = tree[node];
		}
		++i;
	}

	subtree.shrink_to_fit();
	return subtree;
}

template <typename T>
vector<T> get_shade(const vector<T>& vals, long k) {
	long N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		vector<T> slice(1);
		slice[0] = vals[k-N];
		return slice;
	}

	long left = k; //this will be the leftmost leaf under the subtree rooted at P
	long right = k; //similarly the rightmost leaf under subtree rooted at P
	while (left < N) { //stops when left_span is a leaf
		left = 2*left;
		right = 2*right + 1;

	}
	left -= N;
	right -= N;

	typename vector<T>::const_iterator first = vals.begin() + left;
	typename vector<T>::const_iterator last = vals.begin() + right + 1;
	vector<T> slice(first, last);

	return slice;
}


template<typename T>
vector<T> elt_to_base(const vector<Elt<T>> &e) {
	long N = e.size();

	vector<T> b(N);

	for(long i = 0; i < N; ++i) {
		b[i] = e[i].t;
	}

	return b;
}

template<typename T>
vector<Elt<T>> base_to_elt(const vector<T>& b) {
	long N = b.size();

	vector<Elt<T>> e(N);

	for(long i = 0; i < N; ++i) {
		e[i] = b[i];
	}

	return e;
}