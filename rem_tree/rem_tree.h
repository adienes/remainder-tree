#ifndef REM_TREE_H
#define REM_TREE_H

#include <NTL/ZZ.h>
#include <NTL/vector.h>


class rem_tree{
private:
	ZZ startBound;				// lower bound in interval
	ZZ endBound; 				// upper bound in interval
	ZZ (*const startVal)(ZZ);	// pointer to function that calculates start value mod product of moduli (IS THIS RIGHT???)
	Vec<ZZ> A;						// Array of values in leaves
	Vec<ZZ> m; 						// Array of moduli in leaves

public:
	rem_tree(ZZ startBound, ZZ endBound, ZZ (*const startVal)(ZZ), Vec<ZZ> A, Vec<ZZ> m);
	~rem_tree();

};
#endif