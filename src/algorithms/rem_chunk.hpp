#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMCHUNK_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMCHUNK_H_

#include <vector>
#include <functional>
#include "utils.hpp"

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
 */

template <typename T, typename U>
vector<std::function<vector<T> ()>> chunkify(const std::function<vector<T> (long, long)>&,
											const std::function<vector<U> (long, long)>&,
											long, long, long,
											long forest_param = 0, long recompute_param = 0, PolyMatrix formula = {});

#include "rem_chunk.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMCHUNK_H_
