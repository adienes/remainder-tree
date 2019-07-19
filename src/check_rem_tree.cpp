#include <iostream>
#include <vector>
#include <cmath>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>

using namespace std;
using namespace NTL;

void check_p(ZZ_p &ans, long p, vector<ZZ_p> coeffs);
void findF(ZZ_pX &F, ZZ_pX &f, long rtp);
void poly_eval(ZZ_pX &h, ZZ_pX &f, ZZ_pX &g);
void evalF(vector<ZZ_p> &FVals, ZZ_pX &F, long rtp, long prtp);

long p = 563;
ZZ mod = ZZ(p)*ZZ(p);

int main(){
    vector<ZZ_p> coeffs(2);
    coeffs[0].init(mod);
    coeffs[1].init(mod);
    coeffs[0] = 0;
    coeffs[1] = 1;
    
    ZZ_p ans;
    ans.init(mod);

    check_p(ans, p, coeffs);

    cout << "Answer: " << ans << endl;
}
// TODO: init all ZZ_p variables
void check_p(ZZ_p &ans, long p, vector<ZZ_p> coeffs){
    ZZ_pX f;
    for(int i = 0; i < coeffs.size(); i++){
        SetCoeff(f, i, coeffs[i]);
    }

    long rtp = sqrt(p-1); // rtp = floor(sqrt(p-1))
    ZZ_pX F;
    findF(F, f, rtp);

    long prtp = (p-1)/rtp;
    vector<ZZ_p> FVals(prtp);
    evalF(FVals, F, rtp, prtp); // prtp := floor((p-1)/floor(sqrt(p-1)))

    ZZ_p out(1);
    out.init(mod);
    for(long i = rtp*prtp+1; i < p; i++){
        mul(out, out, eval(f, ZZ_p(i)));
    }
    for(long i = 0; i < FVals.size(); i++){
        mul(out, out, FVals[i]);
    }

    ans = out;
}

// return F = f(X+1)f(X+2)...f(X+rtp) mod p
void findF(ZZ_pX &F, ZZ_pX &f, long rtp){
    vector<ZZ_pX> FTree(2*rtp); // rtp leaves -> 2*rtp nodes

    long leftmost = 1 << ((int)ceil(log2(rtp))); // bottom leftmost node in tree

    // Initialize the leaves in FTree: [X+1, X+2, ..., X+rtp]
    for (long i = leftmost; i < 2 * rtp; i++) { // leaves on lowest layer
        ZZ_pX leaf;
        SetCoeff(leaf, 0, i - leftmost + 1);
        SetCoeff(leaf, 1, 1);
        poly_eval(leaf, f, leaf); // leaf = f(leaf())
        FTree[i] = leaf;
    }
    for (long i = rtp; i < leftmost; i++) { // leaves on second lowest layer
        ZZ_pX leaf;
        SetCoeff(leaf, 0, i + rtp - leftmost + 1);
        SetCoeff(leaf, 1, 1);
        poly_eval(leaf, f, leaf); // leaf = f(leaf())
        FTree[i] = leaf;
    }

    // Calculate the rest of the product tree FTree
    for (long i = rtp - 1; i > 0; i--) {
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

// evaluate F at 0, rtp, ..., (prtp-1)*rtp
void evalF(vector<ZZ_p> &FVals, ZZ_pX &F, long rtp, long prtp){
    vector<ZZ_pX> FValTree(2*prtp);

    long leftmost = 1 << ((int)ceil(log2(prtp))); // bottom leftmost node in tree

    // Initialize the leaves in FValTree: [X, X-rtp, ..., X-rtp*(prtp-1)]
    for (long i = leftmost; i < 2 * prtp; i++) { // leaves on lowest layer
        ZZ_pX leaf;
        SetCoeff(leaf, 0, -rtp*(i - leftmost));
        SetCoeff(leaf, 1, 1);
        FValTree[i] = leaf;
    }
    for (long i = prtp; i < leftmost; i++) { // leaves on second lowest layer
        ZZ_pX leaf;
        SetCoeff(leaf, 0, -rtp*(i + prtp - leftmost));
        SetCoeff(leaf, 1, 1);
        FValTree[i] = leaf;
    }

    // Calculate the rest of the product tree FValTree
    for (long i = prtp - 1; i > 0; i--) {
        FValTree[i] = FValTree[2*i] * FValTree[2*i+1]; // parent is product of leaves
    }
    
    // Reduce F mod polynomials in FValTree
    rem(FValTree[1], F, FValTree[1]);
    for (long i = 1; i < prtp; i++) {
        rem(FValTree[2*i], FValTree[i], FValTree[2*i]);
        rem(FValTree[2*i+1], FValTree[i], FValTree[2*i+1]);
    }
    
    for (long i = leftmost; i < 2 * prtp; i++) { // leaves on lowest layer
        FVals[i - leftmost].init(mod);
        FVals[i - leftmost] = ConstTerm(FValTree[i]);
    }
    for (long i = prtp; i < leftmost; i++) { // leaves on second lowest layer
        FVals[i + prtp - leftmost].init(mod);
        FVals[i + prtp - leftmost] = ConstTerm(FValTree[i]);
    }

}