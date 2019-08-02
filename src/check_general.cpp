#include <iostream>
#include <vector>
#include <functional>
#include <cassert>
#include <cmath>
#include <chrono>
#include <NTL/matrix.h>
#include <NTL/ZZ_pX.h> // includes ZZ_p.h and ZZ.h as well

using std::vector;
using namespace std::chrono;
using NTL::Mat;
using NTL::ZZ_p;
using NTL::ZZ_pX;
using NTL::ZZ;


// TODO: add return type to final algorithm

// Reference: https://www.dropbox.com/home/urop2019/literature?preview=Bostan%2C+Gaudry%2C+Schost+-+Linear+recurrences+with+polynomial+coefficients+and+application+to+integer+factorization+and+Cartier-Manin+operator.pdf
// Page 1781 (Lemma 1)
void invert_all(vector<ZZ_p> &out, vector<ZZ_p> &a);
// Page 1781 (Lemma 2)
void find_delta(vector<ZZ_p> &out, ZZ &p);
void find_delta(vector<ZZ_p> &out, ZZ &p, ZZ_p &a);
// Page 1784-5 (Theorem 5)
void shift_values(vector<ZZ_p> &out, vector<ZZ_p> &values, ZZ_p &a, ZZ_p &b, ZZ &p);
void shift_values(vector<Mat<ZZ_p>> &out, vector<Mat<ZZ_p>> &values, ZZ_p &a, ZZ_p &b, ZZ &p);
// Page 1786-8 (Section 4)
void multieval_prod(vector<Mat<ZZ_p>> &out, std::function<void (Mat<ZZ_p>&, ZZ_p&)> A, ZZ &p);
void multieval_prod(vector<Mat<ZZ_p>> &out, ZZ_p &k, std::function<void (Mat<ZZ_p>&, ZZ_p&)> A, ZZ &p);
// Page 1792 (Equation (5))
void matrix_factorial(Mat<ZZ_p> &out, long n, std::function<void (Mat<ZZ_p>&, ZZ_p&)> A, ZZ &p);
// Following functions for testing:
// matrix definition
void A(Mat<ZZ_p> &out, ZZ_p &x);
// print contents of a vector
template <typename T>
void print(vector<T> vec);


/*
 * Evaluate M(x), returned in out
 */
void A(Mat<ZZ_p> &out, ZZ_p &x){
    ZZ_p zero;
    zero.init(x.modulus());
    zero = 0;
    ZZ_p one;
    one.init(x.modulus());
    one = 1;
    /*out.SetDims(2, 2);
    out.put(0, 0, x+1);
    out.put(0, 1, zero);
    out.put(1, 0, one);
    out.put(1, 1, one);
    */
    out.SetDims(1, 1);
    out.put(0, 0, x);

}

int main(){
    std::function<void (Mat<ZZ_p>&, ZZ_p&)> A_func = A;

    Mat<ZZ_p> answer;
    for(long i = 1073741824; i <= 1073741824; i*=2){
        ZZ p;
        NextPrime(p, ZZ(i));
        long p1;
        conv(p1, p-1);
        mul(p, p, p);
        uint64_t start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        matrix_factorial(answer, p1, A_func, p);
        uint64_t time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - start;

        std::cout << "final answer: " << answer << std::endl;
        //std::cout << time << ", ";
    }
    std::cout << std::endl;
    
}

