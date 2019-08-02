#include <vector>
#include <functional>
#include <cassert>

#include "utils.hpp"
#include "factorial_algs.cpp"

#include <NTL/matrix.h>
#include <NTL/ZZ_pX.h> // includes ZZ_p.h and ZZ.h

using NTL::Mat;
using NTL::ZZ_p;
using NTL::ZZ_pX;


template <typename T, typename U>
T calculate_initial(long n, U m, std::function<vector<T> (long, long)> get_A, const PolyMatrix& formula = {}) {
    return naive_factorial(n, m, get_A); // TODO: replace temporary measure with something better

/*
    if(formula == {}){
        return naive_factorial(n, m, get_A); // TODO: include factorial formulas
    }
    
    assert(("Element given is not a square matrix.", formula.size() == formula[0].size()));
    
    // Matrix-type elements
    if(formula.size() > 1){
        // Convert PolyMatrix to Mat<ZZ_pX> and call naive_factorial() and matrix_factorial()

        // Do naive factorial if n is small enough
        if(n < 200000){
            return naive_factorial(n, m, get_A);
        }

        // Do matrix factorial
        long dim = formula.size();
        // Check that all elements have degree < 2
        for(long row = 0; row < dim; row++){
            for(long col = 0; col < dim; col++){
                assert(("Matrix element cannot have non-linear terms.", formula[row][col].size() < 2));
            }
        }
        
        Mat<ZZ_pX> formula_matrix;
        // get_formula_matrix(formula_matrix, dim, formula);
        
        
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


void get_formula_matrix(Mat<ZZ_pX>& out, long dim, const PolyMatrix& formula){
    out.SetDims(dim, dim);
    for(long row = 0; row < dim; row++){
        for(long col = 0; col < dim; col++){
            ZZ_pX poly;
            for(long exp = 0; exp < formula[row][col].size(); exp++){
                ZZ_p coeff;
                coeff.init(m); // TODO: make m into a ZZ somehow :(
                coeff = 0;
                for(long digit = formula[row][col][exp].size()-1; digit >= 0; digit--){
                    coeff *= 2;
                    coeff += formula[row][col][exp][digit] ? 1 : 0;
                }
                SetCoeff(poly, exp, coeff);
            }
            out.put(poly, row, col);
        }
    }
*/
}


// #include "custom_factorial.tpp"
