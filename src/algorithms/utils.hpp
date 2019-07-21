#ifndef REMAINDERTREE_SRC_ALGORITHMS_UTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_UTILS_H_

#include <vector>
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


#include "utils.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_UTILS_H_