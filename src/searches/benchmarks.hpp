#ifndef REMAINDERTREE_SRC_SEARCHES_BENCHMARKS_H_
#define REMAINDERTREE_SRC_SEARCHES_BENCHMARKS_H_

//std::function< std::vector<bool> (int N)> remember this is what is being passed in!
#include <vector>


std::vector<bool> constant_slow(int);

std::vector<bool> random_zz(int);

#include "benchmarks.cpp" //TODO: this is horrible form! not necessary as before because not templates

#endif //REMAINDERTREE_SRC_SEARCHES_BENCHMARKS_H_