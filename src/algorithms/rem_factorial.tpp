#include "intermediate_computation.hpp"

#include "utils.hpp"

#include <NTL/ZZ_pX.h>
#include <NTL/matrix.h>
#include "../elements/element.hpp"


template <typename T, typename U>
T calculate_factorial(long n, const U& m, const std::function<std::vector<T> (long, long)>& get_A, const PolyMatrix& formula) {
    // return compute_product_node<T, U>(get_A(0, n), m, 1);
    if (n == 0) {
		return T(1)%m;
	}

	(void)formula; //just to silence unused variable warning
	return compute_product_node<T,U>(get_A(0,n), m, 1);
}


#include "rem_factorial_custom.tpp"
