#include <vector>

#include <NTL/ZZ.h>

#include "../src/elements/element.hpp"
#include "../src/algorithms/utils.hpp"

/* Example: using the remainder tree algorithm to search for Wolstenholme primes.
 * Wolstenholme's theorem states that choose(2*p - 1, p - 1) = 1 (mod p^3) for prime p.
 * A Wolstenholme prime satisfies the same identity modulo p^4.
 *
 * Suppose we want to use the remainder tree algorithm to aid our search for Wolstenholme primes.
 * Let w_p = choose(2*p - 1, p - 1). We wish to find w_p (mod p^4) for primes p. Define R_1 = 1
 *
 * R_n = 4*n - 2
 *
 * Observe that w_p = ((4*p - 2)/p)*w_{p-1}. Therefore,
 * (R_1 * R_2 ... * R_p)/p! will be w_p
 * Remember the goal is to find w_p (R_p)!/p! (mod p^4). Unfortunately,
 * p! contains a single factor of p which has no inverse modulo p^4.
 *
 * In general to find (n/d) (mod m) where d and m are not coprime, use the following.
 * Suppose q_0*m + r_0 = n/d. For some q_0 and 0 <= r_0 < m.
 * Then multiplying gives q_0*m*d + r_0*d = n
 * If we compute n modulo (m*d), we get q_1*m*d + r_1 = n. Since 0 <= r_0 < m implies 0 <= r_0*d < m*d,
 * then r_1 = r_0*d. The goal is to find r_0, and we do so by computing r_1 and dividing by d.
 *
 * In our case, we should find (R_p)! (mod p^5) and then divide the output by p.
 * Then, reduce modulo p^4 and multiply by the modular inverse of (p-1)!.
 *
 * We will need to do remainder tree twice. The first time, set the multiplicands to be
 * A_0 = 1, and A_n = R_n. Set the moduli to be m_n = n^5 if n is prime, and m_n is 1 otherwise.
 * Then, for each index n in the output, divide the remainder by n, and then reduce modulo (n^4).
 *
 * The second remainder tree, set the multiplicands to be A_0 = A_1 = 1, and A_n = n - 1 and set the
 * moduli to be m_n = n^4 if n is prime, and m_n is 1 otherwise. Then, for each index in the output,
 * set each remainder to be its modular inverse, then multiply by the corresponding index from the first
 * remainder tree. The final vector will contain w_p (mod p^4) for primes p, as desired.
 */


using std::vector;

// Like in the search for Wilson primes, we only need to be able to use integers. See
// wilson.hpp for more explanation on how Elt works and how to wrap types.
using NTL::ZZ;

//The convention here is to generate from lower bound---inclusive to upper---exclusive
vector<Elt<ZZ>> gen_wolstenholme_numerator(long lower, long upper) {
    vector<Elt<ZZ>> output(upper-lower);

    for(long i = lower; i < upper; ++i) {
        if(i <= 1){
            output[i] = Elt<ZZ>(1);
        }
        else {
            //If you worry that the initializer (4*n - 2) in this case will already overflow,
            //Just initialize on i and do arithmetic there.
            output[i-lower] = Elt<ZZ> (4*i - 2);
        }
    }
    return output;
}

vector<Elt<ZZ>> gen_wolstenholme_denominator(long lower, long upper){
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


vector<Elt<ZZ>> gen_fourth_prime_power(long lower, long upper) {
    vector<Elt<ZZ>> output(upper-lower);
    
    for(long i = lower; i < upper; i++){
        ZZ n(i);
        if(ProbPrime(n)) { //Technically a sieve is faster & more correct, but shouldn't make a big difference.
                            //This is just an example anyway.
            NTL::power(n, n, 4);
            output[i-lower] = Elt<ZZ>(n);
        }
        else{
            output[i-lower] = Elt<ZZ>(1);
        }
    }
    return output;
}


vector<Elt<ZZ>> gen_fifth_prime_power(long lower, long upper) {
    vector<Elt<ZZ>> output(upper-lower);
    
    for(long i = lower; i < upper; i++){
        ZZ n(i);
        if(ProbPrime(n)) {
            power(n, n, 5);
            output[i-lower] = Elt<ZZ>(n);
        }
        else {
            output[i-lower] = Elt<ZZ>(1);
        }
    }
    return output;
}


//TODO: combine the above and actually write a search function that zips the outputs and finds XGCD etc.
//TODO: explain how to modify calculate_factorial and compute V