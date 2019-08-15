// included by rem_factorial_custom.tpp (in case we want to change the file location)

#include <cassert>

#include <NTL/ZZ_pX.h>
#include <NTL/matrix.h>

#include "../elements/element.hpp"
#include "factorial_engine.hpp"

using NTL::ZZ;
using NTL::ZZ_p;
using NTL::ZZ_pX;
using NTL::Mat;


Elt<ZZ> calculate_factorial(long n, const Elt<ZZ>& m, const std::function<vector<Elt<ZZ>> (long, long)>& get_A, const PolyMatrix& formula){
    if (n == 0) {
        return Elt<ZZ>(1);
    }

    assert(("No formula given!" && not formula.empty()));
    assert(("Element given is not a square matrix." && (formula.size() == formula[0].size())));
    
    // Matrix-type elements
    assert(("ZZ's must have 1x1 matrix formulas." && ((long)formula.size() <= 1)));

    // Do naive_factorial if n is small enough
    if(n < 30000 || formula.size() == 0){
        return naive_factorial<Elt<ZZ>, Elt<ZZ>>(n, m, get_A); // TODO: write naive_factorial
    }
    long maxdeg = 0;
    for(const auto&& term : formula[0][0]){
        if(term.first > maxdeg){
            maxdeg = term.first;
        }
    }
    // Linear and Polynomial-type elements
    if(n < 1000000000 || maxdeg >= 2){
        // TODO: convert polynomial coefficients into ZZ_p and call poly_factorial()        
        // Otherwise, do poly_factorial
        ZZ_pX poly;
        for(const auto&& term : formula[0][0]){
            ZZ_p coeff;
            coeff.init(m.t);
            coeff = term.second;
            SetCoeff(poly, term.first, coeff);
        }
        ZZ_p output;
        output.init(m.t);
        output = poly_factorial(n, m.t, poly); // TODO: write poly_factorial

        Elt<ZZ> output_elt(rep(output));

        return output_elt;
    }
    // Large Linear-type elements
    else{ 
        // TODO: convert polynomial coefficients into Mat<ZZ_p> and call matrix_factorial()
        Mat<ZZ_pX> matrix;
        matrix.SetDims(1, 1);
        ZZ_pX poly;
        for(const auto&& term : formula[0][0]){
            ZZ_p coeff;
            coeff.init(m.t);
            coeff = term.second;
            SetCoeff(poly, term.first, coeff);
        }
        matrix.put(0, 0, poly);
        Mat<ZZ_p> output;
        output.SetDims(1, 1);
        output = matrix_factorial(n, m.t, matrix);
        Elt<ZZ> output_elt(rep(output.get(0, 0)));
        return output_elt;
        
    }
}
