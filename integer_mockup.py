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
def complexity_graph(N, d=400):
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


complexity_graph(27000)
#print ()
#print ()
#print ("True answer: ", dumb_remainder_tree(test_A, test_m)[:100])