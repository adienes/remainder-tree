#include <vector>
#include <functional>
#include <random>
#include <chrono>

#include <NTL/ZZ.h>

#include "../src/elements/element.hpp"
#include "../src/algorithms/rem_forest.hpp"
#include "../src/algorithms/rem_chunk.hpp"
#include "../src/algorithms/utils.hpp"

#include "../src/algorithms/tree_io.hpp"

#include "wolstenholme.hpp"
#include "kurepa.hpp"
#include "trinomial.hpp"

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
	  power(num, num, 2);
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
    
    PolyMatrix mat = parse_matrix_formula("[x]");

    //vector<std::function<vector<Elt<ZZ>> ()>> chunks = chunkify(A_gen, m_gen, 0, 1<<26, 1<<21, 0, 0, mat);

    vector<std::function<vector<Elt<ZZ> > ()> > numerators = chunkify(gen_wolstenholme_numerator, gen_fifth_prime_power, 0, 1<<24, 1<<19, 0, 0, mat);

    vector<std::function<vector<Elt<ZZ> > ()> > denominators = chunkify(gen_wolstenholme_denominator, gen_fourth_prime_power, 0, 1<<24, 1<<19, 0, 0, mat);
    
    std::cout << "Finished chunkifying!" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    Elt<ZZ>p(1);
    for(long i = 0, chunks_size = chunks.size(); i < chunks_size; i++){
  		std::cout << " i = " << i << std::endl;
        vector<Elt<ZZ> > chunknums = numerators[i]();
	vector<Elt<ZZ> > chunkdenoms = denominators[i]();

	for (auto&& i : chunknums) {
	  i.t /= p.t;

	  ZZ mod;
	  power(mod, p.1, 4);

	  
	  
	}
	for (auto&& i : chunkreturn) {
	  if (i.t+1 == (p*p).t) {
	    std::cout << "Found one! It was: " << p << std::endl;
	  }

	  p.t += 1;
	}
    }


    auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();

	std::cout << "TIME TAKEN: " << elapsed << " MILLISECONDS" << std::endl;
}
