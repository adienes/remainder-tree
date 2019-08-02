#include <vector>
#include <functional>
#include <cassert>

#include "utils.hpp"
#include "rem_forest.hpp"
#include "intermediate_computation.hpp"

using std::vector;


template <typename T, typename U>
vector<std::function<vector<T> ()>> chunkify(const std::function<vector<T> (long, long)>& A_gen, // lower and upper bounds are inclusive-exclusive
											const std::function<vector<U> (long, long)>& m_gen, // lower and upper bounds are inclusive-exclusive
											const std::function<T (long, U, const std::function<vector<T> (long, long)>&, const PolyMatrix&)>& V_gen,
											long lower_bound, long upper_bound, long chunk_size,
											long forest_param, long recompute_param, const PolyMatrix& formula) {


	assert (is_power2(upper_bound-lower_bound) && is_power2(chunk_size));
	assert (chunk_size <= upper_bound-lower_bound);


	vector<std::function<vector<T> ()>> chunk_generators();
	chunk_generators.reserve((upper_bound-lower_bound)/chunk_size);
    
	long _UB = lower_bound+chunk_size; //UB stands for upper bound
	long _LB = lower_bound;

	while (_UB <= upper_bound) { // TODO: calculate the extra bit between last valid _UB and upper_bound
		vector<T> _A = A_gen(_LB, _UB);
		vector<U> _m = m_gen(_LB, _UB);

		U _Y = compute_product_node<U> (_m, 1);
		T _V = V_gen<T,U>(_LB, _Y, A_gen, formula);

	
		std::function<vector<T> ()> chunk_func = std::bind(remainder_forest<T,U>, _A, _m, forest_param, recompute_param, _V, _Y);

		chunk_generators.push_back(chunk_func);

		_UB += chunk_size;
		_LB += chunk_size;
	}

	return chunk_generators;
}
