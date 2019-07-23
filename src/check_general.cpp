#include <iostream>
#include <vector>
#include <cassert>
#include <NTL/matrix.h>
#include <NTL/ZZ_pX.h> // includes ZZ_p.h and ZZ.h as well

using namespace std;
using namespace NTL;

void invert_all(vector<ZZ_p> &out, vector<ZZ_p> &a);
void find_delta(vector<ZZ_p> &out, ZZ &p);
void find_delta(vector<ZZ_p> &out, ZZ &p, ZZ_p &a);
template <typename T>
void print(vector<T> vec);

/* 
 * Steps:
 * [DONE] Create function that take inputs a0, a1, ..., an-1 and outputs 1/a0, ..., 1/an-1
 * [DONE] Create function that takes input d and outputs 1/prod(0-j), 1/prod(1-j), ..., 1/prod(d-j)
 * [DONE] Create function that takes input a, d and outputs prod(a+0-j), prod(a+1-j), ..., prod(a+d-j)
 * [DONE] Create function that take inputs M(a), M(a+b), ..., M(a+kb) and outputs M(a+i), M(a+b+i), ..., M(a+kb+i)
 * Create function to calculate M_sqrt(p-1)(0), ..., M_sqrt(p-1)(p-1)
 * Create main function to calculate M(1)...M(p-1) = M_sqrt(p-1)(0)...M_sqrt(p-1)(p-1-sqrt(p-1))
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
// calculate h = f(g(x))
void poly_eval(ZZ_pX &h, ZZ_pX &f, ZZ_pX &g){
    ZZ_pX out(LeadCoeff(f));
    for(int i = deg(f)-1; i >= 0; i--){
        mul(out, out, g);
        add(out, out, coeff(f, i));
    }
    h = out;
}

int main(){
    ZZ p(7);
    ZZ_p a;
    a.init(p);
    a = 1;
    ZZ_p b;
    b.init(p);
    b = 2;

    vector<ZZ_p> vals(3);
    vector<ZZ_p> out(3);
    for(int i = 0; i < 3; i++){
        vals[i].init(p);
    }
    vals[0] = 2;
    vals[1] = 2;
    vals[2] = 3;

    shift_values(out, vals, a, b, p);

    for(int i = 0; i < 3; i++){
        cout << out[i] << endl;
    }
}

template<typename T>
void print(vector<T> vec){
    for(long i = 0; i < vec.size(); i++){
        cout << vec[i];
        if(i < vec.size()-1){
            cout << ", ";
        }
    }
    cout << endl;
}

