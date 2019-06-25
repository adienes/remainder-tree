#include <iostream>
#include <cmath>
#include <chrono>
#include <cassert>
#include <NTL/ZZ.h>
#include <NTL/vector.h>

using namespace std;
using namespace std::chrono;
using namespace NTL;

void remainder_tree(Vec<ZZ> &C, Vec<ZZ> &A, Vec<ZZ> &m);
void remainder_tree_v1(Vec<ZZ> &C, Vec<ZZ> &A, Vec<ZZ> &m, const int k);
ZZ getNode(int index, Vec<ZZ> &base, ZZ mod);
void remainder_tree_v2(Vec<ZZ> &C, Vec<ZZ> &A, Vec<ZZ> &m);
void print_tree(Vec<ZZ> tree);
//void complexity_graph(int N, int d);

int main(){
	int bound = 1337;

	Vec<ZZ> A;
	A.SetLength(bound);
	Vec<ZZ> m;
	m.SetLength(bound);

	for(int i = 0; i < bound; i++){
		A[i] = i+1;
		m[i] = ProbPrime(ZZ(i+1)) ? i+1 : 1;
	}

	for(int i = 0; i < A.length(); i++){
		cout << A[i] << " ";
	}
	cout << endl;

	for(int i = 0; i < m.length(); i++){
		cout << m[i] << " ";
	}
	cout << endl;
	
	Vec<ZZ> C;
	C.SetLength(bound);

	remainder_tree_v1(C, A, m, 4);
	

	for(int i = 0; i < C.length(); i++){
		cout << C[i] << " ";
	}
	cout << endl;
}

/*
 * Original Remainder Tree implementation
 */
void remainder_tree(Vec<ZZ> &C, Vec<ZZ> &A, Vec<ZZ> &m, ZZ root_value = ZZ(1), int start = 0, int end = -1) {
	// set default value for end
	if (end == -1) end = C.length();

	// Assert that interval [start, end] exists in C, A and m
	assert(end <= C.length());
	assert(end <= A.length());
	assert(end <= m.length());

	// Set N = length of interval
	int N = end - start;

	// Change nothing if N = 0
	if (N == 0) {
		return;
	}

	// Index of leaf at the bottom left
	int leftmost = 1 << ((int)ceil(log2(N)));

	// Declare trees (always of length 2N for any N)
	Vec<ZZ> ATree;
	ATree.SetLength(2 * N);
	Vec<ZZ> mTree;
	mTree.SetLength(2 * N);
	Vec<ZZ> CTree;
	CTree.SetLength(2 * N);

	/* 
	 * For example when N=11 the leaves are in this order:
	 *     / \       /\   /\    /\
	 *    /   \     /  7 8  9 10  11
	 *   /\   /\   /\  
	 *  1  2 3  4 5  6
	 *
	 */

	// Initialize the leaves in ATree and mTree
	for (int i = leftmost; i < 2 * N; i++) { // leaves on lowest layer
		ATree[i] = A[i - leftmost + start];
		mTree[i] = m[i - leftmost + start];
	}
	for (int i = N; i < leftmost; i++) { // leaves on second lowest layer
		ATree[i] = A[i + N - leftmost + start];
		mTree[i] = m[i + N - leftmost + start];
	}

	// Calculate the rest of the product tree mTree
	for (int i = N - 1; i > 0; i--) {
		mTree[i] = mTree[2 * i] * mTree[2 * i + 1]; // parent is product of leaves
	}

	// Calculate the rest of the product tree aTree, taking mod mTree[1] = m[0]*...*m[N-1]
	for(int i = N - 1; i > 0; i--) {
		if ((i & (i+1)) != 0) { // Don't do calculation if on a node in right-most branch
			ATree[i] = (ATree[2 * i] * ATree[2 * i + 1]) % mTree[1]; // parent is product of leaves mod mTree[1]
		}
	}

	// Calculate accumulating remainder tree
	CTree[1] = root_value % mTree[1];
	for (int i = 1; i < N; i++) {
		CTree[2 * i] = CTree[i] % mTree[2 * i]; // Left branch
		CTree[2 * i + 1] = (CTree[i] * ATree[2 * i]) % mTree[2 * i + 1]; // Right branch
	}

	//DEBUG// print_tree(ATree);
	//DEBUG// print_tree(mTree);
	//DEBUG// print_tree(CTree);
	
	for (int i = leftmost; i < 2 * N; i++) {
		C[i - leftmost + start] = CTree[i];
	}
	for (int i = N; i < leftmost; i++) {
		C[i + N - leftmost + start] = CTree[i];
	}
	
	return;
}

/*
 * Implements Costa's optimization
 * Doesn't do intervals yet
 * k = layer at which we switch from recomputing to remainder tree on each subtree
 */
