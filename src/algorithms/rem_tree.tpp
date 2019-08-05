#include <vector>
#include <cassert>

#include "intermediate_computation.hpp"

using std::vector;




/*
Takes A, m, V and returns the remainders as expected.
Makes no space-time tradeoff and assumes all the memory needed is present.

STRONGLY RECOMMEND passing in types Elt<T> and Elt<U> rather than <T> and <U> 
directly. Using it in that way will most likely break if the same methods
have not been implemented as expected.
*/

template <typename T, typename U>
vector<T> remainder_tree_basic(const vector<T>& A, const vector<U>& m, const T& V) {
	assert (A.size() == m.size());
	long long N = A.size();

	vector<U> mTree = subproduct_tree<U> (m, 1); //Default value of P corresponds to root node, but passing in just to be safe
	vector<T> ATree = subproduct_tree_askew<T,U> (A, mTree, 1);

	vector<T> CTree(2*N);

	CTree[1] = V*A[0]; //recall the tree is 1-indexed
	for (long long i = 1; i < N; ++i) { //only go up to second-lowest layer
		CTree[2*i] = CTree[i]%mTree[2*i];
		//CTree[2*i + 1] = (CTree[i]*ATree[2*i + 1])%mTree[2*i + 1];
		
		CTree[2*i + 1] = CTree[i];
		CTree[2*i + 1].mulmod(ATree[2*i + 1], mTree[2*i + 1]);

		//Always kill the parents when no long longer needed
		CTree[i] = T();
		ATree[i] = T();
		mTree[i] = U();

	}

	//we can now read the remainders off the bottom of the tree
	typename vector<T>::const_iterator first = CTree.begin() + N;
	typename vector<T>::const_iterator last = CTree.end();
	vector<T> C(first, last);
	
	return C;
}



//Note that the vectors are passed by value since we may modify them
//Because of this, try to use powers of 2 for best performance.
template <typename T, typename U>
vector<T> remainder_tree_padded(vector<T> A, vector<U> m, const T& V) {
	long long N = A.size();
	assert (A.size() == m.size());

	long long height = ceil(log2(N-1));
	long long N_pad = (1<<height);

	A.resize(N_pad, 0);
	m.resize(N_pad, 1);

	vector<T> C_padded = remainder_tree(A, m, V);

	typename vector<T>::const_iterator first = C_padded.begin();
	typename vector<T>::const_iterator last = C_padded.begin()+N;
	vector<T> C(first, last);

	return C;
}