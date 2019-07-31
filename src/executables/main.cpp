#include <vector>
#include <functional>

#include "../algorithms/intermediate_computation.hpp"
#include "../algorithms/rem_forest.hpp"

//#include "../searches/benchmarks.hpp"
#include <random>

#include "../algorithms/utils.hpp"
#include "../algorithms/tree_io.hpp"
#include "../searches/complexity.hpp"
#include "../algorithms/rem_tree.hpp"

#include "../elements/Element.hpp"

#include <NTL/ZZ.h>


int main()
{
	int B = (1<<6);

	vector <Elt<NTL::ZZ> > A_rand (B);
	vector <Elt<NTL::ZZ> > m_rand (B);

	using namespace std;

	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(1, B);

	for(int i = 0; i < B; i++){
		//int bitsize = log2(i+1)+2;	
		A_rand[i] = dist(mt);
		m_rand[i] = dist(mt);
	}

	std::vector<Elt<NTL::ZZ>> ans = remainder_forest<Elt<NTL::ZZ>, Elt<NTL::ZZ>>(A_rand, m_rand, 5, 1);

	print_row(ans);
	print_row(remainder_tree_basic(A_rand,m_rand));

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