#include <vector>
#include <functional>
#include <random>
#include <chrono>

#include <NTL/ZZ.h>

#include "../elements/element.hpp"
#include "../algorithms/rem_forest.hpp"
#include "../algorithms/rem_chunk.hpp"

using std::vector;
using NTL::ZZ;

vector<Elt<ZZ>> A_main(long lower, long upper){
    vector<Elt<ZZ>> output(upper-lower);
    
    if (lower < upper) {
    	output[0] = Elt<ZZ>(1);
    }
    for(long i = lower+1; i < upper; i++){
        output[i-lower] = Elt<ZZ>(i);
    }
    return output;
}

vector<Elt<ZZ>> m_main(long lower, long upper){
    vector<Elt<ZZ>> output(upper-lower);
    for(long i = lower; i < upper; i++){
        ZZ num(i+1);
        if(ProbPrime(num)){
            output[i-lower] = Elt<ZZ>(i+1);
        }
        else{
            output[i-lower] = Elt<ZZ>(1);
        }
    }
    return output;
}



int main()
{
	
    std::function<vector<Elt<ZZ>> (long, long)> A_gen = A_main;
    std::function<vector<Elt<ZZ>> (long, long)> m_gen = m_main;
    
    vector<std::function<vector<Elt<ZZ>> ()>> chunks = chunkify(A_gen, m_gen, 0, 32, 32, 0, 0);

    for(long i = 0, chunks_size = chunks.size(); i < chunks_size; i++){
        vector<Elt<ZZ>> chunkreturn = chunks[i]();
        for(long j = 0, chunkreturn_size = chunkreturn.size(); j < chunkreturn_size; j++){
            std::cout << chunkreturn[j] << std::endl;
        }
    }
    /*
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
    */
}
