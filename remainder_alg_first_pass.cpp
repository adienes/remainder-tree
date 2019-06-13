#include <iostream>
#include <array>
#include <cmath>
#include <tuple>
#include <assert.h>


//note that everywhere it says 'int' we will eventually want to use GMP or whatnot
//ints are too small...

//translates indices in trees to indices in arrays
//if you want left child of (i,j), do (i+1, 2*j)
//right child is (i+1, 2*j+1), and parent is (i-1, j//2)
int flatten_index(int i, int j)
{
	assert (j <= pow(2,i)-1);
	return pow(2,i)+j-1;
}

//gives tree indices from array index
std::tuple<int, int> lift_index(int k)
{
	int i = log2(k+1);
	int j = k + 1 - pow(2,i);

	return std::make_tuple(i,j);
}


//given an array of size N, returns the product tree of size 2N
//assumes N is a power of 2
//it won't have to be (we can pad it etc.) but this is just a preliminary version
template<std::size_t N>
std::array<int, 2*N-1> product_tree(std::array<int, N> X)
{
	int depth = log2(N); //round this UP when not power of 2

	std::array<int, 2*N-1> ptree;

	//initialize leaves
	for (int j = 0; j < pow(2, depth); j++)
	{
		ptree[flatten_index(depth,j)] = X[j];
	}

	//build up the product tree recursively
	for (int i = depth-1; i >= 0; i--)
	{
		for (int j = 0; j < pow(2,i); j++)
		{
			ptree[flatten_index(i,j)] = ptree[flatten_index(i+1, 2*j)]*ptree[flatten_index(i+1, 2*j+1)];
		}
	}

	return ptree;
}


//given A0, A1, ... An, and m0, m1, ... mn, this returns
//1, A0 mod m1, A0*A1 mod m2, ... A0*...An-1 mod mn
template<std::size_t n>
std::array<int, n> accumulating_remainder_tree(std::array<int, n> A, std::array<int, n> m)
{
	int depth = log2(n);

	std::array<int, 2*n-1> m_ptree = product_tree(m);
	std::array<int, 2*n-1> A_ptree = product_tree(A);

	std::array<int, 2*n - 1> rtree = {1}; //initializes the root to be Id. Might want A0, or V, etc.

	for (int i = 0; i < depth; i++)
	{
		for (int j = 0; j < pow(2, i); j++)
		{
			int left = flatten_index(i+1, 2*j);
			int right = flatten_index(i+1, 2*j+1);
			int parent = flatten_index(i,j);

			//left child is mod, right child is multiply and mod
			rtree[left] = rtree[parent]%m_ptree[left];
			rtree[right] = (rtree[parent]*A_ptree[left])%m_ptree[right];
		}
	}

	//need a nice array slice here...
	std::array<int, n> C;

	for (int j = 0; j < pow(2, depth); j++)
	{
		C[j] = rtree[flatten_index(depth, j)];
	}

	return C;
}

int main()
{
	std::array<int, 8> A = {1,2,3,4,5,6,7,1};
	std::array<int, 8> m = {1,2,3,1,5,1,7,1};

	std::array<int, 8> remainders = accumulating_remainder_tree(A,m);

	for (int i = 0; i < remainders.size(); i++)
	{
		std::cout << remainders[i] << " , ";
	}
	std::cout << std::endl;
	return 0;
}