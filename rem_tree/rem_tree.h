#ifndef REM_TREE_H
#define REM_TREE_H


class rem_tree{
private:
	int startBound;		// lower bound in interval
	int endBound; 		// upper bound in interval
	int startValue;		// starting value inputted into moduli
	int A;				// Array of values in leaves
	int m; 				// Array of moduli in leaves

public:
	rem_tree();
	~rem_tree();

};
#endif