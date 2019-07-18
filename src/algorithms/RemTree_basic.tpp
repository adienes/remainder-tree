#include <vector>
#include <assert.h> 

using std::vector;

//used primarily for the m values
template <typename T>
vector<T> subproduct_tree(const vector<T>& vals, int P) {
	int N = vals.size(); //N is number of leaves

	int left = P; //this will be the leftmost leaf under the subtree rooted at P
	int right = P; //similarly the rightmost leaf under subtree rooted at P
	while (left < 2*N) { //stops when left_span is a leaf
		left = 2*left;
		right = 2*right + 1;

	}
	left -= N;
	right -= N;
	//now left, right store the indices in the ORIGINAL vector which correspond
	//the the span of the chosen subtree

	int M = right - left + 1; //number of leaves in subtree (should still be power2)
	vector<T> subp_tree(2*M);

	for (int i = M; i < 2*M; ++i) { // leaves on lowest layer
		subp_tree[i] = vals[left + (i - M)]; //at i = M this gives left, and i = 2*M - 1 gives right
	}

	// Calculate the rest of the product tree mTree
	for (int i = M - 1; i > 0; --i) {
		//mTree[i] = (mTree[2*i]*mTree[2*i + 1]);

		subp_tree[i] = subp_tree[2*i]*subp_tree[2*i + 1];
		//sometimes in-place multiplication might be faster
		//not sure if it will really make a difference
	}

	return subp_tree;
}

template <typename T, typename U>
vector<T> subproduct_tree_askew(const vector<T>& vals, const U& modulus, int P) {
	int N = vals.size(); //N is number of leaves

	int left = P; //this will be the leftmost leaf under the subtree rooted at P
	int right = P; //similarly the rightmost leaf under subtree rooted at P
	while (left < 2*N) { //stops when left_span is a leaf
		left = 2*left;
		right = 2*right + 1;

	}
	left -= N;
	right -= N;
	//now left, right store the indices in the ORIGINAL vector which correspond
	//the the span of the chosen subtree

	int M = right - left + 1; //number of leaves in subtree (should still be power2)
	vector<T> subp_tree_askew(2*M);

	// Calculate the rest of the product tree aTree, taking mod mTree[1] = m[0]*...*m[N-1]
	// Using Andy's modified structure such that the notation is more intuitive and no elements wasted.
	int nextleftmost = (N >> 1);
	for (int i = N - 1; i > 0; --i) { //alternatively could be Atree[N]
		if (i == nextleftmost) { 
			nextleftmost >>= 1; //traverse up the tree once to the right.
			//subp_tree_askew[i] = subp_tree_askew[0]; //this step is actually unecessary---technically the value is never used
			continue;
		}

		else { //parent is product of left child and superleft child
			//subp_tree_askew[i] = (subp_tree_askew[2*i]*subp_tree_askew[2*i - 1])%modulus;
			
			subp_tree_askew[i] = subp_tree_askew[2*i];
			subp_tree_askew[i].mulmod(subp_tree_askew[2*i - 1], modulus);
			//in place is possibly marginally faster
		}

		subp_tree_askew[2*i] = T(); //always kill the left child
	}

	return subp_tree_askew;
}
template <typename T>
vector<T> subproduct_tree_askew(const vector<T>& vals, const T& modulus, int P) {
	return subproduct_tree_askew<T, T>(vals, modulus, P);
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
	assert (A.size() == m.size());
	int N = A.size();

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
			nextleftmost >>= 1; //traverse up the tree once to the right.
			//ATree[i] = A[0]; //this step is actually unecessary---technically the value is never used
			continue;
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



//Note that the vectors are passed by value since we may modify them
//Because of this, try to use remainder_tree_power2 for best performance.
template <typename T, typename U>
vector<T> remainder_tree_padded(vector<T> A, vector<U> m, const T& V) {
	int N = A.size();
	assert (A.size() == m.size());

	int height = ceil(log2(N-1));
	int N_pad = (1<<height);

	A.resize(N_pad, 0);
	m.resize(N_pad, 1);

	vector<T> C_padded = remainder_tree(A, m, V);

	typename vector<T>::const_iterator first = C_padded.begin();
	typename vector<T>::const_iterator last = C_padded.begin()+N;
	vector<T> C(first, last);

	return C;
}


template <typename T>
vector<T> remainder_tree_padded(const vector<T>& A, const vector<T>& m, const T& V) {
	return remainder_tree_padded<T, T>(A, m, V);
}