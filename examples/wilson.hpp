#include <vector>

#include <NTL/ZZ.h>

#include "../src/elements/element.hpp"
#include "../src/algorithms/utils.hpp"

/* Example: using the remainder tree algorithm to search for Wilson primes.
 * This was the first application for the remainder tree algorithm and also is
 * one of the simplest. Wilson's theorem says that for all primes p, the following is true:
 *(p-1)! = -1 (mod p). A Wilson prime satisfies the same identity modulo p^2.
 *
 * Let A_0 = A_1 = 1, and A_n = n-1. Also set m_n to be n if n is prime, and 1 otherwise.
 * Then, if A_0 * A_1 * ... A_p = -1 (mod m_n), then m_n is a Wilson prime.
 */


using std::vector;

/* The first decision to make is what kind of datatype to use. In this case it is clear that we only
 * need to use integers. However, for many applications such as those in polynomial rings or if the
 * recurrence relation of the dividends is of order higher than 1, other datatypes may be desired.
 * Most of the code was written with using NTL in mind. However, we wrap every value in the type Elt.
 * Elt is a templated class object which will forward constructors, operators, etc. to its underlying type.
 * To specialize it for a new type (for example, to optimize its .mulmod() method), create a new file in
 * src/elements and then include it in elt_custom.tpp
 * I recommend specialized the methods rather than the class itself. Take a look at elt_NTL.tpp for an example.
 */
using NTL::ZZ;

//The convention here is to generate from lower bound---inclusive to upper---exclusive
vector<Elt<ZZ>> gen_n(long lower, long upper) {
    vector<Elt<ZZ>> output(upper-lower);

    for(long i = lower; i < upper; ++i) {
        if(i <= 1){
            output[i] = Elt<ZZ>(1);
        }
        else {
            output[i-lower] = Elt<ZZ> (i-1);
        }
    }
    return output;
}


vector<Elt<ZZ>> gen_second_prime_power(long lower, long upper) {
    vector<Elt<ZZ>> output(upper-lower);
    
    for(long i = lower; i < upper; i++){
        ZZ n(i);
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



//TODO: explain how to modify calculate_factorial and compute V