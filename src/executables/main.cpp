#include <vector>
#include <functional>
#include <random>
#include <chrono>

#include <NTL/ZZ.h>

#include "../elements/element.hpp"
#include "../algorithms/rem_forest.hpp"
#include "../algorithms/rem_chunk.hpp"
#include "../algorithms/utils.hpp"

#include "../algorithms/tree_io.hpp"

using std::vector;
using NTL::ZZ;

vector<Elt<ZZ>> A_main(long lower, long upper){
    vector<Elt<ZZ>> output(upper-lower);
    
    for(long i = lower; i < upper; i++){
        if(i == 0){
            output[0] = Elt<ZZ>(1);
        }
        else{
            output[i-lower] = Elt<ZZ>(i);
        }
    }
    return output;
}

vector<Elt<ZZ>> m_main(long lower, long upper){
    vector<Elt<ZZ>> output(upper-lower);
    for(long i = lower; i < upper; i++){
        ZZ num(i+1);
        if(ProbPrime(num)){
            output[i-lower] = Elt<ZZ>(num);
        }
        else{
            output[i-lower] = Elt<ZZ>(1);
        }
    }
    return output;
}



int main()
{
	

    
    // std::string line = "x^5+43*x^2-x-9";
    // parse_matrix_entry(line);

    std::function<vector<Elt<ZZ>> (long, long)> A_gen = A_main;
    std::function<vector<Elt<ZZ>> (long, long)> m_gen = m_main;
    
    std::map<long, long> poly  = parse_matrix_entry("x");
    vector<std::map<long, long>> col(1);
    col[0] = poly;
    PolyMatrix mat(1);
    mat[0] = col;

    vector<std::function<vector<Elt<ZZ>> ()>> chunks = chunkify(A_gen, m_gen, 0, 1<<25, 1<<25, 0, 0, mat);

    std::cout << "Finished chunkifying!" << std::endl;

    for(long i = 0, chunks_size = chunks.size(); i < chunks_size; i++){
  		std::cout << " i = " << i << std::endl;
        vector<Elt<ZZ>> chunkreturn = chunks[i]();
        std::cout << "got chunkreturn! it is:" << std::endl;;
        print_row(chunkreturn);

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