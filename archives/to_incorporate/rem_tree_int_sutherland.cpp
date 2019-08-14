#include <iostream>
#include <cmath>
#include <chrono>
#include <cassert>
#include <vector>
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include "Element.hpp"

using namespace std;
using namespace std::chrono;
using namespace NTL;

void test_wilson(int bound);
void test_wolstenholme(int bound);
void test_kurepa(int bound);

template<typename T, typename M>
void remainder_tree(vector<Elt<T>> &C, vector<Elt<T>> &A, vector<Elt<T>> &m, Elt<T> &AProd, Elt<M> &mProd, Elt<T> const &root_value = Elt<T>(1), int start = 0, int end = -1);
// void remainder_tree_v1(vector<Elt<T>> &C, vector<Elt<T>> &A, vector<Elt<M>> &m, Elt<T> const &root_value = Elt<T>(1), const int k = 2);
template<typename T, typename M>
Elt<T> get_node(int index, vector<Elt<T>> &base, Elt<M> const &mod = Elt<M>(0));
template<typename T, typename M>
void remainder_tree_v2(vector<Elt<T>> &C, vector<Elt<T>> &A, vector<Elt<M>> &m, Elt<T> const &root_value = Elt<T>(1), const int k = 2);
template<typename T>
void print_tree(vector<Elt<T>> tree);
void complexity_graph(int N, int d);

/* Tags:
 *
 * //DEBUG// for debug statements
 * Optimization idea for optimizations that havent been impemented yet
 * TODO: add typename specifications to functions when they are called
 * TODO: add .t whenever need to access a value of Elt
 */

int main(){
	
	complexity_graph(1<<24, 10);
}

template<typename T>
void elt_to_base(vector<T> &b, vector<Elt<T>> &e){
	assert(b.size() == e.size());
	for(int i = 0; i < b.size(); i++){
		b[i] = e[i].t;
	}
}

template<typename T>
void base_to_elt(vector<Elt<T>> &e, vector<T> &b){
	assert(e.size() == b.size());
	for(int i = 0; i < e.size(); i++){
		e[i] = b[i];
	}
}

