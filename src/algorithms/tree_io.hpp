#ifndef REMAINDERTREE_SRC_ALGORITHMS_TREEIO_H_
#define REMAINDERTREE_SRC_ALGORITHMS_TREEIO_H_

#include <vector>

template <typename T>
void print_row(const std::vector<T>&);

template <typename T>
void print_tree(const std::vector<T>&);

template <typename T>
void print_tree_formatted(const std::vector<T>&);

#include "tree_io.tpp"

#endif // REMAINDERTREE_SRC_ALGORITHMS_TREEIO_H_