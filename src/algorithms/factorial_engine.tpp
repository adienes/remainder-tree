#include "utils.hpp"

using std::vector;

using NTL::ZZ_pX;
using NTL::ZZ_p;
using NTL::ZZ;
using NTL::Mat;

/*
 * Finds the product A[0]A[1]...A[n-1] using:
 * naive_factorial: naive product tree calculation
 * poly_factorial: calculation that works when A is a polynomial
 * matrix_factorial: calculation that works when A is a matrix with linear entries
 */

template<typename T, typename U>
T naive_factorial(long n, U& m, std::function<vector<T> (long, long)>& get_A){
    // use get node method in utils.h
    return compute_product_node<T, U>(get_A(0, n), m, 1);
}

// Helper methods for poly_factorial
void check_p(ZZ_p &ans, long p, vector<ZZ_p> coeffs);
void findF(ZZ_pX &F, ZZ_pX &f, long rtp);
void poly_eval(ZZ_pX &h, ZZ_pX &f, ZZ_pX &g);
void evalF(vector<ZZ_p> &FVals, ZZ_pX &F, long rtp, long prtp);

ZZ_p poly_factorial(long n, ZZ& m, ZZ_pX& poly){
    ZZ_p output;
    check_p(output, n, m, poly);
    return output;
}

// Helper methods for matrix_factorial
void A(Mat<ZZ_p> &out, ZZ_p &x, Mat<ZZ_pX>& in);
void invert_all(vector<ZZ_p> &out, vector<ZZ_p> &a);
void find_delta(vector<ZZ_p> &out, ZZ &m);
void find_delta(vector<ZZ_p> &out, ZZ &m, ZZ_p &a);
void shift_values(vector<ZZ_p> &out, vector<ZZ_p> &values, ZZ_p &a, ZZ_p &b, ZZ &m);
void shift_values(vector<Mat<ZZ_p>> &out, vector<Mat<ZZ_p>> &values, ZZ_p &a, ZZ_p &b, ZZ &m);
void multieval_prod(vector<Mat<ZZ_p>> &out, Mat<ZZ_pX>& matrix, ZZ &m);
void multieval_prod(vector<Mat<ZZ_p>> &out, ZZ_p &k, Mat<ZZ_pX>& matrix, ZZ &m);
void matrix_factorial(Mat<ZZ_p> &out, long n, Mat<ZZ_pX>& matrix, ZZ &m);

Mat<ZZ_p> matrix_factorial(long n, ZZ& m, Mat<ZZ_pX>& matrix){
     Mat<ZZ_p> output;
     matrix_factorial(output, n, matrix, m);
     return output;
}

void A(Mat<ZZ_p>& out, ZZ_p& x, Mat<ZZ_pX>& in){
    out.SetDims(in.NumRows(), in.NumCols());
    for(long row = 0; row < in.NumRows(); row++){
        for(long col = 0; col < in.NumCols(); col++){
            out.put(eval(in.get(row, col), x), row, col);
        }
    }
}

/*
 * BEGIN: poly_factorial algorithm
 */
void check_p(ZZ_p &ans, long n, ZZ& m, ZZ_pX& f){
    
    long rtn = sqrt(n-1); // rtn = floor(sqrt(n-1))
    ZZ_pX F;
    findF(F, f, rtn);

    long nrtn = (n-1)/rtn;
    vector<ZZ_p> FVals(nrtn);
    evalF(FVals, F, m, rtn, nrtn); // nrtn := floor((n-1)/floor(sqrt(n-1)))

    ZZ_p out(1);
    out.init(m);
    for(long i = rtn*nrtn+1; i < n; i++){
        mul(out, out, eval(f, ZZ_p(i)));
    }
    for(long i = 0, FVals_size = FVals.size(); i < FVals_size; i++){
        mul(out, out, FVals[i]);
    }

    ans = out;
}

