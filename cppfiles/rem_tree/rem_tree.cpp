#include "rem_tree.h"

using namespace std;
using namespace NTL;

rem_tree::rem_tree(ZZ startBound, ZZ endBound, ZZ (*const startVal)(ZZ), Vec<ZZ> A, Vec<ZZ> m){
	this->startBound = startBound;
	this->endBound = endBound;
	this->startVal = startVal; 
	this->A = A;
	this->m = m;
}

