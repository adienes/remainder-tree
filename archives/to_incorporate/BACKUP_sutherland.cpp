#include <iostream>
#include <cmath>
#include <chrono>
#include <cassert>
#include <vector>
#include <NTL/ZZ.h>
#include <NTL/vector.h>

using namespace std;
using namespace std::chrono;
using namespace NTL;

void test_wilson(int bound);
void test_wolstenholme(int bound);
void test_kurepa(int bound);

void remainder_tree(vector<ZZ> &C, vector<ZZ> &A, vector<ZZ> &m, ZZ &AProd, ZZ &mProd, ZZ const &root_value, int start, int end);
// void remainder_tree_v1(vector<ZZ> &C, vector<ZZ> &A, vector<ZZ> &m, ZZ const &root_value, const int k);
ZZ get_node(int index, vector<ZZ> &base, ZZ const &mod);
void remainder_tree_v2(vector<ZZ> &C, vector<ZZ> &A, vector<ZZ> &m, ZZ const &root_value, const int k);
void print_tree(vector<ZZ> tree);
void complexity_graph(int N, int d);

/* Tags:
 *
 * //DEBUG// for debug statements
 * Optimization idea for optimizations that havent been impemented yet
 */

int main(){
	
	complexity_graph(1<<24, 10);
}

// Test for Wilson Primes
void test_wilson(int bound){

	vector<ZZ> A;
	A.resize(bound);
	vector<ZZ> m;
	m.resize(bound);

	// make sure implicit ints don't overflow	
	for(int i = 1; i <= bound; i++){
		A[i-1] = ZZ(i);
		m[i-1] = ProbPrime(ZZ(i)) ? ZZ(i)*ZZ(i) : ZZ(1);
	}
	
	/*	
	for(int i = 0; i < A.size(); i++){
		cout << A[i] << " ";
	}
	cout << endl;

	for(int i = 0; i < m.size(); i++){
		cout << m[i] << " ";
	}
	cout << endl;
	*/

	vector<ZZ> C;
	C.resize(bound);

	remainder_tree_v2(C, A, m, ZZ(1), 4);
	
	/*	
	for(int i = 0; i < C.size(); i++){
		cout << (i+1) << ": " << C[i] << endl;
	}
	cout << endl;
	*/
}

// Test for Wolstenholme Primes
void test_wolstenholme(int bound){
	
	vector<ZZ> Anum; 
	Anum.resize(bound);
	vector<ZZ> m;
	m.resize(bound);
	
	for(int i = 1; i <= bound; i++){
		Anum[i-1] = 4*i+2;
		m[i-1] = ProbPrime(ZZ(i)) ? ZZ(i)*ZZ(i)*ZZ(i)*ZZ(i)*ZZ(i) : ZZ(1);

	}		
	
	vector<ZZ> Cnum;
	Cnum.resize(bound);

	remainder_tree_v2(Cnum, Anum, m, ZZ(1), 4);

	for(int i = 1; i <= bound; i++){
		Cnum[i-1] /= i; // i = prime at this index
	}

	
	vector<ZZ> Adem;
	Adem.resize(bound);

	for(int i = 1; i <= bound; i++){
		Adem[i-1] = i+1;
	}

	vector<ZZ> Cdem;
	Cdem.resize(bound);

	remainder_tree_v2(Cdem, Adem, m, ZZ(1), 4);

	for(int i = 1; i <= bound; i++){
		Cdem[i-1] /= i;
		ZZ d;
		ZZ k;
		XGCD(d, Cdem[i-1], k, Cdem[i-1], ZZ(i)*ZZ(i)*ZZ(i)*ZZ(i)); // changes Cdem[i-1] to its inverse mod p^4
		if (Cdem[i-1] < 0) Cdem[i-1] += ZZ(i)*ZZ(i)*ZZ(i)*ZZ(i); // make residue positive

		Cnum[i-1] = (Cnum[i-1] * Cdem[i-1]) % (ZZ(i)*ZZ(i)*ZZ(i)*ZZ(i));
	}

	for(int i = 1; i <= bound; i++){
		if (Cnum[i-1] == 1){
			cout << i << ": " << Cnum[i-1] << endl;
		}
	}

}

