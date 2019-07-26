#include <vector>
#include <functional>
#include <cassert>

#include "utils.hpp"
#include "RemTree_forest.hpp"
#include "RemTree_recompute.hpp"

using std::vector;


template <typename T, typename U>
vector<std::function<vector<T> ()>> chunkify(const std::function<vector<T> (int, int)>& A_gen,
											const std::function<vector<U> (int, int)>& m_gen,
											const std::function<T (int, U)>& V_gen,
											int lower_bound, int upper_bound, int chunk_size,
											int forest_param, int recompute_param, T V) {


	assert (is_power2(upper_bound-lower_bound+1) && is_power2(chunk_size));
	assert (chunk_size <= upper_bound-lower_bound+1);


	vector<std::function<vector<T> ()>> chunk_generators();
	chunk_generators.reserve((upper_bound-lower_bound+1)/chunk_size);

	
	int _UB = lower_bound+chunk_size-1; //UB stands for upper bound
	int _LB = lower_bound;

	while (_UB <= upper_bound) {
		vector<T> _A = A_gen(_LB, _UB);
		vector<U> _m = m_gen(_LB, _UB);

		U _Y = compute_product_node<U> (_m, 1);
		T _V = V_gen<T,U>(chunk_LB-1, _Y); //TODO: maybe the -1 should be gone on first parameter

																						//TODO: recompute_param
		std::function<vector<T> ()> chunk_func = std::bind(remainder_forest<T,U>, _A, _m, forest_param, _V, _Y);

		chunk_generators.push_back(chunk_func);

		_UB += chunk_size;
		_LB += chunk_size;
	}

	return chunk_generators;
}