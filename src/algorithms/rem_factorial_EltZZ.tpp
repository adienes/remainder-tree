// included by rem_factorial_custom.tpp (in case we want to change the file location)

#include "factorial_engine.hpp"
#include <NTL/ZZ_pX.h>
#include <NTL/matrix.h>
#include "../elements/element.hpp"

using NTL::ZZ;
using NTL::ZZ_p;
using NTL::ZZ_pX;
using NTL::Mat;


template<>
Elt<ZZ> calculate_factorial(long n, const Elt<ZZ>& m, const std::function<vector<Elt<ZZ>> (long, long)>& get_A, const PolyMatrix& formula){
    assert(("Element given is not a square matrix.", formula.size() == formula[0].size()));
    
    // Matrix-type elements
    assert(("ZZ's must have 1x1 matrix formulas.", formula.size() <= 1));    
    
    // Do naive_factorial if n is small enough
    if(n < 30000){
        return naive_factorial<Elt<ZZ>, Elt<ZZ>>(n, m, get_A); // TODO: write naive_factorial
    }

    // Linear and Polynomial-type elements
    else if(n < 1000000000 || formula[0][0].size() >= 2){
        // TODO: convert polynomial coefficients into ZZ_p and call poly_factorial()
        
        
        // Otherwise, do poly_factorial
        ZZ_pX poly;
        for(long exp = 0; exp < formula[0][0].size(); exp++){                                                                                                     
            ZZ_p coeff;
            coeff.init(m.t);
            coeff = 0;
            for(long digit = formula[0][0][exp].size()-1; digit >= 0; digit--){
                coeff *= 2;
                coeff += formula[0][0][exp][digit] ? 1 : 0;
            }
            SetCoeff(poly, exp, coeff);
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
        for(long exp = 0; exp < formula[0][0].size(); exp++){
            ZZ_p coeff;
            coeff.init(m.t); 
            coeff = 0;
            for(long digit = formula[0][0][exp].size()-1; digit >= 0; digit--){
                coeff *= 2;
                coeff += formula[0][0][exp][digit] ? 1 : 0;
            }
            SetCoeff(poly, exp, coeff);
        }
        matrix.put(0, 0, poly);

        Mat<ZZ_p> output;
        output.SetDims(1, 1);
        output = matrix_factorial(n, m.t, matrix);
        Elt<ZZ> output_elt(rep(output.get(0, 0)));

        return output_elt;
        
    }
}
