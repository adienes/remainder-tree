#include <NTL/ZZ.h>
#include "elements/Element.hpp"
#include <vector>
#include <string>
#include "algorithms/RemTreeBasic.hpp"

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

	vector< Elt<NTL::ZZ> > ans = remainder_tree< Elt<NTL::ZZ>, Elt<NTL::ZZ> >(A, m);
	
	std::cout << std::endl;
	for (auto&& x: ans)
	{
		std::cout << x.t << std::endl;
	}

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
}