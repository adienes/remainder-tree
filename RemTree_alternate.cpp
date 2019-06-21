#include <iostream>
#include <array>
#include <cmath>
#include <tuple>
#include <cassert>
#include <NTL/ZZ.h>
#include <NTL/vector.h>
#include <random>
#include <chrono>

using namespace std;
using namespace NTL;

//ex. compile:
//g++ -o <exe> remainder_alg_first_pass.cpp -std=c++11 -march=native -O2 -lntl -lgmp -lgmpxx -pthread


// to compile and run on linux use
// g++ -o <executable file name> remainder_alg_first_pass.cpp
// ./<executable file name>


//note that everywhere it says 'int' we will eventually want to use GMP or whatnot
//ints are too small...

//translates indices in trees to indices in arrays
//if you want left child of (i,j), do (i+1, 2*j)
//right child is (i+1, 2*j+1), and parent is (i-1, j//2)
int flatten_index(int i, int j)
{
	assert (j <= pow(2,i) - 1);
	return pow(2,i) +j-1;
}

//gives tree indices from array index
tuple<int, int> lift_index(int k)
{
	int i = log2(k+1);
	int j = k + 1 - pow(2,i);

	return make_tuple(i,j);
}


void print_tree(Vec<ZZ> & X)
{
	cout << endl;
	for (int i = 0; i < X.length(); i++)
	{
		cout << X[i] << " , ";
	}
	cout << endl;
}


Vec<ZZ> alternate_tree(Vec<ZZ> &A, Vec<ZZ> &m)
{
	int N = A.length();
	assert (N == m.length());
	int depth = log2(N);

	//assume the input is of size a power of 2
	Vec<ZZ> mtree;
	mtree.SetLength(2*N-1);

	//initialize leaves
	for (int j = 0; j < pow(2, depth); j++)
	{
		int leaf = flatten_index(depth,j);
		mtree[leaf] = m[j];
	}

	//build up the product tree recursively
	for (int i = depth-1; i >= 0; i--)
	{
		for (int j = 0; j < pow(2,i); j++)
		{
			int parent = flatten_index(i,j);
			int left = 2*parent + 1;
			int right = left + 1;

			//padding is not necessary if on this step we catch IndexError with 1
			mtree[parent] = mtree[left]*mtree[right];
		}
	}



	Vec<ZZ> Atree;
	Atree.SetLength(2*N-1);

	//initialize leaves
	for (int j = 0; j < pow(2, depth); j++)
	{
		int leaf = flatten_index(depth, j);
		Atree[leaf] = A[j];
	}

	//build up the 'product' tree recursively
	for (int i = depth-1; i >= 0; i--)
	{
		int leftmost = flatten_index(i,0);
		int leftmost_child = 2*leftmost + 1;
		Atree[leftmost] = A[leftmost_child];

		for (int j = 1; j < pow(2, i); j++)
		{
			int parent = flatten_index(i,j);
			int superleft = 2*parent;
			int left = superleft + 1;

			Atree[parent] = Atree[superleft]*Atree[left];
		}
	}

	Vec<ZZ> remtree;
	remtree.SetLength(2*N-1);

	remtree[0] = A[0]%mtree[0];

	for (int i = 0; i < depth; i++)
	{
		//notice it goes every two!
		for (int j = 0; j < pow(2, i); j++)
		{	
			int parent = flatten_index(i,j);
			int left = 2*parent + 1;
			int right = left + 1;

			remtree[left] = remtree[parent]%mtree[left];
			remtree[right] = (remtree[parent]*Atree[right])%mtree[right];
		}
	}

	return remtree;
}



int main()
{
	long N = pow(2,24);

	Vec<ZZ> A;
	Vec<ZZ> m;

	A.SetLength(N);
	m.SetLength(N);

	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(1, N);

	for(int i = 0; i < N; i++){
		int bitsize = log2(i+1)+2;

		
		A[i] = dist(mt);
		m[i] = dist(mt);

	}


	//Vec<ZZ> ztree = zero_vector(2*N);

	//print_tree(A);
	//print_tree(m);

	auto start = chrono::high_resolution_clock::now();
	Vec<ZZ> remainders = alternate_tree(A,m);
	auto finish = chrono::high_resolution_clock::now();

	chrono::duration<double> runtime = finish-start;

	//print_tree(remainders);
	cout << remainders[0] << endl;
	cout << runtime.count() << endl;




}