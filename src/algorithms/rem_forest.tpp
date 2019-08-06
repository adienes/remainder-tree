/*
 * Range Segments > Forest > Recompute
 */

#include "intermediate_computation.hpp"
#include "utils.hpp"

#include <cassert>

#include <vector>
using std::vector;

template <typename T, typename U>
vector<T> compute_remainder_tree_layer(const vector<T>& A, const vector<U>& m, long l, T& V, U& Y, bool update_forest_vals) {
	//level is 0-indexed, so the left most is (1<<l)
	//thus, l = 1 corresponds to the first children of the root

	assert (A.size() == m.size());

	if (l == 0) {
		U modulus = compute_product_node<U>(m, 1);
		
		if (update_forest_vals) {
			Y.div(modulus);
		}

		vector<T> root = {V};
		root[0].mulmod(A[0], modulus);
		return root;
	}


	//TODO: might be marinally faster to do it all in place rather than recursively, but this is
	//pretty easy to understand. It will not be difficult to make it in place later if we
	//deem it to be useful. Here is what it might look like done iteratively
	/*
	 * Everything is done 'in place' on one vector. For example, if I want the third layer,
	 * It is computed as follows (0-indexing the vector) where + computes and - erases
	 * [+1]
	 * [+2, +3, -1]
	 * [+4, +5, -2, +6, +7, -3]
	 * [+0, +1, -4, +2, +3, -5,  +4, +5, -6, +6, +7, -7]
	 * 
	 * The values are captured for modulus and skew products using the above functions to recompute
	 */

	vector<T> parent_layer = compute_remainder_tree_layer<T,U>(A, m, l-1, V, Y, update_forest_vals);

	long N = parent_layer.size();

	vector<T> child_layer (2*N);

	for (long j = 0; j < N; ++j) {

		//left child is parent mod the mtree val
		child_layer[2*j] = parent_layer[j]%compute_product_node<U>(m, flatten(l, 2*j));

		//right child is parent times the atree mod the mtree
		U node_mod = compute_product_node<U>(m, flatten(l, 2*j + 1));

		child_layer[2*j + 1] = parent_layer[j];
		child_layer[2*j + 1].mulmod(compute_product_node_askew<T,U>(A, node_mod, flatten(l, 2*j + 1)), node_mod);

		parent_layer[j] = T(); //Always delete parent after it is not needed
	}

	return child_layer;
}



template <typename T, typename U>
vector<T> remainder_tree_in_forest(const vector<T>& A, const vector<U>& m, long recompute_param, T& V, U& Y, bool update_forest_vals) {
	assert (A.size() == m.size());

	long N = A.size(); //Size of entire segment
	long M = (N>>recompute_param); //Number of leaves in one subtree

	vector<T> C(N); //This will be the final output vector
	long C_offset = 0;

	vector<T> parent_layer; //Declare because it is set inside conditionals

	if (recompute_param == 0) {
		parent_layer = {V};

	} else {
		//Normally Y will be modified when the top is computed in here!
		parent_layer = compute_remainder_tree_layer<T,U>(A, m, recompute_param-1, V, Y, update_forest_vals);
	}

	//Iterates through the subtrees at layer k
	for (long s = 0; s < (1<<recompute_param); ++s) {
		long sub_root = flatten(recompute_param, s); //Note that sub_root is indexed in the context of the entire tree
		long C_parent = (s >> 1);

		vector<U> mTree = subproduct_tree<U> (m, sub_root); //We compute one subtree at a time
		vector<T> ATree = subproduct_tree_askew<T,U> (A, mTree, sub_root);

		//Use the top of mTree to modify Y
		//Will only be executed once if recompute_param == 0
		if ((recompute_param == 0) && update_forest_vals) {
			Y.div(mTree[1]);
		}


		vector<T> CTree(2*M);

		//First value of subtree is a little annoying to get to
		//Note that parent_layer already contains V
		CTree[1] = parent_layer[C_parent];

		if (sub_root%2 == 0) {//aka left child
			CTree[1].mod(mTree[1]);
		}
		else {//aka right child
			CTree[1].mulmod(compute_product_node_askew<T,U>(A, mTree[1], sub_root), mTree[1]);
		}

		for (long i = 1; i < M; ++i) { //only go up to second-lowest layer
			CTree[2*i] = CTree[i]%mTree[2*i];
			//CTree[2*i + 1] = (CTree[i]*ATree[2*i + 1])%mTree[2*i + 1];
		
			CTree[2*i + 1] = CTree[i];
			CTree[2*i + 1].mulmod(ATree[2*i + 1], mTree[2*i + 1]);

			//Always kill the parents when no longer needed
			CTree[i] = T();
			ATree[i] = T();
			mTree[i] = U();
		}

		for (long i = M; i < 2*M; ++i) { //For each leaf in the sub CTree
			C[i-M + C_offset] = CTree[i]; //Slot into output vector
		}
		C_offset += M;
	}

	if (update_forest_vals) {
		V.mulmod(compute_product_node<T>(A, Y, 1), Y);
	}
	return C;
}



template <typename T, typename U>
vector<T> remainder_forest(const vector<T>& A, const vector<U>& m, long forest_param, long recompute_param, T& V, U& Y) {
	//should assert that Y is properly formed (i.e. divisible by each m)
	assert (A.size() == m.size());
	long N = A.size(); //Size of entire segment
	long M = (N>>forest_param); //Number of leaves in one tree

	assert (is_power2(N)); //This alg is not built for non-powers of 2.
	assert ((M>>recompute_param) > 0); //Ensures that the params are not too deep

	//Ignore all the extra stuff if we just want basic remainder tree
	if (forest_param == 0) {
		Y = U();
		return remainder_tree_in_forest<T,U>(A, m, recompute_param, V, Y, false);
	}


	vector<T> C(N); //This will be the final output vector
	long C_offset = 0;

	//Iterates through the subtrees at depth forest_param
	for (long s = 0; s < (1<<forest_param); ++s) {
		long sub_root = flatten(forest_param, s);

		vector<T> A_slice = get_shade<T>(A, sub_root);
		vector<U> m_slice = get_shade<U>(m, sub_root);

		vector<T> C_slice = remainder_tree_in_forest<T,U>(A_slice, m_slice, recompute_param, V, Y, true);

		for (long i = 0; i < M; ++i) { //For each leaf in the sub CTree
			C[i + C_offset] = C_slice[i]; //Slot into output vector, Indexing is slightly different than recompute paradigm
		}

		C_offset += M;
	}

	return C;
}



template <typename T, typename U>
vector<T> remainder_forest(const vector<T>& A, const vector<U>& m, long forest_param, long recompute_param, T V) {
	U Y;
	if (forest_param != 0) {
		Y = compute_product_node<U>(m, 1);
	} else {
		Y = U();
	}

	return remainder_forest<T,U>(A, m, forest_param, recompute_param, V, Y);
}



template <typename T, typename U>
vector<T> remainder_forest(const vector<T>& A, const vector<U>& m) {
	return remainder_forest<T,U>(A, m, 0, 0);
}