#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREECHUNK_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREECHUNK_H_

#include <vector>
#include <functional>
#include <cassert>

#include "utils.hpp"
#include "RemTree_forest.hpp"
#include "RemTree_recompute.hpp"


/*
 * Inputs: a function that generates A in a given range, a function that generates m in a given range,
 * a function which computes the value of V for later chunks, the total range of values to compute,
 * the initial V, the size of each chunk, forest parameter, and the recompute parameter, along with maybe later parameters
 * to be added (use permanent memory, checkpoints, etc?)
 * 
 * Note that both chunk_size and upper_bound-lower_bound+1 must be powers of 2
 *
 * Output: a vector of function pointer which, when run, return a vector of remainders for their assigned range
 * TODO: Maybe I should attach a value of how large each vector of remainders is.
 * TODO: Uses only ints, and is likely to overflow!
 */

template <typename T, typename U>
vector<std::function<vector<T> ()>> chunkify(const std::function<vector<T> (int, int)>&,
											const std::function<vector<U> (int, int)>&,
											const std::function<T (int, U)>&,
											int, int, int,
											int forest_param = 0, int recompute_param = 0, T V = T(1));

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREECHUNK_H_