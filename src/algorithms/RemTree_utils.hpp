#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <iostream>
#include <vector>
#include <string>

inline int flatten(int, int);

template <typename T>
void print_tree(std::vector<T>&);

template <typename T>
void print_tree_formatted(const std::string&, const std::vector<T>&, int, int, bool);

template <typename T>
void print_tree_formatted(const std::vector<T>&);

#include "RemTree_utils.tpp"

#endif // REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_