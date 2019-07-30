#include <vector>
#include <functional>

#include "../algorithms/intermediate_computation.hpp"
#include "../algorithms/rem_forest.hpp"
#include "../searches/benchmarks.hpp"
#include "../algorithms/utils.hpp"
#include "../algorithms/tree_io.hpp"
#include "../searches/complexity.hpp"

#include "../elements/Element.hpp"

#include <NTL/ZZ.h>


int main()
{

	std::vector<Elt<int>> A = {1, 1, 2, 3, 4, 5, 6, 7};
	std::vector<Elt<int>> m = {1, 2, 3, 1, 5, 1, 7, 1};

	auto mTree = subproduct_tree(m);
	auto stree = subproduct_tree_askew(A, mTree);

	Elt<int> V = Elt<int>(14);
	std::vector<Elt<int>> parent_layer;
	parent_layer = {V};

	std::vector<Elt<int>> ans = remainder_forest<Elt<int>, Elt<int>>(A, m);
	print_tree_formatted(mTree);

	print_tree_formatted(stree);

	for (auto&& i : ans) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	// std::vector<NTL::ZZ> v;
	// std::cout << log2(v.max_size()) << std::endl;

	// std::vector<std::function< std::vector<bool> (int N)>> searchfns = {constant_slow, random_zz};

	// auto cg = complexity_graph(1<<6, 1<<4, searchfns);

	// for (auto&& i : cg) {

	// 	for (auto&& j : i) {
	// 		std::cout << j << " ";
	// 	}
	// 	std::cout << std::endl;
	// }

	// write_complexity_graph(cg);

	return 1;

}