void test_kurepa(int bound){
	// need to first implement remainder tree for matrices
}

/*
 * Original Remainder Tree implementation
 */

void remainder_tree(vector<ZZ> &C, vector<ZZ> &A, vector<ZZ> &m, ZZ &AProd, ZZ &mProd, ZZ const &root_value = ZZ(1), int start = 0, int end = -1) {
	
	//DEBUG// cout << "AProd: " << AProd << endl;
	//DEBUG// cout << "mProd: " << mProd << endl;
	//DEBUG// cout << "root_value: " << root_value << endl;
	// set default value for end
	if (end == -1) end = C.size();

	// Assert that interval [start, end] exists in C, A and m
	assert(end <= C.size());
	assert(end <= A.size());
	assert(end <= m.size());

	// Set N = length of interval
	int N = end - start;

	// Change nothing if N = 0
	if (N == 0) {
		return;
	}

	// Index of leaf at the bottom left
	int leftmost = 1 << ((int)ceil(log2(N)));

	// Declare trees (always of length 2N for any N)
	vector<ZZ> ATree;
	ATree.resize(2 * N);
	vector<ZZ> mTree;
	mTree.resize(2 * N);
	vector<ZZ> CTree;
	CTree.resize(2 * N);

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
		ATree[i] = (ATree[2 * i] * ATree[2 * i + 1]) % mProd; // parent is product of leaves mod mTree[1]
		ATree[2 * i] %= mTree[1];
		ATree[2 * i + 1].kill();
	}

	mProd /= mTree[1]; // Get rid of this tree's moduli from mProd
	AProd = ATree[1]; // Set AProd as the product of A's mod new mProd

	// Calculate accumulating remainder tree
	CTree[1] = root_value % mTree[1];
	//DEBUG// cout << "CTree root: " << CTree[1] << endl;
	for (int i = 1; i < N; i++) {
		CTree[2 * i] = CTree[i] % mTree[2 * i]; // Left branch
		CTree[2 * i + 1] = (CTree[i] * ATree[2 * i]) % mTree[2 * i + 1]; // Right branch
		CTree[i].kill();
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
 * Implements Sutherland's optimization
 * Doesn't do intervals yet
 * k = layer where we divide into subtrees
 */
void remainder_tree_v2(vector<ZZ> &C, vector<ZZ> &A, vector<ZZ> &m, ZZ const &root_value = ZZ(1), const int k = 2){

	// Assert that lengths of A and m match
	assert(C.size() == A.size());
	assert(C.size() == m.size());

	// Set N = length of input arrays
	int N = C.size();

	// Change nothing if N = 0
	if (N == 0) {
		return;
	}

	// Ensure that there are at least k layers
	assert(N >= (1<<k));


	// Index of leaf at the bottom left
	int leftmost = 1 << ((int)ceil(log2(N)));

	// Declare Ctree (always of length 2N for any N)
	vector<ZZ> CTree;
	CTree.resize(2 * N);

	/* 
	 * For example when N=11 the leaves are in this order:
	 *     / \       /\   /\    /\
	 *    /   \     /  7 8  9 10  11
	 *   /\   /\   /\
	 *  1  2 3  4 5  6
	 *
	 */

	// Calculate the product of all the mods to keep A's small
	// ZZ mProd = get_node(1, m, ZZ(0));

	uint64_t start2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	
	// Step 2: Calculate the subproduct trees
	// Roots are CTree[2^k + i]: {CTree[2^k], ..., CTree[2^(k+1)-1]}

	// First find index of root in subtree with leaves in both layers
	int notfirstk = ((int)log2(2*N-1) - k); // (#bits in 2*N-1) minus k
	int special = (2*N-1 - leftmost) >> notfirstk; // firstk digits excluding the most significant digit

	ZZ AProd = ZZ(1);
	ZZ mProd = get_node(1, m, ZZ(0));	
	CTree[1<<k] = root_value % mProd;
	// Subtrees with leaves in first layer
	for(int i = 0; i < special; i++) {
		// Number of leaves: 2^notfirstk = leftmost/2^k
		//DEBUG// cout << "Calculating interval: [" << (i<<notfirstk) << ", " << ((i+1)<<notfirstk) << "]" << endl; 
		remainder_tree(C, A, m, AProd, mProd, CTree[(1<<k) + i], i<<notfirstk, (i+1)<<notfirstk);
		CTree[(1<<k) + i+1] = (CTree[(1<<k) + i] * AProd) % mProd;
	}

	// Subtree with leaves in both layers
	// First, calculate number of leaves in this subtree, stored in specialleaves
	int notfirstkdigits = (2*N-1) % (1<<notfirstk);
	int onenotfirstkdigits = (1<<notfirstk) + notfirstkdigits;
	int specialleaves = (onenotfirstkdigits+1)/2; 
	//DEBUG// cout << "Calculating interval: [" << (special<<notfirstk) << ", " << ((special<<notfirstk) + specialleaves) << "]" << endl;
	remainder_tree(C, A, m, AProd, mProd, CTree[(1<<k) + special], special<<notfirstk, (special<<notfirstk) + specialleaves);
	CTree[(1<<k) + special+1] = (CTree[(1<<k) + special] * AProd) % mProd;

	// Subtrees with leaves in second layer
	for(int i = special+1; i < 1<<k; i++){
		//DEBUG// cout << "Calculating interval: [" << ((special<<notfirstk) + specialleaves + ((i - special-1)<<(notfirstk-1))) << ", " << ((special<<notfirstk) + specialleaves + ((i - special)<<(notfirstk-1))) << "]" << endl; 
		remainder_tree(C, A, m, AProd, mProd, CTree[(1<<k) + i], (special<<notfirstk) + specialleaves + ((i - special-1)<<(notfirstk-1)), (special<<notfirstk) + specialleaves + ((i - special)<<(notfirstk-1)));
		if (i == (1<<k) - 1) continue; // Prevent index out of range for next operation
		CTree[(1<<k) + i+1] = (CTree[(1<<k) + i] * AProd) % mProd;
	}

	uint64_t end2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	//DEBUG// cout << "Time taken for subtree step: " << (end2-start2) << endl;

	return;

}

/*
 * Returns the value of the node on the tree at index k with leaves having value base
 */
ZZ get_node(int i, vector<ZZ> &base, ZZ const &mod = ZZ(0)) { // Optimization idea: pass in what you're taking a mod of as well so if the modulus ever gets bigger than the value, just return the value
	int N = base.size();
	int leftmost = 1 << ((int)ceil(log2(N)));
	if (mod == 0){
		if (i >= leftmost) return base[i - leftmost];
		else if (i >= N) return base[i + N - leftmost];
		
		return get_node(2*i, base, ZZ(0))*get_node(2*i+1, base, ZZ(0));
	}
	
	else {
		if (i >= leftmost) return base[i - leftmost] % mod;
		else if (i >= N) return base[i + N - leftmost] % mod;
		
		return (get_node(2*i, base, mod)*get_node(2*i+1, base, mod)) % mod;
	}
}

/*
 * Prints a tree given in vector<ZZ> form
 */
void print_tree(vector<ZZ> tree){
	int top = 1;
	int counter = 0;
	for(int i = 1; i < tree.size(); i++){
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

void complexity_graph(int N, int d){
	vector<int> x;
	vector<int> y;
	vector<int> z;

	int interval = N/d;
	int B = 0;
	while(B <= N){
		cout << "Testing: " << B << endl;
		/*
		int testSize = B;
		int numSize = B;
		
		vector<ZZ> test_A;
		test_A.resize(testSize);
		vector<ZZ> test_m;
		test_m.resize(testSize);
		for (int i = 0; i < testSize; i++) {
			test_A[i] = rand() % numSize + 1;
			test_m[i] = rand() % numSize + 1;
		}

		vector<ZZ> test_C;
		test_C.resize(testSize);
		*/

		x.push_back(B);
		
		
		uint64_t start;
		uint64_t end;
		
		start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		//remainder_tree(test_C, test_A, test_m);
		end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

		y.push_back(end-start);


		start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		test_wilson(B);
		end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		
		z.push_back(end-start);

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
	for(int i = 0; i < z.size(); i++){
		cout << z[i] << ", ";
	}
	cout << endl;

}
