#include <vector>

#include <NTL/ZZ.h>

#include "../src/elements/element.hpp"
#include "../src/algorithms/utils.hpp"

/* Example: using the remainder tree algorithm to compute central trinomial
 * coefficients modulo squared primes.
 *
 * The n'th central trinomial coefficient, denoted a_n is the coefficient
 * of x^n in (1+x+x^2)^n. It is also the number of permutations of n symbols
 * taken from {-1, 0, 1} which sum to 0. The a_n satisfy the following identity:
 * a_p = 1 (mod p) for all primes p, and are given by the following recurrence:
 * a_n = [(2*n - 1)*a_{n-1} + 3*(n - 1)*a_{n-2}]/n, with a_0 = a_1 = 1.
 *
 * Suppose we wish to find primes p for which a_p = 1 (mod p^2) using remainder tree.
 * Let R_1 be the 2x2 matrix with ones in the top row and zeros in the bottom. For n>1, define
 *
 *       [2*n - 1,  n]
 * R_n = [3*(n-1),  0]
 *
 * Now, let M_1 = R_1. For n > 1, define
 *
 *       [a_n, a_{n-1}]
 * M_n = [0,         0]  
 *
 * Now observe n! * M_n = (R_n)! Our goal is to find
 * a_2 (mod 4), a_3 (mod 9), a_5 (mod 25)... Using remainder tree,
 * we can find n! * M_n (mod n) for a range of values of n, then divide each by n!
 * However, a similar problem to the Wolstenholme case arises: we cannot divide by n modulo n
 * We will employ a similar solution (see wolstenholme.hpp for more explanation)
 *
 * For primes p, we will use remainder tree to compute (R_p)! (mod p^3), take the
 * upper left entry only, divide it by p, and then reduce modulo p^2.
 * With another remainder tree, separately compute the rest of the denominator, (n-1)! modulo p^2,
 * find its modular inverse, and multiply to get the final result: a_p (mod p^2)
 *
 * For the first remainder tree, let A_0 = Id, and A_n = R_n.
 * The moduli m_n will be n^3 when n is prime, and 1 otherwise.
 * For the second remainder tree, let A_0 = A_1 = 1, and then A_n = n-1. The moduli
 * are now just n^2 when n is prime, and 1 otherwise.
 * Divide each of the upper left entries in the output by their index, then reduce modulo p^2,
 * and multiply by the modular inverse of the corresponding output in the second remainder tree.
 */


/* Like the Kurepa example, we will need to use a matrix of integers for this problem.
 * Thankfully, we still do not have to deal with using polynomials.
 */
using NTL::ZZ;
using NTL::Mat;
//TODO: specialize methods for Elt<Mat<ZZ> >, including modding by ZZ

vector<Elt<Mat<ZZ> > > gen_trinomial_numerator(long lower, long upper) {
    vector<Elt<Mat<ZZ> > > output(upper-lower);

    for(long i = lower; i < upper; i++) {
        Mat<ZZ> M;
        M.SetDims(2,2);

        if (i == 0) {
            M[0][0] = ZZ(1);
            M[0][1] = ZZ(0);
            M[1][0] = ZZ(0);
            M[1][1] = ZZ(1);
            output[i] = Elt<Mat<ZZ> >(M);
        }

        else if (i == 1) {
            M[0][0] = ZZ(1);
            M[0][1] = ZZ(1);
            M[1][0] = ZZ(0);
            M[1][1] = ZZ(0);
            output[i] = Elt<Mat<ZZ> >(M);
        }

        else {
            M[0][0] = ZZ(2*i - 1);
            M[0][1] = ZZ(i);
            M[1][0] = ZZ(3*(i - 1));
            M[1][1] = ZZ(0);
            output[i-lower] = Elt<Mat<ZZ> >(M);
        }
    }
    return output;
}

vector<Elt<ZZ>> gen_trinomial_denominator(long lower, long upper){
    vector<Elt<ZZ>> output(upper-lower);

    for(long i = lower; i < upper; ++i) {
        if(i <= 1){
            output[i] = Elt<ZZ>(1);
        }
        else {
            output[i-lower] = Elt<ZZ> (i - 1);
        }
    }
    return output;
}


vector<Elt<ZZ>> gen_second_prime_power(long lower, long upper) {
    vector<Elt<ZZ>> output(upper-lower);
    
    for(long i = lower; i < upper; i++){
        NTL::ZZ n(i);
        if(ProbPrime(n)) { //Technically a sieve is faster & more correct, but shouldn't make a big difference.
                            //This is just an example anyway.
            power(n, n, 2);
            output[i-lower] = Elt<ZZ>(n);
        }
        else{
            output[i-lower] = Elt<ZZ>(1);
        }
    }
    return output;
}

vector<Elt<ZZ>> gen_third_prime_power(long lower, long upper) {
    vector<Elt<ZZ>> output(upper-lower);
    
    for(long i = lower; i < upper; i++){
        ZZ n(i);
        if(ProbPrime(n)) { //Technically a sieve is faster & more correct, but shouldn't make a big difference.
                            //This is just an example anyway.
            power(n, n, 3);
            output[i-lower] = Elt<ZZ>(n);
        }
        else{
            output[i-lower] = Elt<ZZ>(1);
        }
    }
    return output;
}


//TODO: combine the above and actually write a search function that zips the outputs and finds XGCD etc.
//TODO: explain how to modify calculate_factorial and compute V