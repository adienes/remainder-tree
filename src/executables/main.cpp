#include <vector>
#include <functional>

#include "../algorithms/intermediate_computation.hpp"
#include "../algorithms/rem_forest.hpp"
#include "../searches/benchmarks.hpp"
#include "../algorithms/utils.hpp"
#include "../algorithms/tree_io.hpp"
#include "../searches/complexity.hpp"
#include "../algorithms/rem_tree.hpp"

#include "../elements/Element.hpp"

#include <NTL/ZZ.h>


int main()
{

	std::vector<Elt<NTL::ZZ>> A = {1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	std::vector<Elt<NTL::ZZ>> m = {1, 2, 3, 1, 5, 1, 7, 1, 1, 1, 11, 1, 13, 1, 1, 1};

	auto mTree = subproduct_tree(m);
	auto stree = subproduct_tree_askew(A, mTree);

	Elt<NTL::ZZ> V = Elt<NTL::ZZ>(14);
	std::vector<Elt<NTL::ZZ>> parent_layer;
	parent_layer = {V};

	std::vector<Elt<NTL::ZZ>> ans = remainder_forest<Elt<NTL::ZZ>, Elt<NTL::ZZ>>(A, m, 0, 2);

	std::cout << "\n";

	print_tree_formatted(mTree);

	print_tree_formatted(stree);

	print_row(ans);
	print_row(remainder_tree_basic(A,m));

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