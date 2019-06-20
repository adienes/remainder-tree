// remainder_tree_array.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <cmath>
#include <cassert>
#include <random>
#include <vector>
#include <NTL/ZZ.h>
#include <NTL/vector.h>

using namespace std;
using namespace std::chrono;
using namespace NTL;
/*
 * To compile and run in Linux:
 * g++ -o remainder_tree_array remainder_tree_array.cpp -lntl -lgmp -pthread
 * ./remainder_tree_array
 *
 */

/* 
 * Returns C[], an array of residues A0 mod m0, A0A1 mod m0m1, etc.
 * A: array of A0, A1, ...
 * m: array of m0, m1, ...
 */
Vec<ZZ> remainder_tree(Vec<ZZ> A, Vec<ZZ> m);
void print_tree(Vec<ZZ> tree);
void complexity_graph(int N, int d);

int main()
{
	// for testing with Wilson's theorem

	/*int testSize = 20;
	int arr_m[testSize] = {1, 2, 3, 1, 5, 1, 7, 1, 1, 1, 11, 1, 13, 1, 1, 1, 17, 1, 19, 1};

	Vec<ZZ> test_A;
	test_A.SetLength(testSize);
	Vec<ZZ> test_m;
	test_m.SetLength(testSize);
	for (int i = 0; i < testSize; i++) {
		//test_A[i] = rand() % numSize + 1;
		//test_m[i] = rand() % numSize + 1;
		test_A[i] = i+1;
		test_m[i] = arr_m[i];
	}
	
	for (int i = 0; i < testSize; i++) {
		cout << test_A[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < testSize; i++) {
		cout << test_m[i] << " ";
	}
	cout << endl;
	

	Vec<ZZ> test_C = remainder_tree(test_A, test_m);

	for (int i = 0; i < testSize; i++) {
		cout << test_C[i] << " ";
	}
	cout << endl;
	*/


	complexity_graph(300000, 1000);
	

}

Vec<ZZ> remainder_tree(Vec<ZZ> A, Vec<ZZ> m) {
	assert(A.length() == m.length());

	int N = A.length();
	if (N == 0) {
		Vec<ZZ> v;
		v.SetLength(0);
		return v;
	}

	int leftmost = 1 << ((int)log2(N) + 1);

	Vec<ZZ> ATree;
	ATree.SetLength(2 * N);
	Vec<ZZ> mTree;
	mTree.SetLength(2 * N);
	Vec<ZZ> CTree;
	CTree.SetLength(2 * N);

	for (int i = leftmost; i < 2 * N; i++) {
		ATree[i] = A[i - leftmost];
		mTree[i] = m[i - leftmost];
	}
	for (int i = N; i < leftmost; i++) {
		ATree[i] = A[i + N - leftmost];
		mTree[i] = m[i + N - leftmost];
	}

	for (int i = N - 1; i > 0; i--) {
		if ((i & (i+1)) != 0) ATree[i] = ATree[2 * i] * ATree[2 * i + 1];
		mTree[i] = mTree[2 * i] * mTree[2 * i + 1];
	}

	CTree[1] = 1;
	for (int i = 1; i < N; i++) {
		CTree[2 * i] = CTree[i] % mTree[2 * i];
		CTree[2 * i + 1] = (CTree[i] * ATree[2 * i]) % mTree[2 * i + 1];
	}

	//print_tree(ATree);
	//print_tree(mTree);
	//print_tree(CTree);
	
	Vec<ZZ> C;
	C.SetLength(N);

	for (int i = leftmost; i < 2 * N; i++) {
		C[i - leftmost] = CTree[i];
	}
	for (int i = N; i < leftmost; i++) {
		C[i + N - leftmost] = CTree[i];
	}

	return C;
}


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

void complexity_graph(int N, int d){
	vector<int> x;
	vector<int> y;

	int interval = N/d;
	int B = 0;
	while(B < N){

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

		time_t end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

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


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
