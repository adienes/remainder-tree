#include <vector>
#include <functional>

#include "../searches/benchmarks.hpp"
#include "../algorithms/RemTree_utils.hpp"


int main()
{

	std::vector<std::function< std::vector<bool> (int N)>> searchfns = {constant_slow, random_zz};

	auto cg = complexity_graph(1<<22, 1<<7, searchfns);

	for (auto&& i : cg) {

		for (auto&& j : i) {
			std::cout << j << " ";
		}
		std::cout << std::endl;
	}


}