template<typename T>
void print(vector<T> vec){
    for(long i = 0; i < vec.size(); i++){
        std::cout << vec[i];
        if(i < vec.size()-1){
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

/* 
 * Steps:
 * [DONE] Create function that take inputs a0, a1, ..., an-1 and outputs 1/a0, ..., 1/an-1
 * [DONE] Create function that takes input d and outputs 1/prod(0-j), 1/prod(1-j), ..., 1/prod(d-j)
 * [DONE] Create function that takes input a, d and outputs prod(a+0-j), prod(a+1-j), ..., prod(a+d-j)
 * [DONE] Create function that take inputs M(a), M(a+b), ..., M(a+kb) and outputs M(a+i), M(a+b+i), ..., M(a+kb+i)
 * [DONE] Create function to calculate M_k(0), M_k(k), ..., M_k(k^2)
 * [DONE] Create main function to calculate M(1)...M(k^2) = M_k(0)M_k(k)...M_k(k^2-k)
 */


/*
 * Takes a list of elements a and returns their inversions mod p
 */
void invert_all(vector<ZZ_p> &out, vector<ZZ_p> &a){
    assert(out.size() == a.size());

    long n = a.size();
    vector<ZZ_p> inverses(n);

    vector<ZZ_p> accum_prod(n); // a0, a0a1, ..., a0a1...an-1

    accum_prod[0] = a[0];
    for(long i = 1; i < n; i++){
        mul(accum_prod[i], accum_prod[i-1], a[i]);
    }

    inv(accum_prod[n-1], accum_prod[n-1]);
    for(long i = n-1; i > 0; i--){
        mul(inverses[i], accum_prod[i], accum_prod[i-1]); // 1/ai = 1/(a1...ai) * a1...a(i-1)
        mul(accum_prod[i-1], accum_prod[i], a[i]); // a1...a(i-1) -> 1/(a1...a(i-1))
    }
    inverses[0] = accum_prod[0];
    
    out = inverses;
}

/*
 * d = out.size()-1;
 * Returns 1/delta(i, d) = 1/prod(i-j) where the product goes from j=0 to d skipping i.
 */
void find_delta(vector<ZZ_p> &out, ZZ &p){
    long d = out.size() - 1;
    vector<ZZ_p> ints(d);
    for(long i = 0; i < ints.size(); i++){
        ints[i].init(p);
        ints[i] = i+1;
    }
    vector<ZZ_p> inv_ints(d);
    invert_all(inv_ints, ints);
    
    vector<ZZ_p> deltas(d+1);
    deltas[0] = inv_ints[0]; // = 1
    for(long i = 1; i < inv_ints.size(); i++){
        mul(deltas[0], deltas[0], inv_ints[i]);
    } // deltas[0] = 1/d!
    
    if(d%2 == 1){
        deltas[0] = -deltas[0]; // deltas[0] = 1/((-1)^d*d!)
    }

    for(long i = 1; i < deltas.size(); i++){
        mul(deltas[i], deltas[i-1], i-1-d);
        mul(deltas[i], deltas[i], inv_ints[i-1]);
    }

    out = deltas;
}

/*
 * d = out.size()-1;
 * Returns Delta(a, i, d) = prod(a+i-j) where the product goes from j=0 to d.
 */
void find_delta(vector<ZZ_p> &out, ZZ_p &a, ZZ &p){
    long d = out.size() - 1;

    vector<ZZ_p> ints(d);
    for(long i = 0; i < ints.size(); i++){
        ints[i].init(p);
        ints[i] = a-d+i;
    }
    vector<ZZ_p> inv_ints(d);
    invert_all(inv_ints, ints);
    
    vector<ZZ_p> Deltas(d+1);
    Deltas[0] = a;
    for(long i = 0; i < inv_ints.size(); i++){
        mul(Deltas[0], Deltas[0], ints[i]);
    } // Deltas[0] = (a-d)...(a-1)(a)
    for(long i = 1; i < Deltas.size(); i++){
        mul(Deltas[i], Deltas[i-1], a+i);
        mul(Deltas[i], Deltas[i], inv_ints[i-1]);
    }

    out = Deltas;
}

/*
 * Takes in the values of polynomials F(r), F(r+b), ..., F(r+db) and outputs F(r+a), F(r+b+a), ... F(r+db+a)
 * Requires deg(F) <= d to work mathematically, since it will interpolate based on the given evaluations of F.
 */
void shift_values(vector<ZZ_p> &out, vector<ZZ_p> &values, ZZ_p &a, ZZ_p &b, ZZ &p){
    assert(out.size() == values.size());

    long d = values.size() - 1;
    vector<ZZ_p> P(d+1);
    find_delta(P, p);
    
    ZZ_pX Px;
    for(long i = 0; i < values.size(); i++){
        mul(P[i], P[i], values[i]);
        SetCoeff(Px, i, P[i]);
    }

    ZZ_p shift;
    shift.init(p);
    inv(shift, b);
    mul(shift, shift, a); // shift = a/b

    vector<ZZ_p> S(2*d+1);
    for(long i = 0; i < S.size(); i++){
        S[i] = shift + i - d;
    }
    invert_all(S, S);

    ZZ_pX Sx;
    for(long i = 0; i < S.size(); i++){
        SetCoeff(Sx, i, S[i]);
    }   

    ZZ_pX PS;
    mul(PS, Px, Sx);

    find_delta(out, shift, p);
    for(long i = 0; i < out.size(); i++){
        mul(out[i], out[i], coeff(PS, i+d));
    }
}

/*
 * Takes in the values of matrices of polynomials M(r), M(r+b), ..., M(r+db) and outputs M(r+a), M(r+b+a), ... M(r+db+a)
 * Requires the maximal degree of any element of M(x) to be <= d to successfully interpolate
 */
void shift_values(vector<Mat<ZZ_p>> &out, vector<Mat<ZZ_p>> &values, ZZ_p &a, ZZ_p &b, ZZ &p){
    assert(out.size() == values.size());
    assert(values[0].NumRows() == values[0].NumCols());
    for(long i = 0; i < out.size(); i++){
        assert(values[i].NumRows() == values[0].NumRows());
        assert(values[i].NumCols() == values[0].NumCols());
        out[i].SetDims(values[0].NumRows(), values[0].NumCols());
    } 

    for(long row = 0; row < values[0].NumRows(); row++){
        for(long col = 0; col < values[0].NumCols(); col++){
            vector<ZZ_p> vals(values.size());
            for(long i = 0; i < values.size(); i++){
                vals[i] = values[i].get(row, col);
            }
            vector<ZZ_p> shifted_vals(vals.size());
            shift_values(shifted_vals, vals, a, b, p);
            for(long i = 0; i < shifted_vals.size(); i++){
                out[i].put(row, col, shifted_vals[i]);
            }
        }
    }
}

/*
 * k := out.size()-1
 * Calculates M_k(x) at 0, k, ..., k^2 recursively
 * M_k(x) = M(x+1)M(x+2)...M(x+k)
 * To calculate M_m(x) at 0, k, ..., mk, use the value M_m/2(x) at 0, k, ..., (m/2)k
 */
void multieval_prod(vector<Mat<ZZ_p>> &out, std::function<void (Mat<ZZ_p>&, ZZ_p&)> A, ZZ &p){
    ZZ_p kp;
    kp.init(p);
    kp = out.size()-1;
    multieval_prod(out, kp, A, p);
}
/*
 * m := out.size()-1
 */
void multieval_prod(vector<Mat<ZZ_p>> &out, ZZ_p &k, std::function<void (Mat<ZZ_p>&, ZZ_p&)> A, ZZ &p){
    long m = out.size()-1;
    
    if(m == 1){
        ZZ_p one;
        one.init(p);
        one = 1;
        ZZ_p kone = k+1;
        A(out[0], one);
        A(out[1], kone);
        return;
    }

    vector<Mat<ZZ_p>> lower_layer(m/2+1);
    multieval_prod(lower_layer, k, A, p);
   
    vector<Mat<ZZ_p>> ll_extend(lower_layer.size());
    ZZ_p m21k = k*(m/2+1);
    shift_values(ll_extend, lower_layer, m21k, k, p);

    vector<Mat<ZZ_p>> ll_total;
    ll_total.reserve(lower_layer.size() + ll_extend.size());
    ll_total.insert(ll_total.end(), lower_layer.begin(), lower_layer.end());
    ll_total.insert(ll_total.end(), ll_extend.begin(), ll_extend.end());

    vector<Mat<ZZ_p>> ll_shift(ll_total.size());
    ZZ_p m2;
    m2.init(p);
    m2 = m/2;
    shift_values(ll_shift, ll_total, m2, k, p);

    for(long i = 0; i < out.size(); i++){
        mul(out[i], ll_shift[i], ll_total[i]);
    }
    if(m % 2 == 1){
        for(long i = 0; i < out.size(); i++){
            ZZ_p ikm = k*i + m;
            Mat<ZZ_p> extra;
            A(extra, ikm);
            mul(out[i], extra, out[i]);
        }
    }

}

/*
 * Calculate M(1)M(2)...M(n) mod p
 */
void matrix_factorial(Mat<ZZ_p> &out, long n, std::function<void (Mat<ZZ_p>&, ZZ_p&)> A, ZZ &p){
    long rtn = sqrt(n);
    
    vector<Mat<ZZ_p>> seg_prods(rtn+1);
    multieval_prod(seg_prods, A, p);

    if(n < rtn*rtn + rtn){
        for(long i = seg_prods.size()-2; i > 0; i--){
            mul(seg_prods[i-1], seg_prods[i], seg_prods[i-1]);
        }
        for(long i = rtn*rtn+1; i <= n; i++){
            Mat<ZZ_p> extra;
            ZZ_p x;
            x.init(p);
            x = i;
            A(extra, x);
            mul(seg_prods[0], extra, seg_prods[0]);
        }
    }
    else{
        for(long i = seg_prods.size()-1; i > 0; i--){
            mul(seg_prods[i-1], seg_prods[i], seg_prods[i-1]);
        }
        for(long i = rtn*rtn+rtn+1; i <= n; i++){
            Mat<ZZ_p> extra;
            ZZ_p x;
            x.init(p);
            x = i;
            A(extra, x);
            mul(seg_prods[0], extra, seg_prods[0]);
        }
    }
    out = seg_prods[0];
}

