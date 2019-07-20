#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <vector>
#include <functional>
#include "../elements/Element.hpp"

bool is_power2(int);

int flatten(int, int);

/*
 * TODO: this returns a smaller vector of just the subtree rooted at that node.
 * There is probably a way to refactor the code such that you can just put
 * a given node as the root and it considers all the subsequent indices relative
 * to the root.
 * Also, beware that this really only works on full binary trees; I'm not sure
 * how it interacts with incomplete trees.
 */
template <typename T>
std::vector<T> get_subtree (const std::vector<T>&, int);

template<typename T>
std::vector<T> elt_to_base(const std::vector<Elt<T>> &);

template<typename T>
std::vector<Elt<T>> base_to_elt(const std::vector<T>&);


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
std::vector <std::vector<int>> complexity_graph(int, int, std::vector <std::function< std::vector<bool> (int N)>>);

#include "RemTree_utils.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_