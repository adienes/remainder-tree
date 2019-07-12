#include <NTL/ZZ.h>
#include "../elements/Element.hpp"
#include <vector>
#include <string>
#include "../algorithms/RemTree_basic.hpp"
#include "../algorithms/RemTree_utils.hpp"
#include <random>

using std::vector;


template <typename T, typename U>
int add(int a, int b)
{
	vector< Elt<T> > x(5);
	vector< Elt<U> > y(6);
	return a+b;
}

template <typename X>
int add(int a, int b)
{
	return add<X, X>(a,b);
}

int main()
{
	std::cout << add<NTL::ZZ>(3,4) << std::endl;

	vector< Elt<NTL::ZZ> > A = {1,1,2,3,4,5,6,7};
	vector< Elt<NTL::ZZ> > m = {1,2,3,1,5,1,7,1};

	vector< Elt<NTL::ZZ> > ans = remainder_tree_power2< Elt<NTL::ZZ> >(A, m);
	
	print_tree_formatted(ans);

	// int i = 0;
	// for (int x = 0; x < 5; ++x)
	// {
	// 	std::cout << st[x].t << std::endl;
	// 	st[x].t += i;
	// 	i += 1;
	// 	if (i%2 == 1)
	// 	{
	// 		st[x] = 0;
	// 	}
	// }


	// std::cout << std::endl;
	// for (auto&& x: st)
	// {
	// 	std::cout << x.t << std::endl;
	// }

	long int N = (1<<16);
	vector <Elt<NTL::ZZ> > A_rand (N);
	vector <Elt<NTL::ZZ> > m_rand (N);

	using namespace std;

	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(1, N);

	for(int i = 0; i < N; i++){
		//int bitsize = log2(i+1)+2;

		
		A_rand[i] = dist(mt);
		m_rand[i] = dist(mt);
	}

	auto start = chrono::high_resolution_clock::now();
	vector< Elt<NTL::ZZ> > ans_rand = remainder_tree_power2(A_rand, m_rand);
	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> runtime = finish-start;

	print_tree_formatted(ans_rand);

	cout << endl << runtime.count() << endl;

}