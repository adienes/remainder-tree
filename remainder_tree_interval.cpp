#include <iostream>
#include <chrono>
#include <NTL/ZZ.h>
#include <NTL/vector.h>

using namespace std;
using namespace std::chrono;
using namespace NTL;

Vec<ZZ> remainder_tree_v1(Vec<ZZ> &A, Vec<ZZ> &m);
ZZ getNode(int index, Vec<ZZ> &base);
Vec<ZZ> remainder_tree_v2(Vec<ZZ> &A, Vec<ZZ> &m);
void print_tree(Vec<ZZ> tree);
void complexity_graph(int N, int d);

int main(){

}

/*
 * Implements Costa's optimization
 * Doesn't do intervals yet
 */
Vec<ZZ> remainder_tree_v1(Vec<ZZ> &A, Vec<ZZ> &m){
	// layer at which we switch from recomputing to remainder tree on each subtree
	const int k = 4; 

	// Assert that lengths of A and m match
	assert(A.length() == m.length());

	// Set N = length of input arrays
	int N = A.length();

	// Return default value if N = 0
	if (N == 0) {
		Vec<ZZ> v;
		v.SetLength(0);
		return v;
	}

	// Index of leaf at the bottom left
	int leftmost = 1 << ((int)log2(N) + 1);

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
	mProd = getNode(1, m, 0);

	// Step 1: Calculate the kth layer by recomputing everything necessary
	CTree[1] = 1;
	for (int i = 1; i < 1<<k; i++) {
		CTree[2 * i] = CTree[i] % getNode(2*i, m, 0); // Left branch
		CTree[2 * i + 1] = (CTree[i] * getNode(2*i, A, mProd)) % getNode(2*i+1, m, 0); // Right branch
		CTree[i].kill();
	}

	// Step 2: Calculate the subproduct trees
	// Roots are: CTree[2^k], ..., CTree[2^(k+1)-1]
	for(int i = 0; i < 1<<k; i++) {
		Vec<ZZ> ATree;
		ATree.SetLength(2 * N);
		Vec<ZZ> mTree;
		mTree.SetLength(2 * N);
	}

	// Initialize the leaves in ATree and mTree
	for (int i = leftmost; i < 2 * N; i++) { // leaves on lowest layer
		ATree[i] = A[i - leftmost];
		mTree[i] = m[i - leftmost];
	}
	for (int i = N; i < leftmost; i++) { // leaves on second lowest layer
		ATree[i] = A[i + N - leftmost];
		mTree[i] = m[i + N - leftmost];
	}





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
		
		return getNode(2*i, base)*getNode(2*i+1, base);
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
Vec<ZZ> remainder_tree_v2(Vec<ZZ> &A, Vec<ZZ> &m){
	
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
		/*
		for (int i = 0; i < testSize; i++) {
			cout << test_A[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < testSize; i++) {
			cout << test_m[i] << " ";
		}
		cout << endl;
		*/

		uint64_t start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		Vec<ZZ> test_C = remainder_tree(test_A, test_m);

		/*for (int i = 0; i < testSize; i++) {
			cout << test_C[i] << " ";
		}
		cout << endl;
		*/

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
