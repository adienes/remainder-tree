#include <vector>
#include <functional>
#include <random>
#include <chrono>

#include <NTL/ZZ.h>

#include "../src/elements/element.hpp"
#include "../src/algorithms/rem_forest.hpp"

#include "../src/algorithms/rem_chunk.hpp"

//std::function<vector<T> ()> chunk_funk = [&]() -> vector<T> { return remainder_forest<T, U> (_A, _m, forest_param, recompute_param, _V, _Y); };

int main()
{
	long B = (1<<19);

	vector <Elt<NTL::ZZ> > A_rand (B);
	vector <Elt<NTL::ZZ> > m_rand (B);

	using namespace std;

	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<long> dist(1, B);

	for(long i = 0; i < B; i++){
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

	// std::vector<std::function< std::vector<bool> (long N)>> searchfns = {random_zz};

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