void remainder_tree_v1(Vec<ZZ> &C, Vec<ZZ> &A, Vec<ZZ> &m, const int k){

	// Assert that lengths of A and m match
	assert(C.length() == A.length());
	assert(C.length() == m.length());

	// Set N = length of input arrays
	int N = C.length();

	// Change nothing if N = 0
	if (N == 0) {
		return;
	}

	// Ensure that there are at least k layers
	assert(N >= (1<<k));


	// Index of leaf at the bottom left
	int leftmost = 1 << ((int)ceil(log2(N)));

	// Declare Ctree (always of length 2N for any N)
	Vec<ZZ> CTree;
	CTree.SetLength(2 * N);

	/* 
	 * For example when N=11 the leaves are in this order:
	 *     / \       /\   /\    /\
	 *    /   \     /  7 8  9 10  11
	 *   /\   /\   /\  
	 *  1  2 3  4 5  6
	 *
	 */

	// Calculate the product of all the mods to keep A's small
	ZZ mProd = getNode(1, m, ZZ(0));

	// Step 1: Calculate the kth layer by recomputing everything necessary
	CTree[1] = 1;
	for (int i = 1; i < 1<<k; i++) {
		CTree[2 * i] = CTree[i] % getNode(2*i, m, ZZ(0)); // Left branch
		CTree[2 * i + 1] = (CTree[i] * getNode(2*i, A, mProd)) % getNode(2*i+1, m, ZZ(0)); // Right branch
		CTree[i].kill();
	}


	// Step 2: Calculate the subproduct trees
	// Roots are CTree[2^k + i]: {CTree[2^k], ..., CTree[2^(k+1)-1]}

	// First find index of root in subtree with leaves in both layers
	int notfirstk = ((int)log2(2*N-1) - k); // (#bits in 2*N-1) minus k
	int special = (2*N-1 - leftmost) >> notfirstk; // firstk digits excluding the most significant digit
	
	// Subtrees with leaves in first layer
	for(int i = 0; i < special; i++) {
		// Number of leaves: 2^notfirstk = leftmost/2^k
		//DEBUG// cout << "Calculating interval: [" << (i<<notfirstk) << ", " << ((i+1)<<notfirstk) << "]" << endl; 
		remainder_tree(C, A, m, CTree[(1<<k) + i], i<<notfirstk, (i+1)<<notfirstk);
	}

	// Subtree with leaves in both layers
	// First, calculate number of leaves in this subtree, stored in specialleaves
	int notfirstkdigits = (2*N-1) % (1<<notfirstk);
	int onenotfirstkdigits = (1<<notfirstk) + notfirstkdigits;
	int specialleaves = (onenotfirstkdigits+1)/2; 
	//DEBUG// cout << "Calculating interval: [" << (special<<notfirstk) << ", " << ((special<<notfirstk) + specialleaves) << "]" << endl;
	remainder_tree(C, A, m, CTree[(1<<k) + special], special<<notfirstk, (special<<notfirstk) + specialleaves);

	// Subtrees with leaves in second layer
	for(int i = special+1; i < 1<<k; i++){
		//DEBUG// cout << "Calculating interval: [" << ((special<<notfirstk) + specialleaves + ((i - special-1)<<(notfirstk-1))) << ", " << ((special<<notfirstk) + specialleaves + ((i - special)<<(notfirstk-1))) << "]" << endl; 
		remainder_tree(C, A, m, CTree[(1<<k) + i], (special<<notfirstk) + specialleaves + ((i - special-1)<<(notfirstk-1)), (special<<notfirstk) + specialleaves + ((i - special)<<(notfirstk-1)));
	}

	return;

}

/*
 * Returns the value of the node on the tree at index k with leaves having value base
 */
ZZ getNode(int i, Vec<ZZ> &base, ZZ mod) {
	int N = base.length();
	int leftmost = 1 << ((int)log2(N) + 1);
	if (mod == 0){
		if (i >= leftmost) return base[i - leftmost];
		else if (i >= N) return base[i + N - leftmost];
		
		return getNode(2*i, base, ZZ(0))*getNode(2*i+1, base, ZZ(0));
	}
	
	else {
		if (i >= leftmost) return base[i - leftmost] % mod;
		else if (i >= N) return base[i + N - leftmost] % mod;
		
		return (getNode(2*i, base, mod)*getNode(2*i+1, base, mod)) % mod;
	}
}

/*
 * Implements Sutherland's optimization
 * Doesn't do intervals yet
 */
void remainder_tree_v2(Vec<ZZ> &C, Vec<ZZ> &A, Vec<ZZ> &m){
	
}

/*
 * Prints a tree given in Vec<ZZ> form
 */
void print_tree(Vec<ZZ> tree){
	int top = 1;
	int counter = 0;
	for(int i = 1; i < tree.length(); i++){
		cout << tree[i] << " ";
		counter++;
		if (counter == top){
			cout << endl;
			counter = 0;
			top *= 2;
		}
	}
	cout << endl;
}

/*
 * Gives data points on size of input vs. computation time.
 * N = max size of data, d = number of data points
 */

/*
void complexity_graph(int N, int d){
	vector<int> x;
	vector<int> y;

	int interval = N/d;
	int B = 0;
	while(B <= N){

		int testSize = B;
		int numSize = B;
		
		Vec<ZZ> test_A;
		test_A.SetLength(testSize);
		Vec<ZZ> test_m;
		test_m.SetLength(testSize);
		for (int i = 0; i < testSize; i++) {
			test_A[i] = rand() % numSize + 1;
			test_m[i] = rand() % numSize + 1;
		}
		for (int i = 0; i < testSize; i++) {
			cout << test_A[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < testSize; i++) {
			cout << test_m[i] << " ";
		}
		cout << endl;
		

		uint64_t start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		Vec<ZZ> test_C = remainder_tree(test_A, test_m);

		for (int i = 0; i < testSize; i++) {
			cout << test_C[i] << " ";
		}
		cout << endl;
		

		uint64_t end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		x.push_back(B);
		y.push_back(end-start);

		B += interval;
	}

	for(int i = 0; i < x.size(); i++){
		cout << x[i] << ", ";
	}		
	cout << endl;

	for(int i = 0; i < y.size(); i++){
		cout << y[i] << ", ";
	}		
	cout << endl;


}

*/