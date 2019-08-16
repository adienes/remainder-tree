#include <vector>

#include <NTL/ZZ.h>

#include "../src/elements/element.hpp"
#include "../src/algorithms/utils.hpp"

/* Example: using the remainder tree algorithm to search for a counterexample to
 * Kurepa's conjecture.
 *
 * The left factorial function is the sum of lesser factorials:
 * !n = 0! + 1! + ... + (n-1)!
 *
 * Suppose we wish to find primes p for which p divides !p.
 *
 *       [n,  1]
 * R_n = [0,  1]
 *
 * Observe that !n is the top right entry of R_1 * R_2 ...* R_n
 * Our goal is to find !2 (mod 2), !3 (mod 3), !5 (mod 5) ...
 * Using remainder tree, set the moduli m_n to be n if n prime, 1 otherwise,
 * and set A_0 = Id, A_n = R_n. Apply remainder tree with the A, m. Then, at each index
 * take only the top right entry of the resulting matrix. If 0 appears in the output
 * for a prime index, then a counterexample has been found.
 */

using std::vector;

/* Unlike the Wolstenholme example (see it for more details), we need to use
 * a matrix datatype. Thankfully NTL can provide one for us.
 */
using NTL::ZZ;
using NTL::Mat;
//TODO: specialize methods for Elt<Mat<ZZ> >, including modding by ZZ

vector<Elt<Mat<ZZ> > > gen_kurepa_multiplicand(long lower, long upper) {
    vector<Elt<Mat<ZZ> > > output(upper-lower);

    for(long i = lower; i < upper; i++) {
        Mat<ZZ> M;
        M.SetDims(2,2);

        if(i == 0) {
            M[0][0] = ZZ(1);
            M[0][1] = ZZ(0);
            M[1][0] = ZZ(0);
            M[1][1] = ZZ(1);
            output[i] = Elt<Mat<ZZ> >(M);
        }
        else {
            M[0][0] = ZZ(i);
            M[0][1] = ZZ(1);
            M[1][0] = ZZ(0);
            M[1][1] = ZZ(1);
            output[i-lower] = Elt<Mat<ZZ> >(M);
        }
    }
    return output;
}

vector<Elt<ZZ>> gen_prime(long lower, long upper) {
    vector<Elt<ZZ>> output(upper-lower);
    
    for(long i = lower; i < upper; i++){
        ZZ n(i);
        if(ProbPrime(n)) { //Technically a sieve is faster & more correct, but shouldn't make a big difference.
                            //This is just an example anyway.
            output[i-lower] = Elt<ZZ>(n);
        }
        else{
            output[i-lower] = Elt<ZZ>(1);
        }
    }
    return output;
}


//TODO: combine the above and actually write a search function that takes the top right entry of each matrix
//TODO: explain how to modify calculate_factorial and compute V