// Test for Wilson Primes
void test_wilson(int bound){

	vector<ZZ> A;
	A.resize(bound);
	vector<ZZ> m;
	m.resize(bound);

	vector<Elt<ZZ>> A_elt;
	A_elt.resize(bound);
	vector<Elt<ZZ>> m_elt;
	m_elt.resize(bound);

	base_to_elt(A_elt, A);
	base_to_elt(m_elt, m);
	

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
	vector<Elt<ZZ>> C_elt;
	C_elt.resize(bound);

	base_to_elt(C_elt, C);

	remainder_tree_v2<ZZ, ZZ>(C_elt, A_elt, m_elt, ZZ(1), 4);
	
	elt_to_base(C, C_elt);

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

	vector<Elt<ZZ>> Anum_elt; 
	Anum_elt.resize(bound);
	vector<Elt<ZZ>> m_elt;
	m_elt.resize(bound);

	
	for(int i = 1; i <= bound; i++){
		Anum[i-1] = 4*i+2;
		m[i-1] = ProbPrime(ZZ(i)) ? ZZ(i)*ZZ(i)*ZZ(i)*ZZ(i)*ZZ(i) : ZZ(1);

	}		

	base_to_elt(Anum_elt, Anum);
	base_to_elt(m_elt, m);

	vector<ZZ> Cnum;
	Cnum.resize(bound);
	vector<Elt<ZZ>> Cnum_elt;
	Cnum_elt.resize(bound);

	remainder_tree_v2<ZZ, ZZ>(Cnum_elt, Anum_elt, m_elt, ZZ(1), 4);

	elt_to_base(Cnum, Cnum_elt);

	for(int i = 1; i <= bound; i++){
		Cnum[i-1] /= i; // i = prime at this index
	}

	
	vector<ZZ> Adem;
	Adem.resize(bound);
	vector<Elt<ZZ>> Adem_elt;
	Adem_elt.resize(bound);

	for(int i = 1; i <= bound; i++){
		Adem[i-1] = i+1;
	}

	base_to_elt(Adem_elt, Adem);

	vector<ZZ> Cdem;
	Cdem.resize(bound);
	vector<Elt<ZZ>> Cdem_elt;
	Cdem_elt.resize(bound);

	remainder_tree_v2<ZZ, ZZ>(Cdem_elt, Adem_elt, m_elt, ZZ(1), 4);

	elt_to_base(Cdem, Cdem_elt);

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

template<typename T, typename M>
void remainder_tree(vector<Elt<T>> &C, vector<Elt<T>> &A, vector<Elt<M>> &m, Elt<T> &AProd, Elt<M> &mProd, Elt<T> const &root_value, int start, int end){
	
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
	vector<Elt<T>> ATree;
	ATree.resize(2 * N);
	vector<Elt<M>> mTree;
	mTree.resize(2 * N);
	vector<Elt<T>> CTree;
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
		delete ATree[2 * i + 1];
	}

	mProd /= mTree[1]; // Get rid of this tree's moduli from mProd
	AProd = ATree[1]; // Set AProd as the product of A's mod new mProd

	// Calculate accumulating remainder tree
	CTree[1] = root_value % mTree[1];
	//DEBUG// cout << "CTree root: " << CTree[1] << endl;
	for (int i = 1; i < N; i++) {
		CTree[2 * i] = CTree[i] % mTree[2 * i]; // Left branch
		CTree[2 * i + 1] = (CTree[i] * ATree[2 * i]) % mTree[2 * i + 1]; // Right branch
		delete CTree[i];
	}

	//DEBUG// print_tree<ZZ>(ATree);
	//DEBUG// print_tree<ZZ>(mTree);
	//DEBUG// print_tree<ZZ>(CTree);
	
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
template<typename T, typename M>
void remainder_tree_v2(vector<Elt<T>> &C, vector<Elt<T>> &A, vector<Elt<M>> &m, Elt<T> const &root_value, const int k){

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
	vector<Elt<T>> CTree;
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
	// Elt<M> mProd = get_node<ZZ, ZZ>(1, m, Elt<M>(0));

	uint64_t start2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	
	// Step 2: Calculate the subproduct trees
	// Roots are CTree[2^k + i]: {CTree[2^k], ..., CTree[2^(k+1)-1]}

	// First find index of root in subtree with leaves in both layers
	int notfirstk = ((int)log2(2*N-1) - k); // (#bits in 2*N-1) minus k
	int special = (2*N-1 - leftmost) >> notfirstk; // firstk digits excluding the most significant digit

	Elt<T> AProd = Elt<T>(1);
	Elt<M> mProd = get_node<ZZ, ZZ>(1, m, Elt<M>(0));	
	CTree[1<<k] = root_value % mProd;
	// Subtrees with leaves in first layer
	for(int i = 0; i < special; i++) {
		// Number of leaves: 2^notfirstk = leftmost/2^k
		//DEBUG// cout << "Calculating interval: [" << (i<<notfirstk) << ", " << ((i+1)<<notfirstk) << "]" << endl; 
		remainder_tree<ZZ, ZZ>(C, A, m, AProd, mProd, CTree[(1<<k) + i], i<<notfirstk, (i+1)<<notfirstk);
		CTree[(1<<k) + i+1] = (CTree[(1<<k) + i] * AProd) % mProd;
	}

	// Subtree with leaves in both layers
	// First, calculate number of leaves in this subtree, stored in specialleaves
	int notfirstkdigits = (2*N-1) % (1<<notfirstk);
	int onenotfirstkdigits = (1<<notfirstk) + notfirstkdigits;
	int specialleaves = (onenotfirstkdigits+1)/2; 
	//DEBUG// cout << "Calculating interval: [" << (special<<notfirstk) << ", " << ((special<<notfirstk) + specialleaves) << "]" << endl;
	remainder_tree<ZZ, ZZ>(C, A, m, AProd, mProd, CTree[(1<<k) + special], special<<notfirstk, (special<<notfirstk) + specialleaves);
	CTree[(1<<k) + special+1] = (CTree[(1<<k) + special] * AProd) % mProd;

	// Subtrees with leaves in second layer
	for(int i = special+1; i < 1<<k; i++){
		//DEBUG// cout << "Calculating interval: [" << ((special<<notfirstk) + specialleaves + ((i - special-1)<<(notfirstk-1))) << ", " << ((special<<notfirstk) + specialleaves + ((i - special)<<(notfirstk-1))) << "]" << endl; 
		remainder_tree<ZZ, ZZ>(C, A, m, AProd, mProd, CTree[(1<<k) + i], (special<<notfirstk) + specialleaves + ((i - special-1)<<(notfirstk-1)), (special<<notfirstk) + specialleaves + ((i - special)<<(notfirstk-1)));
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
template<typename T, typename M>
Elt<T> get_node(int i, vector<Elt<T>> &base, Elt<M> const &mod) { // Optimization idea: pass in what you're taking a mod of as well so if the modulus ever gets bigger than the value, just return the value
	int N = base.size();
	int leftmost = 1 << ((int)ceil(log2(N)));
	if (mod == 0){
		if (i >= leftmost) return base[i - leftmost];
		else if (i >= N) return base[i + N - leftmost];
		
		return get_node<ZZ, ZZ>(2*i, base, Elt<M>(0))*get_node<ZZ, ZZ>(2*i+1, base, Elt<M>(0));
	}
	
	else {
		if (i >= leftmost) return base[i - leftmost] % mod;
		else if (i >= N) return base[i + N - leftmost] % mod;
		
		return (get_node<ZZ, ZZ>(2*i, base, mod)*get_node<ZZ, ZZ>(2*i+1, base, mod)) % mod;
	}
}

/*
 * Prints a tree given in vector<ZZ> form
 */
template<typename T>
void print_tree(vector<Elt<T>> tree){
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
		//remainder_tree<ZZ, ZZ>(test_C, test_A, test_m);
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
