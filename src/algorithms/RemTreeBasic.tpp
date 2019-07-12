#include <vector>
#include <assert.h> 

#include<iostream>

using std::vector;

template <typename T>
void print_tree(vector<T>& v) {
	std::cout << std::endl;
	for (auto&& i : v) {
		std::cout << i.t << std::endl;
	}
}

/*
Takes A, m, V and returns the remainders as expected.
Makes no space-time tradeoff and assumes all the memory needed is present.

STRONGLY RECOMMEND passing in types Elt<T> and Elt<U> rather than <T> and <U> 
directly. Using it in that way will most likely break if the same methods
have not been implemented as expected.
*/

template <typename T, typename U>
vector<T> remainder_tree(const vector<T>& A, const vector<U>& m, const T& V) {
	int N = A.size();
	assert (A.size() == m.size());

	//the product trees will always be twice as big as the given array
	vector<T> ATree(2*N); //this initializes with elt(0)
	vector<U> mTree(2*N);
	vector<T> CTree(2*N);

	// Note that N is the index of the leftmost leaf
	// Initialize the leaves in ATree and mTree
	for (int i = N; i < 2*N; ++i) { // leaves on lowest layer
		ATree[i] = A[i - N];
		mTree[i] = m[i - N];
	}

	// Calculate the rest of the product tree mTree
	for (int i = N - 1; i > 0; --i) {
		//mTree[i] = (mTree[2*i]*mTree[2*i + 1]);

		mTree[i] = mTree[2*i];
		mTree[i].mul(mTree[2*i + 1]);
		//sometimes in-place multiplication might be faster
		//not sure if it will really make a difference
	}

	// Calculate the rest of the product tree aTree, taking mod mTree[1] = m[0]*...*m[N-1]
	// Using Andy's modified structure such that the notation is more intuitive and no elements wasted.
	int nextleftmost = (N >> 1);
	for (int i = N - 1; i > 0; --i) { //alternatively could be Atree[N]
		if (i == nextleftmost) { 
			ATree[i] = A[0];
			nextleftmost >>= 1; //traverse up the tree once to the right.
		}

		else { //parent is product of left child and superleft child
			//ATree[i] = (ATree[2*i]*ATree[2*i - 1])%mTree[1];
			
			ATree[i] = ATree[2*i];
			ATree[i].mulmod(ATree[2*i - 1], mTree[1]);
			//in place is possibly marginally faster
		}

		ATree[2*i] = T(); //always kill the left child
	}

	CTree[1] = V*A[0]; //recall the tree is 1-indexed
	for (int i = 1; i < N; ++i) { //only go up to second-lowest layer
		CTree[2*i] = CTree[i]%mTree[2*i];
		//CTree[2*i + 1] = (CTree[i]*ATree[2*i + 1])%mTree[2*i + 1];
		
		CTree[2*i + 1] = CTree[i];
		CTree[2*i + 1].mulmod(ATree[2*i + 1], mTree[2*i + 1]);

		CTree[i] = T(); //always kill the parent
	}

	//we can now read the remainders off the bottom of the tree
	typename vector<T>::const_iterator first = CTree.begin() + N;
	typename vector<T>::const_iterator last = CTree.end();
	vector<T> C(first, last);
	
	return C;
}

//this only exists because I'm lazy with notation lol
template <typename T>
vector<T> remainder_tree(const vector<T>& A, const vector<T>& m, const T& V) {
	return remainder_tree<T, T>(A, m, V);
}