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

	#base case for recursion
	if N == 1:
		return [A[0]%m[0]]


	#otherwise, pair the first element with Id, and then every successive second element
	#the C_i are recovered by either taking a mod, or multiplying then taking a mod
	#it will 'complete' the binary tree so rounds up to nearest power of 2


	p_m = []
	for i in range(0,N,2):
		try:
			p_m.append(m[i]*m[i+1])

		except IndexError:
			p_m.append(m[i]) #only occurs at the end when N is odd

	p_A = [A[0]] #careful of mutability issue here
	for i in range(1, N-1, 2):
		p_A.append(A[i]*A[i+1])


	p_C = remainder_tree(p_A, p_m)

	#print ("Traversing up to r(", p_A,",", p_m, ")")
	#print ("Found the answer as ", p_C)


	C = []
	for i in range(N):
		parent = p_C[i//2]

		if i%2 == 0:
			C.append(parent%m[i])

		if i%2 == 1:
			C.append((parent*A[i])%m[i])

		#print ("Now returning: ", C)
		#print ()
	return C

def remainder_tree_v2(A, m): #tries a slightly different structure of pairing? might do fewer overall ops
	N = len(A)
	assert N == len(m)

	if N == 0: #shouldn't really ever occur but this handles weird cases
		return []

	#base case for recursion
	if N == 1:
		return [A[0]%m[0]]


	#otherwise, return the C_0, then pair every 2 elements
	#the C_i are recovered by either taking a mod, or multiplying then taking a mod
	#it will 'complete' the binary tree so rounds up to nearest power of 2


	p_m = []
	for i in range(1,N,2):
		try:
			p_m.append(m[i]*m[i+1])

		except IndexError:
			p_m.append(m[i]) #only occurs at the end when N is odd

	p_A = [] #careful of mutability issue here
	for i in range(0, N-1, 2):
		p_A.append(A[i]*A[i+1])


	p_C = remainder_tree_v2(p_A, p_m)

	#print ("Traversing up to r(", p_A,",", p_m, ")")
	#print ("Found the answer as ", p_C)


	C = [A[0]%m[0]]
	for i in range(1,N):
		parent = p_C[(i-1)//2]

		if i%2 == 1:
			C.append(parent%m[i])

		if i%2 == 0:
			C.append((parent*A[i])%m[i])

		#print ("Now returning: ", C)
		#print ()
	return C

def array_remainder_tree(A,m):
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

	return CTree


def array_rt_v1(A,m):
	pass




def dumb_remainder_tree(A,m):
	N = len(A)
	assert N == len(m)

	C = [A[0]]
	for i in range(1, N):
		C.append(C[i-1]*A[i])

	for i in range(N):
		C[i] %= m[i]

	return C

def weird_remainder_tree(A,m):
	N = len(A)
	assert N == len(m)

	p = 1
	for i in m:
		p *= i

	C = [A[0]]
	for i in range(1,N):
		C.append((C[i-1]*A[i])%p)

	for i in range(N):
		C[i] %= m[i]

	return C


random.seed(14)

colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
colorIndex = 0
print("assiened")

#up to N numbers and d datapoints
def complexity_graph(N, d, rt_alg):
	global colorIndex
	global colors

	x = [0]
	y = [0]

	interval = N//d
	B = interval
	while B <= N:
		#print (B)

		A = [random.randint(1, B) for i in range(B)]
		m = [random.randint(1, B) for i in range(B)] 
		start = time.time()
		q = rt_alg(A, m)

		end = time.time()

		x.append(B)
		y.append(end-start)

		B += interval

	plt.plot(x,y, colors[colorIndex])
	colorIndex = colorIndex + 1

#test_A = [random.randint(1,300) for i in range(80000)]
#test_m = [random.randint(1,300) for i in range(80000)]

#test_A = [1,2,3,4,5,6,7,8,9,10]
#test_m = [17,19,31,101,103,11,13,29,23,37]

#print ("Fast answer: ", remainder_tree_v2(test_A, test_m)[:100])

print("Starting test.")
print("remainder_tree")
complexity_graph(30000, 1000, remainder_tree)

print("remainder_tree_v2")
complexity_graph(30000, 1000, remainder_tree_v2)

print("array_remainder_tree")
complexity_graph(30000, 1000, array_remainder_tree)

plt.show()

#print ()
#print ()
#print ("True answer: ", dumb_remainder_tree(test_A, test_m)[:100])