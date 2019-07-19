#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <vector>
#include <assert.h>
#include <functional>

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


/* TODO: It is slightly dangerous to cast milliseconds to int.
 * TODO: Does not make any checks for powers of 2 etc. Please only pass in powers of 2.
 *
 * This function is a bit of a weird one. I'm assuming that passed in is an upper bound,
 * a number of datapoints, and a vector of search functions which take a range and return
 * a vector of bools (found item or not). The search functions passed in might want more
 * arguments than just an upper bound. Use std::bind for this purpose to pass in fixed arguments.
 * Return value is a vector containing the x-value, and then the y-values for each function,
 * plus any other data we may want to collect (dunno what yet) all collected in a vector.
 */
std::vector <std::vector<int>> complexity_graph(int N, int d, std::vector <std::function< std::vector<bool> (int N)>> search_funcs) {
	int s = search_funcs.size();

	std::vector<std::vector<int>> graph;
	graph.reserve(d);
	graph[0] = std::vector<int> (s+1, 0); //Corresponds to origin---make s+1 larger if there are other data collected.
	
	int interval = N/d;
	int B = interval;
	while (B <= N) {

		std::vector<int> datum;
		datum.reserve(s+1);
		datum.push_back(B); //this is the x-value

		for (auto f : search_funcs) {
			auto start = std::chrono::high_resolution_clock::now();
			f(B);
			auto end = std::chrono::high_resolution_clock::now();

			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
			datum.push_back(elapsed);
		}

		graph.push_back(datum);

		B += interval;
	}

	return graph;
}

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_