#include <vector>
#include <functional>

#include "../algorithms/intermediate_computation.hpp"
#include "../algorithms/rem_forest.hpp"

#include "../searches/benchmarks.hpp"
#include <random>

#include "../algorithms/utils.hpp"
#include "../algorithms/tree_io.hpp"
#include "../algorithms/rem_tree.hpp"

#include "../elements/Element.hpp"

#include <NTL/ZZ.h>


#include "../searches/benchmarks.hpp"
#include "../searches/complexity.hpp"

#include <chrono>


int main()
{
	int B = (1<<25);

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



	auto start = std::chrono::high_resolution_clock::now();

	vector<Elt<NTL::ZZ>> ans = remainder_forest<Elt<NTL::ZZ>, Elt<NTL::ZZ>>(A_rand, m_rand, 0, 0);
	vector<Elt<NTL::ZZ>> control (1);
	//auto control = remainder_tree_basic(A_rand, m_rand);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
	
	//print_row(ans);
	if (ans == control) {
		std::cout << "VERIFIED TO BE CORRECT" << std::endl;
	}

	std::cout << "TIME TAKEN: " << elapsed << " MILISECONDS" << std::endl;

	// auto temp = random_zz(8);

	// std::vector<std::function< std::vector<bool> (int N)>> searchfns = {random_zz};

	// auto cg = complexity_graph(1<<6, 1<<4, searchfns);

	// for (auto&& i : cg) {

	// 	for (auto&& j : i) {
	// 		std::cout << j << " ";
	// 	}
	// 	std::cout << std::endl;
	// }

	// write_complexity_graph(cg);

	// return 1;

}