// return F = f(X+1)f(X+2)...f(X+rtn) mod m
void findF(ZZ_pX &F, ZZ_pX &f, long rtn){
    vector<ZZ_pX> FTree(2*rtn); // rtn leaves -> 2*rtn nodes

    long leftmost = 1 << ((int)ceil(log2(rtn))); // bottom leftmost node in tree

    // Initialize the leaves in FTree: [X+1, X+2, ..., X+rtn]
    for (long i = leftmost; i < 2 * rtn; i++) { // leaves on lowest layer
        ZZ_pX leaf;
        SetCoeff(leaf, 0, i - leftmost + 1);
        SetCoeff(leaf, 1, 1);
        poly_eval(leaf, f, leaf); // leaf = f(leaf())
        FTree[i] = leaf;
    }
    for (long i = rtn; i < leftmost; i++) { // leaves on second lowest layer
        ZZ_pX leaf;
        SetCoeff(leaf, 0, i + rtn - leftmost + 1);
        SetCoeff(leaf, 1, 1);
        poly_eval(leaf, f, leaf); // leaf = f(leaf())
        FTree[i] = leaf;
    }

    // Calculate the rest of the product tree FTree
    for (long i = rtn - 1; i > 0; i--) {
        FTree[i] = FTree[2*i] * FTree[2*i+1]; // parent is product of leaves
        // TODO doesn't work:
        // delete FTree[2*i];
        // delete FTree[2*i+1];
    }

    F = FTree[1];
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

// evaluate F at 0, rtn, ..., (nrtn-1)*rtn
void evalF(vector<ZZ_p> &FVals, ZZ_pX &F, ZZ& m, long rtn, long nrtn){
    vector<ZZ_pX> FValTree(2*nrtn);

    long leftmost = 1 << ((int)ceil(log2(nrtn))); // bottom leftmost node in tree

    // Initialize the leaves in FValTree: [X, X-rtn, ..., X-rtn*(nrtn-1)]
    for (long i = leftmost; i < 2 * nrtn; i++) { // leaves on lowest layer
        ZZ_pX leaf;
        SetCoeff(leaf, 0, -rtn*(i - leftmost));
        SetCoeff(leaf, 1, 1);
        FValTree[i] = leaf;
    }
    for (long i = nrtn; i < leftmost; i++) { // leaves on second lowest layer
        ZZ_pX leaf;
        SetCoeff(leaf, 0, -rtn*(i + nrtn - leftmost));
        SetCoeff(leaf, 1, 1);
        FValTree[i] = leaf;
    }

    // Calculate the rest of the product tree FValTree
    for (long i = nrtn - 1; i > 0; i--) {
        FValTree[i] = FValTree[2*i] * FValTree[2*i+1]; // parent is product of leaves
    }

    // Reduce F mod polynomials in FValTree
    rem(FValTree[1], F, FValTree[1]);
    for (long i = 1; i < nrtn; i++) {
        rem(FValTree[2*i], FValTree[i], FValTree[2*i]);
        rem(FValTree[2*i+1], FValTree[i], FValTree[2*i+1]);
    }

    for (long i = leftmost; i < 2 * nrtn; i++) { // leaves on lowest layer
        FVals[i - leftmost].init(m);
        FVals[i - leftmost] = ConstTerm(FValTree[i]);
    }
    for (long i = nrtn; i < leftmost; i++) { // leaves on second lowest layer
        FVals[i + nrtn - leftmost].init(m);
        FVals[i + nrtn - leftmost] = ConstTerm(FValTree[i]);
    }

}

/*
 * BEGIN: matrix_factorial algorithm
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
void find_delta(vector<ZZ_p> &out, ZZ &m){
    long d = out.size() - 1;
    vector<ZZ_p> ints(d);
    for(long i = 0, ints_size = ints.size(); i < ints_size; i++){
        ints[i].init(m);
        ints[i] = i+1;
    }
    vector<ZZ_p> inv_ints(d);
    invert_all(inv_ints, ints);
    
    vector<ZZ_p> deltas(d+1);
    deltas[0] = inv_ints[0]; // = 1
    for(long i = 1, inv_ints_size = inv_ints.size(); i < inv_ints_size; i++){
        mul(deltas[0], deltas[0], inv_ints[i]);
    } // deltas[0] = 1/d!
    
    if(d%2 == 1){
        deltas[0] = -deltas[0]; // deltas[0] = 1/((-1)^d*d!)
    }

    for(long i = 1, deltas_size = deltas.size(); i < deltas_size; i++){
        mul(deltas[i], deltas[i-1], i-1-d);
        mul(deltas[i], deltas[i], inv_ints[i-1]);
    }

    out = deltas;
}

/*
 * d = out.size()-1;
 * Returns Delta(a, i, d) = prod(a+i-j) where the product goes from j=0 to d.
 */
void find_delta(vector<ZZ_p> &out, ZZ_p &a, ZZ &m){
    long d = out.size() - 1;

    vector<ZZ_p> ints(d);
    for(long i = 0, ints_size = ints.size(); i < ints_size; i++){
        ints[i].init(m);
        ints[i] = a-d+i;
    }
    vector<ZZ_p> inv_ints(d);
    invert_all(inv_ints, ints);
    
    vector<ZZ_p> Deltas(d+1);
    Deltas[0] = a;
    for(long i = 0, inv_ints_size = inv_ints.size(); i < inv_ints_size; i++){
        mul(Deltas[0], Deltas[0], ints[i]);
    } // Deltas[0] = (a-d)...(a-1)(a)
    for(long i = 1, Deltas_size = Deltas.size(); i < Deltas_size; i++){
        mul(Deltas[i], Deltas[i-1], a+i);
        mul(Deltas[i], Deltas[i], inv_ints[i-1]);
    }

    out = Deltas;
}

/*
 * Takes in the values of polynomials F(r), F(r+b), ..., F(r+db) and outputs F(r+a), F(r+b+a), ... F(r+db+a)
 * Requires deg(F) <= d to work mathematically, since it will interpolate based on the given evaluations of F.
 */
void shift_values(vector<ZZ_p> &out, vector<ZZ_p> &values, ZZ_p &a, ZZ_p &b, ZZ &m){
    assert(out.size() == values.size());

    long d = values.size() - 1;
    vector<ZZ_p> P(d+1);
    find_delta(P, m);
    
    ZZ_pX Px;
    for(long i = 0, values_size = values.size(); i < values_size; i++){
        mul(P[i], P[i], values[i]);
        SetCoeff(Px, i, P[i]);
    }

    ZZ_p shift;
    shift.init(m);
    inv(shift, b);
    mul(shift, shift, a); // shift = a/b

    vector<ZZ_p> S(2*d+1);
    for(long i = 0, S_size = S.size(); i < S_size; i++){
        S[i] = shift + i - d;
    }
    invert_all(S, S);

    ZZ_pX Sx;
    for(long i = 0, S_size = S.size(); i < S_size; i++){
        SetCoeff(Sx, i, S[i]);
    }   

    ZZ_pX PS;
    mul(PS, Px, Sx);

    find_delta(out, shift, m);
    for(long i = 0, out_size = out.size(); i < out_size; i++){
        mul(out[i], out[i], coeff(PS, i+d));
    }
}
                                                                                  
/*
 * Takes in the values of matrices of polynomials M(r), M(r+b), ..., M(r+db) and outputs M(r+a), M(r+b+a), ... M(r+db+a)
 * Requires the maximal degree of any element of M(x) to be <= d to successfully interpolate
 */
void shift_values(vector<Mat<ZZ_p>> &out, vector<Mat<ZZ_p>> &values, ZZ_p &a, ZZ_p &b, ZZ &m){
    assert(out.size() == values.size());
    assert(values[0].NumRows() == values[0].NumCols());
    for(long i = 0, out_size = out.size(); i < out_size; i++){
        assert(values[i].NumRows() == values[0].NumRows());
        assert(values[i].NumCols() == values[0].NumCols());
        out[i].SetDims(values[0].NumRows(), values[0].NumCols());
    } 

    for(long row = 0; row < values[0].NumRows(); row++){
        for(long col = 0; col < values[0].NumCols(); col++){
            vector<ZZ_p> vals(values.size());
            for(long i = 0, values_size = values.size(); i < values_size; i++){
                vals[i] = values[i].get(row, col);
            }
            vector<ZZ_p> shifted_vals(vals.size());
            shift_values(shifted_vals, vals, a, b, m);
            for(long i = 0, shifted_vals_size = shifted_vals.size(); i < shifted_vals_size; i++){
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
void multieval_prod(vector<Mat<ZZ_p>> &out, Mat<ZZ_pX>& matrix, ZZ &m){
    ZZ_p kp;
    kp.init(m);
    kp = out.size()-1;
    multieval_prod(out, kp, matrix, kp);
}
/*
 * m := out.size()-1
 */
void multieval_prod(vector<Mat<ZZ_p>> &out, ZZ_p &k, Mat<ZZ_pX>& matrix, ZZ &m){
    long n = out.size()-1;
    
    if(n == 1){
        ZZ_p one;
        one.init(m);
        one = 1;
        ZZ_p kone = k+1;
        A(out[0], one, matrix);
        A(out[1], kone, matrix);
        return;
    }

    vector<Mat<ZZ_p>> lower_layer(n/2+1);
    multieval_prod(lower_layer, k, matrix, m);
   
    vector<Mat<ZZ_p>> ll_extend(lower_layer.size());
    ZZ_p n21k = k*(n/2+1);
    shift_values(ll_extend, lower_layer, n21k, k, m);

    vector<Mat<ZZ_p>> ll_total;
    ll_total.reserve(lower_layer.size() + ll_extend.size());
    ll_total.insert(ll_total.end(), lower_layer.begin(), lower_layer.end());
    ll_total.insert(ll_total.end(), ll_extend.begin(), ll_extend.end());

    vector<Mat<ZZ_p>> ll_shift(ll_total.size());
    ZZ_p n2;
    n2.init(m);
    n2 = n/2;
    shift_values(ll_shift, ll_total, n2, k, m);

    for(long i = 0, out_size = out.size(); i < out_size; i++){
        mul(out[i], ll_shift[i], ll_total[i]);
    }
    if(n % 2 == 1){
        for(long i = 0, out_size = out.size(); i < out_size; i++){
            ZZ_p ikn = k*i + n;
            Mat<ZZ_p> extra;
            A(extra, ikn, matrix);
            mul(out[i], extra, out[i]);
        }
    }

}

/*
 * Calculate M(1)M(2)...M(n) mod p
 */
void matrix_factorial(Mat<ZZ_p> &out, long n, Mat<ZZ_pX>& matrix, ZZ &m){
    long rtn = sqrt(n);
    
    vector<Mat<ZZ_p>> seg_prods(rtn+1);
    multieval_prod(seg_prods, matrix, m);

    if(n < rtn*rtn + rtn){
        for(long i = seg_prods.size()-2; i > 0; i--){
            mul(seg_prods[i-1], seg_prods[i], seg_prods[i-1]);
        }
        for(long i = rtn*rtn+1; i <= n; i++){
            Mat<ZZ_p> extra;
            ZZ_p x;
            x.init(m);
            x = i;
            A(extra, x, matrix);
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
            x.init(m);
            x = i;
            A(extra, x, matrix);
            mul(seg_prods[0], extra, seg_prods[0]);
        }
    }
    out = seg_prods[0];
}

