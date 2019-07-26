#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEFOREST_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEFOREST_H_

/*
 * Range Segments > Forest > Recompute
 */


#include <vector>

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
 */
template <typename T, typename U>
vector<T> remainder_forest(const std::vector<T>&, const std::vector<U>&, int, T V = T(1), U Y = U(0));

#include "RemTree_forest.tpp"

#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEFOREST_H_