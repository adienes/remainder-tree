#include <vector>
#include <functional>
#include <cassert>

#include "utils.hpp"


template <typename T, typename U>
T calculate_initial(long n, U m, std::function<T (long)> get_A, const PolyMatrix& formula = {}) {
    if(formula == {}){
        return naive_factorial(n, m, get_A); // TODO: include factorial formulas
    }
    
    assert(("Element given is not a square matrix.", formula.size() == formula[0].size()));
    
    // Matrix-type elements
    if(formula.size() > 1){
        // Check that all elements have degree < 2
        for(long row = 0; row < formula.size(); row++){
            for(long col = 0; col < formula.size(); col++){
                assert(("Matrix element cannot have non-linear terms.", formula[row][col].size() < 2));
            }
        }
        
        // TODO: convert PolyMatrix to Mat<ZZ_p> and call naive_factorial() and matrix_factorial()
    }
    // Polynomial-type elements
    else if(formula[0][0].size() >= 2){
        // TODO: convert polynomial coefficients into ZZ_p and call naive_factorial() and poly_factorial()
    }
    // Linear-type elements
    else if(formula[0][0].size() < 2){
        // TODO: convert polynomial coefficients into ZZ_p and Mat<ZZ_p> and call naive_factorial(), poly_factorial(), and matrix_factorial()
    }
}


// #include "custom_factorial.tpp"
