import random
import time
import matplotlib.pyplot as plt

"""
We have a sequence A0 A1 A2 ... of integers, and also a sequence m0 m1 m2 ... of moduli.
This returns a vector containing C_i where C0 = A0 mod m0, C1 = A0*A1 mod m1, ...
"""

def remainder_tree(A, m):
	N = len(A)
	assert N == len(m)

	if N == 0: #shouldn't really ever occur but this handles weird cases
		return []

	leftmost = 1 << N.bit_length()
	# amazingly, total length of array (empty [0]) is 2^(lgN+1) + 2(N - 2^lgN) = 2*N (or via invariants)
	ATree = [0]*2*N
	mTree = [0]*2*N
	CTree = [0]*2*N

	# fill in the base values of A, m in the tree (the initial m[i] and the initial A[i])
	for i in range(leftmost, 2*N):
		ATree[i] = A[i - leftmost]
		mTree[i] = m[i - leftmost]
	for i in range(N, leftmost):
		ATree[i] = A[i + N-leftmost]
		mTree[i] = m[i + N-leftmost]

	# fill in the rest of the values in tree (the products of m[i] and A[i])
	for i in range(N-1, 0, -1):
		if not i & (i+1) == 0: # don't calculate A values for rightmost branch of tree (indices 2^j-1) since we don't need them
			ATree[i] = ATree[2*i]*ATree[2*i+1]
		mTree[i] = mTree[2*i]*mTree[2*i+1]

	# computation of the modulos starting at the root with initial value 1
	CTree[1] = 1
	for i in range(1, N):
		CTree[2*i] = CTree[i] % mTree[2*i]
		CTree[2*i+1] = (CTree[i] * ATree[2*i]) % mTree[2*i+1]

	# initialize and set output
	C = [0]*N
	for i in range(leftmost, 2*N):
		C[i - leftmost] = (CTree[i] * ATree[i]) % mTree[i]
	for i in range(N, leftmost):
		C[i + N-leftmost] = (CTree[i] * ATree[i]) % mTree[i]

	return C


def dumb_remainder_tree(A,m):
	N = len(A)
	assert N == len(m)

	C = [A[0]]
	for i in range(1, N):
		C.append(C[i-1]*A[i])

	for i in range(N):
		C[i] %= m[i]

	return C


#up to N numbers and d datapoints
def complexity_graph(N, d=1000):
	x = []
	y = []

	interval = N//d
	B = 0
	while B < N:
		start = time.time()

		A = [random.randint(1, B) for i in range(B)]
		m = [random.randint(1, B) for i in range(B)] 
		q = remainder_tree(A, m)

		end = time.time()

		x.append(B)
		y.append(end-start)

		B += interval

	plt.plot(x,y)
	plt.show()


test_A = [random.randint(1,300) for i in range(80000)]
test_m = [random.randint(1,300) for i in range(80000)]

#test_A = [i for i in range(100)]
print ("Fast answer: ", remainder_tree(test_A, test_m)[:100])


complexity_graph(30000)


#print ()
#print ()
#print ("True answer: ", dumb_remainder_tree(test_A, test_m)[:100])