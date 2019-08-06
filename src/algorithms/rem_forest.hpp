#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMFOREST_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMFOREST_H_

/*
 * Range Segments > Forest > Recompute
 */


#include <vector>

//This computes the entire layer rather than a specific value, as there are better ways to
//compute single values of a accumulating remainder tree! (aka the V algorithm)
template <typename T, typename U>
std::vector<T> compute_remainder_tree_layer(const std::vector<T>&, const std::vector<U>&, long, T&, U&, bool update_forest_vals = false);


template <typename T, typename U>
std::vector<T> remainder_tree_in_forest(const std::vector<T>&, const std::vector<U>&, long, T&, U&, bool update_forest_vals = false);

/*
 * Like before, pass in A, m as vectors. Also pass in a starting V and a parameter 'k'
 * which is the level at which to segment the tree. There is some code duplication (grr!)
 * but it is probably the best way to go without sacrificing performance.
 * Still uses vector entry instead of function---like a sieve. I think inputting a function
 * to generate A and m values will only be useful at the highest level, i.e. Range Segments.
 * 
 * Also note that remainder_forest REQUIRES division. This might mean that for certain types of moduli
 * it is not worth it. However it should be good for integers.
 *
 * So how much size to allocate one segment on which RemainderForest is run? The limiting factor
 * is storing the product of all the m and storing the first full trees (unless further space-time tradeoffs are done)
 * 
 * This will chew up the inputted V and Y! so be careful!
 * If you just want to call remainder forest not chunkified, make sure to use the overloaded version below.
 */
template <typename T, typename U>
std::vector<T> remainder_forest(const std::vector<T>&, const std::vector<U>&, long, long, T& V, U& Y);

//Overloaded purposefully. This one passes in V by value and does not need Y. It does not modify any parameters.
//This is the one that should be used 'standalone,' rather than inside chunkify.
template <typename T, typename U>
std::vector<T> remainder_forest(const std::vector<T>&, const std::vector<U>&, long, long, T V = T(1));

//Provides some default arguments (forest_param = 0, recompute_param = 0, V = T(1))
template <typename T, typename U>
std::vector<T> remainder_forest(const std::vector<T>&, const std::vector<U>&);


#include "rem_forest.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMFOREST_H_