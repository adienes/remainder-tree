/*
 * Range Segments > Forest > Recompute
 */

#include "RemTree_basic.hpp"
#include "RemTree_recompute.hpp"
#include "utils.hpp"

#include <cassert>

#include <vector>
using std::vector;


template <typename T, typename U>
vector<T> remainder_forest(const vector<T>& A, const vector<U>& m, int k, T V, U Y) {

	assert (A.size() == m.size());
	int N = A.size(); //Size of entire segment
	int M = (N<<k); //Number of leaves in one subtree

	//Ignore all the extra stuff if we just want basic remainder tree
	if (k == 0) {
		return remainder_tree<T,U>(A, m, V);
	}


	vector<T> C(N); //This will be the final output vector
	int C_offset = 0; //Could also be done as in RT_recompute; this finds the span of a subtree basically

	if (Y == 0) { //default value only! 0 should never be passed in.
		Y = compute_product_node<U>(m, 1);
	}
	//Unlike Y, we are ok with V default value

	//Iterates through the subtrees at depth k
	for (int s = 0; s < (1<<k); ++s) {
		int sub_root = flatten(k, s);


		vector<U> mTree = subproduct_tree<U> (m, sub_root); //We compute one subtree at a time
		vector<T> ATree = subproduct_tree_askew<T,U> (A, mTree[1], sub_root);

		vector<T> CTree(2*M);

		CTree[1] = A[C_offset]; //First A value of this subtree.
		//Note that ATree stores only right children, which is why it's hard to find A_0
		CTree[1].mulmod(V, mTree[1]);

		//We can now update V and Y to free up some space in advance
		Y /= mTree[1];
		V.mulmod(compute_product_node<T>(A, sub_root), Y);
		//Sadly have to recompute the product of A since I have been using the askew trees
		//TODO: Do not recompute the product of A, and also keep it reduced... this uses integer product.
		//However, integer product would be used here, even if we did not use the askew product tree,
		//as it would be contained in a larger modulus. Given this, perhaps the entire askew tree should
		//be computed under the large modulus? Then the necessary update to V can come just be traversing
		//the rightmost branches.

		for (int i = 1; i < M; ++i) { //only go up to second-lowest layer
			CTree[2*i] = CTree[i]%mTree[2*i];
			//CTree[2*i + 1] = (CTree[i]*ATree[2*i + 1])%mTree[2*i + 1];
		
			CTree[2*i + 1] = CTree[i];
			CTree[2*i + 1].mulmod(ATree[2*i + 1], mTree[2*i + 1]);

			CTree[i] = T(); //always kill the parent
		}

		for (int i = M; i < 2*M; ++i) { //For each leaf in the sub CTree
			C[i-M + C_offset] = CTree[i]; //Slot into output vector
		}
		C_offset += M;
	}

	return C;
}
