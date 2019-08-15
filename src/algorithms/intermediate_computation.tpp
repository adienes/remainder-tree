#include <cassert>

#include "utils.hpp"

using std::vector;

//used primarily for the m values
template <typename T>
vector<T> subproduct_tree(const vector<T>& vals, long P) {
	long N = vals.size(); //N is number of leaves

	long left = P; //this will be the leftmost leaf under the subtree rooted at P
	long right = P; //similarly the rightmost leaf under subtree rooted at P
	while (left < N) { //stops when left is a leaf. Same as 2*left < 2*N (since have to stop one before)
		left = 2*left;
		right = 2*right + 1;
	}
	left -= N;
	right -= N;
	//now left, right store the indices in the ORIGINAL vector which correspond
	//the the span of the chosen subtree


	long M = right - left + 1; //number of leaves in subtree (should still be power2)
	vector<T> subp_tree(2*M);

	for (long i = M; i < 2*M; ++i) { // leaves on lowest layer
		subp_tree[i] = vals[left + (i - M)]; //at i = M this gives left, and i = 2*M - 1 gives right
	}

	// Calculate the rest of the product tree mTree
	for (long i = M - 1; i > 0; --i) {
		//mTree[i] = (mTree[2*i]*mTree[2*i + 1]);

		subp_tree[i] = subp_tree[2*i]*subp_tree[2*i + 1];
		//sometimes in-place multiplication might be faster
		//not sure if it will really make a difference
	}

	return subp_tree;
}

template <typename T, typename U>
vector<T> subproduct_tree_askew(const vector<T>& vals, const vector<U>& mod_tree, long P) {
	long N = vals.size(); //N is number of leaves

	long left = P; //this will be the leftmost leaf under the subtree rooted at P
	long right = P; //similarly the rightmost leaf under subtree rooted at P
	while (left < N) { //stops when left_span is a leaf
		left = 2*left;
		right = 2*right + 1;

	}
	left -= N;
	right -= N;
	//now left, right store the indices in the ORIGINAL vector which correspond
	//the the span of the chosen subtree

	long M = right - left + 1; //number of leaves in subtree (should still be power2)
	vector<T> subp_tree_askew(2*M);

	for (long i = M; i < 2*M; ++i) { // leaves on lowest layer
		subp_tree_askew[i] = vals[left + (i - M)]; //at i = M this gives left, and i = 2*M - 1 gives right
	}

	// Calculate the rest of the product tree aTree, taking mod mTree[1] = m[0]*...*m[N-1]
	// Using Andy's modified structure such that the notation is more longuitive and no elements wasted.
	long nextleftmost = (M >> 1);
	for (long i = M - 1; i > 0; --i) { //alternatively could be Atree[N]
		if (i == nextleftmost) {
			//TODO: could add something that kills the A[0] value but really might be overkill
			nextleftmost >>= 1; //traverse up the tree once to the right.
			//subp_tree_askew[i] = subp_tree_askew[0]; //this step is actually unecessary---technically the value is never used
			continue;
		} else { //parent is product of left child and superleft child
			//subp_tree_askew[i] = (subp_tree_askew[2*i]*subp_tree_askew[2*i - 1])%modulus;
			
			subp_tree_askew[i] = subp_tree_askew[2*i - 1];
			subp_tree_askew[i].mulmod(subp_tree_askew[2*i], mod_tree[1]);
			//in place is possibly marginally faster

			//subp_tree_askew[2*i].mod(mod_tree[2*i]); not necessary as it gets killed
			subp_tree_askew[2*i + 1].mod(mod_tree[2*i + 1]);

		}
		
		subp_tree_askew[2*i] = T(); //always kill the left child (this is askew)
	}

	return subp_tree_askew;
}


template <typename T>
T compute_product_node(const vector<T>& vals, long k) { //typically used to get the value for the m tree
	long N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		return vals[k-N];
	}

	else {
		return compute_product_node<T>(vals, 2*k)*compute_product_node<T>(vals, 2*k+1);
	}
}

//In case we need product with a modulus
template <typename T, typename U>
T compute_product_node(const vector<T>& vals, const U& modulus, long k) { //typically used to get the value for the m tree
	long N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		return vals[k-N]%modulus;
	}

	else {
		T C = compute_product_node<T>(vals, modulus, 2*k);
		C.mulmod(compute_product_node<T>(vals, modulus, 2*k+1), modulus);
		return C;
	}
}

template <typename T, typename U>
T compute_product_node_askew(const vector<T>& vals, const U& modulus, long k) { //typically used to get the value for the A tree
	if (modulus == 0) { //DEBUG: default value only, 0 should never be passed in!
		return T();
	}

	long N = vals.size();

	assert (k < 2*N);

	if (k >= N) { //aka a leaf node
		return vals[k-N]%modulus;
	}

	else if (is_power2(k)) {
		return vals[0]%modulus;
	}

	else {
		T C = compute_product_node_askew<T,U>(vals, modulus, 2*k-1);
		C.mulmod(compute_product_node_askew<T,U>(vals, modulus, 2*k), modulus);
		return C;
	}	
}


//For internal use only, to implement the below.
template <typename T, typename U>
T _compute_remainder_tree_value(const vector<T>& A, const U& modulus, long k, const T& V) {
	if (k == 1) { //return A[0] mod mProd
		return ((V*A[0])%modulus);
	}

	//else, it is not the root of the Ctree
	bool dir = (k%2);
	if (dir) { //right child is parent times the atree mod the mtree
		T C = _compute_remainder_tree_value<T,U>(A, modulus, (k >> 1), V);
		C.mulmod(compute_product_node_askew<T,U>(A, modulus, k), modulus);
		return C;
	}

	else { //left child is parent mod the mtree val

		//%modulus; is not necessary because parent is already reduced!
		return _compute_remainder_tree_value<T,U>(A, modulus, (k >> 1), V);
	}
}

//This is only to test "chunks". It will be replaced, although shouldn't typically be a bottleneck.
template <typename T, typename U>
T compute_remainder_tree_value(const vector<T>& A, const vector<U>& m, long k, const T& V) {
	assert (A.size() == m.size());

	U modulus = compute_product_node<U> (m, k);

	return _compute_remainder_tree_value<T,U>(A, modulus, k, V);
}


