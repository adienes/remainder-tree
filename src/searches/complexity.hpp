#ifndef REMAINDERTREE_SRC_SEARCHES_COMPLEXITY_H_
#define REMAINDERTREE_SRC_SEARCHES_COMPLEXITY_H_

#include <vector>
#include <functional>
#include <string>

/* TODO: It is slightly dangerous to cast milliseconds to int.
 * TODO: Does not make any checks for powers of 2 etc. Please only pass in powers of 2.
 *
 * This function is a bit of a weird one. I'm assuming that passed in is an upper bound,
 * a number of datapoints, and a vector of search functions which take a range and return
 * a vector of bools (found item or not). The search functions passed in might want more
 * arguments than just an upper bound. Use std::bind for this purpose to pass in fixed arguments.
 * Return value is a vector containing the x-value, and then the y-values for each function,
 * plus any other data we may want to collect (dunno what yet) all collected in a vector.
 */
std::vector <std::vector<int>> complexity_graph(int, int, std::vector <std::function< std::vector<bool> (int N)>>);

void write_complexity_graph (const std::vector<std::vector<int>>&, std::string);

#include "complexity.cpp" //TODO: bad style

#endif //REMAINDERTREE_SRC_SEARCHES_COMPLEXITY_H_