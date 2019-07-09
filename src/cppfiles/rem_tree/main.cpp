#include <rem_tree/rem_tree.h>

#include <iostream>
#include <chrono>
#include <NTL/ZZ.h>
#include <NTL/vector.h>

using namespace std;
using namespace NTL;

ZZ startFunc(ZZ modProd);

int main(){
	int startBound = 10;
	int endBound = 20;

	Vec<ZZ> A;
	A.setLength(endBound);
	Vec<ZZ> m;
	m.setLength(startBound);
	for(int i = 0; i < endBound; i++){
		A[i] = i;
		m[i] = (ProbPrime(ZZ(i))) ? i : 1;
	}

	ZZ (*const startVal)(ZZ) = startFunc;

	Vec<ZZ> newA;
	newA.setLength(endBound - startBound);
	Vec<ZZ> newm;
	newm.setLength(endBound - startBound);
	for(int i = 0; i < endBound - startBound; i++){
		newA[i] = A[i + startBound];
		newm[i] = m[i + startBound];
	}

	// create object
	rem_tree T(10, 20, startVal, newA, newm);
}

ZZ startFunc(ZZ modProd){
	// have to fill in
}