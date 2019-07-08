6/10/19
Implemented remainder tree for general integer inputs and mods with tree data structure and recursive structure. Runtime graph plotted.

6/11/19
More runtime graphs created and compared. Studied from the literature. Experimented with different recursive tree structures.

6/12/19
Got confused about recursion tree; tried to set up Linux or Visual Studio or any way to run C++ code.

6/13/19
Created a C++ implementation. No optimizations done yet whatsoever. First priority tomorrow is to set it up with GMP. Then, do the log-factor speedup, also I need to implement RemainderForest. Notation still sucks, sadly.

6/14/19
Tested out a possible memory optimization of the tree by only calculating what mods are necessary. Edgar advised against it due to memory concerns.

6/15/19
Added some python runtime graphs from Daniel's newly set up Linux machine

6/17/19
Preemptively changed int types to ZZ types from NTL, but things don't work yet. Need to learn more about NTL documentation to fix everything.

6/18/19
Tried to fix yesterday's NTL converted code but failed, converted another piece of code to use ZZ but can't figure out how to get the NTL library to link correctly.

6/19/19
Tried to start the general object-oriented remaidner tree program. Made various minor edits like indexing and file organization.

6/20/19
Completed a C++ remainder tree implementation with ZZ.

6/21/19
Made a little more progress on the general implementation. Also modified the working version of C++ implemetation to test for Wilson's Theorem, and measured the runtime of the algorithm.

6/22/19
Devised a way to calculate the necessary product of A values in a tree half the size of the original A product tree. However, such a method probably is overkill because we can just deallocate the pointers to the ZZ in the original A product tree to get the same effect. (Maybe it makes memory less fragmented?)

6/23/19
Finished part of Costa's space-saving optimization of the remainer tree algorithm.

6/24/19
Did some file clean-up.

6/25/19
Finished implementation of Costa's space-saving optimization, and added code for checking the runtime. Discovered that the time the program takes to run is much much too slower than what it should be.

6/26/19
Added a makefile and did some cleanup

6/27/19
Daniel: started learning vim

6/28/19
Starting work on Sutherland's space-saving optimization. Found the fix of the slowness in Costa's algorithm, which was accidentally passing by value instead of by reference (always pass big things by reference!!). Updated the README.md to log all the progress we've done so far.

6/29/19
Completed Sutherland's space-saving optimization, and created graphs of how much time and space it takes when choosing different layers to form the subtrees in. This algorithm looks very promising based on the numbers; it outperforms Costa's optimization that we implemented.

6/30/19
Fixed Sutherland's algorithm to work for all moduli (there was a int overflow bug) and added a graph of runtime for finding Wilson Primes. Found that using my computer, calculating all primes from 1 to 2^41 will take 26069 hours, so there is still optimizations to be done.

7/1/19
Implemented Sutherland's algorithm for Wolstenholme primes, and found that it will take around 5.3 hours to calculate to the current record of 10^9, so there is a lot of hope in this regard. Also added graph of calculation up to 2^24.

7/7/19
Andy: I've worked the past week or so through a bunch of painful c++ things. I tried multiple different architectures (for example using abstract base class rather than templates), but I ended up going with the templates because I would prefer compile-time polymorphism rather than runtime. I learned about covariance, rvalue references, templates&specialization,  etc. doing this. I was going to make it more abstract/OO at the C++ level (such as making a binary tree module and whatnot), but I actually think it makes sense to have the C++ subroutines to be as bare bones as possible for maximum speed, and do OO stuff in Python.
I also added an interface for Python. I considered tools such as CFFI (seems to have higher runtime overhead, and only works for C), cppyy (could be good, especially with PyPy, but I don't want to impose much requirements on the user---also might be more memory-hungry), and Cython (would rather the dichotomy of C++ vs Python and not some weird hybrid). I settled on pybind11 which is basically a stripped down Boost library. I added the Python interface because I think a lot of things will be easy to test out and implement in Python rather than fighting C++ all the time.
For example, at the top layers of the tree, the time to call functions is massively dominated by the time to store and compute on those huge numbers, so it's fine to do 'high levels' in Python and call the C++ arithmetic. As the numbers get smaller it will make sense to call a C++ subroutine on the entire subtree (once it can fit in memory) to alleviate the function call overhead.
I have made it relatively easy to add modules and types, so I would like to put in NTT and various matrix / polynomial libraries (perhaps GMP or FLINT). I still haven't totally figured out how to link the libraries into pybind11 sadly, but this is my next priority.
Things to do: Integrate Daniel's algs into my interface. Implement a seive / way to generate the A and m. Implement BSGS as a way to verify answers. Implement the sqrt(N) time 'factorial' calc on the linear recurrence.