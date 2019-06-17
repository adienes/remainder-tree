#include <iostream>
#include <array>
#include <cmath>
#include <tuple>
#include <cassert>
#include <NTL/ZZ.h>
#include <NTL/vector.h>

using namespace std;

using namespace NTL;

//to compile this on a mac use
//c++ -std=c++11 -stdlib=libc++ remainder_alg_first_pass.cpp -o rafp

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
	assert (j <= 1<<i -1);
	return 1<<i +j-1;
}

//gives tree indices from array index
tuple<int, int> lift_index(int k)
{
	int i = log2(k+1);
	int j = k + 1 - 1<<i;

	return make_tuple(i,j);
}


void print_tree(Vec<ZZ> X)
{
	cout << endl;
	for (int i = 0; i < X.length(); i++)
	{
		cout << X[i] << " , ";
	}
	cout << endl;
}


//reflects a tree on the y-axis in place
Vec<ZZ> reflect_tree(Vec<ZZ> X) //why doesn't mutability work the way I think it does?
{
	int depth = log2(X.length());

	for (int i = 1; i <= depth; i++)
	{
		int width = 1<<i;

		for (int j = 0; 2*j < width; j++)
		{
			int a = flatten_index(i,j);
			int b = flatten_index(i, width-j-1);

			//this switches the two locations
			X[b] = X[a]^X[b];
			X[a] = X[a]^X[b];
			X[b] = X[a]^X[b];
		}
	}
	return X;
}


//given an array of size N, returns the product tree of size 2N
//assumes N is a power of 2
//it won't have to be (we can pad it etc.) but this is just a preliminary version
Vec<ZZ> product_tree(Vec<ZZ> X, Vec<ZZ> mtree = {})
{
	//INPUT: a list of integers; OUTPUT: a product tree of double the size
	int depth = log2(X.length()); //round this UP when not power of 2

	Vec<ZZ> ptree;

	//initialize leaves
	for (int j = 0; j < 1<<depth; j++)
	{
		int leaf = flatten_index(depth,j);

		ptree[leaf] = X[j];

		if (mtree[leaf] != 0)
		{
			ptree[leaf] %= mtree[leaf];
		}
	}

	//build up the product tree recursively
	for (int i = depth-1; i >= 0; i--)
	{
		for (int j = 0; j < 1<<i; j++)
		{
			int parent = flatten_index(i,j);
			int left = 2*parent + 1;
			int right = left + 1;

			//padding is not necessary if on this step we catch IndexError with 1
			ptree[parent] = ptree[left]*ptree[right];

			if (mtree[parent] != 0)
			{
				ptree[parent] %= mtree[parent];
			}

		}
	}

	return ptree;
}


Vec<ZZ> accumulating_tree(Vec<ZZ> X, Vec<ZZ> mtree = {})
{
	//code cut from body of accumulating_remainder_tree
	//this can be used to store the product tree of A modulo the mi
	//this can be applied to any tree; for our applications we will only apply it to product trees
	//INPUT: a tree; OUTPUT: accumulating tree (of same size)
	int depth = log2(X.length());

	Vec<ZZ> acctree[0] = 1;

	for (int i = 0; i < depth; i++)
	{
		for (int j = 0; j < 1<<i; j++)
		{
			int parent = flatten_index(i,j);
			int left = 2*parent + 1;
			int right = left+1;

			acctree[left] = acctree[parent];
			acctree[right] = acctree[parent]*X[left];

			if (mtree[left] != 0)
			{
				acctree[left] %= mtree[left];
			}

			if (mtree[right] != 0)
			{
				acctree[right] %= mtree[right];
			}
		}
	}

	return acctree;

}



//so might make sense to pad beginning of m with a 1, and pad end of A with a 1
//given A0, A1, ... An, and m0, m1, ... mn, this returns
//1, A0 mod m1, A0*A1 mod m2, ... A0*...An-1 mod mn
Vec<ZZ> accumulating_remainder_tree(Vec<ZZ> A, Vec<ZZ> m)
{
	int depth = log2(A.length());
	Vec<ZZ> m_ptree = product_tree(m);

	Vec<ZZ> m_acctree = reflect_tree(accumulating_tree(reflect_tree(m_ptree)));


	//this power tree is reduced modulo the reflected m_ftree
	Vec<ZZ> A_ptree = product_tree(A, m_acctree);
	//delete m_acctree;

	print_tree(m_ptree);
	print_tree(m_acctree);
	print_tree(A_ptree);

	Vec<ZZ> A_rtree = accumulating_tree(A_ptree, m_ptree);

	//need a nice array slice here...
	Vec<ZZ> C;

	for (int j = 0; j < 1<<depth; j++)
	{
		C[j] = A_rtree[flatten_index(depth, j)];
	}

	return C;
	
}





int main()
{
	array<int, 8> Aint = {1,2,3,4,5,6,7,1};
	array<int, 8> mint = {1,2,3,1,5,1,7,1};
	Vec<ZZ> A;
	Vec<ZZ> m;
	for(int i = 0; i < 8; i++){
		A[i] = Aint[i];
		m[i] = mint[i];
	}

	Vec<ZZ> remainders = accumulating_remainder_tree(A,m);

	print_tree(remainders);	
}