#ifndef REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_
#define REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_


bool isPowerTwo(int x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

int flatten(int i , int j) {
	return (1<<i)+j;
}


#endif //REMAINDERTREE_SRC_ALGORITHMS_REMTREEUTILS_H_