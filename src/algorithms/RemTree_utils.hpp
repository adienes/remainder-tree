#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_

#include <iostream>
#include <vector>

inline int flatten(int i , int j) {
	return (1<<i)+j;
}

template <typename T>
void print_tree(std::vector<T> tree) {
	int nextrightmost = 1;
	int N = tree.size();

	for(int i = 1; i < N; ++i) {
		std::cout << tree[i];

		if (i == nextrightmost) {
			nextrightmost <<= 1;
			nextrightmost++;

			std::cout << std::endl;
		}

		else {
			std::cout << " ";
		}
	}

	std::cout << std::endl;
}


#endif // REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_