#ifndef REMAINDERTREE_SRC_BINARYTREE_H_
#define REMAINDERTREE_SRC_BINARYTREE_H_

#include <array>

template<typename T>
class BinaryTree
{
	/*
		WARNING: right now this is indexed using simply ints.
		If an array (or tree) of size greater than 2^31 is desired,
		there may be overflow / memory issues.

		TODO: I would like some visuals / printing
	*/
	protected:
		int depth;
		std::array<T> tree;

	public:
		BinaryTree(int);
		BinaryTree(std::array<T>&);
		BinaryTree(lambda(int));
		~BinaryTree();

		int getDepth() const { return depth; }

		const T& getNode(int, int) const;
		const T& getNode(int) const;
		const T& operator [](int) const;

		const T& left(int) const;
		const T& right(int) const;
		const T& parent(int) const;

		const T& left(int, int) const;
		const T& right(int, int) const;
		const T& parent(int, int) const;


		const T& root() const;

		void setNode(int, int, T&);
		void setNode(int, T&);

		BinaryTree copy() const;

		void map(lambda(int, int));
		void map(lambda(int));

		void clear();



};

#include "BinaryTree.tpp"
#endif // REMAINDERTREE_SRC_BINARYTREE_H_