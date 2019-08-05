//std::function< std::vector<bool> (long N)> remember this is what is being passed in!
#include <NTL/ZZ.h>

#include "../elements/Element.hpp"
#include "../algorithms/rem_forest.hpp"

#include <vector>
#include <random>

using std::vector;

vector<bool> constant_slow(long B) {
	long n = 1;
	for (long i = 0; i < 10*B; ++i) { n = (((n*2+17)/3-19)*5-101); } //just gibberish

	vector<bool> incidence;
	bool ans = (B+n)%5;

	incidence.push_back(ans);
	return incidence;
}


vector<bool> random_zz(long B, long forest_param, long recompute_param) {

	vector <Elt<NTL::ZZ> > A_rand (B);
	vector <Elt<NTL::ZZ> > m_rand (B);

	//using namespace std;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<long> dist(1, B);

	for(long i = 0; i < B; ++i){
		//long bitsize = log2(i+1)+2;
		A_rand[i] = dist(mt);
		m_rand[i] = dist(mt);
	}

	remainder_forest(A_rand, m_rand, forest_param, recompute_param);

	vector<bool> ret(1);
	return ret;
}