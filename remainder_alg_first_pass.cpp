#include <iostream>
#include <array>
#include <cmath>
#include <tuple>
#include <assert.h>


//to compile this on a mac use
//c++ -std=c++11 -stdlib=libc++ remainder_alg_first_pass.cpp -o rafp


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


template<std::size_t N>
void print_tree(std::array<int, N> X)
{
	std::cout << std::endl;
	for (int i = 0; i < N; i++)
	{
		std::cout << X[i] << " , ";
	}
	std::cout << std::endl;
}


//reflects a tree on the y-axis in place
template<std::size_t N>
std::array<int, N> reflect_tree(std::array<int, N> X) //why doesn't mutability work the way I think it does?
{
	int depth = log2(N);

	for (int i = 1; i <= depth; i++)
	{
		int width = pow(2,i);

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
template<std::size_t N>
std::array<int, 2*N-1> product_tree(std::array<int, N> X, std::array<int, 2*N-1> mtree = {})
{
	//INPUT: a list of integers; OUTPUT: a product tree of double the size
	int depth = log2(N); //round this UP when not power of 2

	std::array<int, 2*N-1> ptree;

	//initialize leaves
	for (int j = 0; j < pow(2, depth); j++)
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
		for (int j = 0; j < pow(2,i); j++)
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


template<std::size_t n>
std::array<int, n> accumulating_tree(std::array<int, n> X, std::array<int, n> mtree = {})
{
	//code cut from body of accumulating_remainder_tree
	//this can be used to store the product tree of A modulo the mi
	//this can be applied to any tree; for our applications we will only apply it to product trees
	//INPUT: a tree; OUTPUT: accumulating tree (of same size)
	int depth = log2(n);

	std::array<int, n> acctree = {1};

	for (int i = 0; i < depth; i++)
	{
		for (int j = 0; j < pow(2, i); j++)
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
template<std::size_t n>
std::array<int, n> accumulating_remainder_tree(std::array<int, n> A, std::array<int, n> m)
{
	int depth = log2(n);
	std::array<int, 2*n-1> m_ptree = product_tree(m);

	std::array<int, 2*n-1> m_acctree = reflect_tree(accumulating_tree(reflect_tree(m_ptree)));


	//this power tree is reduced modulo the reflected m_ftree
	std::array<int, 2*n-1> A_ptree = product_tree(A, m_acctree);
	//delete m_acctree;

	print_tree(m_ptree);
	print_tree(m_acctree);
	print_tree(A_ptree);

	std::array<int, 2*n-1> A_rtree = accumulating_tree(A_ptree, m_ptree);

	//need a nice array slice here...
	std::array<int, n> C;

	for (int j = 0; j < pow(2, depth); j++)
	{
		C[j] = A_rtree[flatten_index(depth, j)];
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