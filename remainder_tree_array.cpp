// remainder_tree_array.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <cassert>
#include <random>

using namespace std;

/* 
 * Returns C[], an array of residues A0 mod m0, A0A1 mod m0m1, etc.
 * A: array of A0, A1, ...
 * m: array of m0, m1, ...
 */
vector<int> remainder_tree(vector<int> A, vector<int> m);

int main()
{
	//int A[5] = { 1, 2, 3, 4, 5 };
	//int newA = test(A, 5);
	const int testSize = 10;
	const int numSize = 10;

	vector<int> test_A(testSize);
	vector<int> test_m(testSize);
	for (int i = 0; i < testSize; i++) {
		test_A[i] = rand() % numSize + 1;
		test_m[i] = rand() % numSize + 1;
	}

	for (int i = 0; i < numSize; i++) {
		cout << test_A[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < numSize; i++) {
		cout << test_m[i] << " ";
	}
	cout << endl;


	vector<int> test_C = remainder_tree(test_A, test_m);

	for (int i = 0; i < numSize; i++) {
		cout << test_C[i] << " ";
	}

}

vector<int> remainder_tree(vector<int> A, vector<int> m) {
	assert(A.size() == m.size());

	int N = A.size();
	if (N == 0) return vector<int>(0);

	int leftmost = 1 << ((int)log2(N) + 1);

	vector<int> ATree(2 * N);
	vector<int> mTree(2 * N);
	vector<int> CTree(2 * N);

	for (int i = leftmost; i < 2 * N; i++) {
		ATree[i] = A[i - leftmost];
		mTree[i] = m[i - leftmost];
	}
	for (int i = N; i < leftmost; i++) {
		ATree[i] = A[i + N - leftmost];
		mTree[i] = m[i + N - leftmost];
	}

	for (int i = N - 1; i > 0; i--) {
		if (i & (i + 1) != 0) ATree[i] = ATree[2 * i] * ATree[2 * i + 1];
		mTree[i] = mTree[2 * i] * mTree[2 * i + 1];
	}

	CTree[1] = 1;
	for (int i = 1; i < N; i++) {
		CTree[2 * i] = CTree[i] % mTree[2 * i];
		CTree[2 * i + 1] = (CTree[i] * ATree[2 * i]) % mTree[2 * i + 1];
	}

	vector<int> C(N);

	for (int i = leftmost; i < 2 * N; i++) {
		C[i - leftmost] = CTree[i];
	}
	for (int i = N; i < leftmost; i++) {
		C[i + N - leftmost] = CTree[i];
	}

	return C;
}

int* test(int A[], int N) {
	int* C = new int[N];

	for (int i = 0; i < N; i++) {
		C[i] = A[i] * 2;
	}

	return C;
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
