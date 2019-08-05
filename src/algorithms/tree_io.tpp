#include <iostream>
#include <vector>
#include <string>

#include "utils.hpp"

template <typename T>
void print_row(const std::vector<T>& row) {
	for (auto&& i : row) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
}


template <typename T>
void print_tree(const std::vector<T>& tree) {
	long long nextrightmost = 1;
	long long N = tree.size();

	for(long long i = 1; i < N; ++i) {
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

//for internal use only
template <typename T>
void _print_tree_formatted(const std::string& prefix, const std::vector<T>& tree, long long i, long long j, bool isLeft)
{

	long long loc = flatten(i,j);
	long long N = tree.size();
	if (loc >= N) { return; }

    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──" );

    // print the value of the node
    std::cout << tree[flatten(i,j)] << std::endl;

    // enter the next tree level - left and right branch
    _print_tree_formatted( prefix + (isLeft ? "│   " : "    "), tree, i+1, 2*j, true);
    _print_tree_formatted( prefix + (isLeft ? "│   " : "    "), tree, i+1, 2*j+1,false);
}

template <typename T>
void print_tree_formatted(const std::vector<T>& tree)
{
    _print_tree_formatted<T>("", tree, 0, 0, false);
}