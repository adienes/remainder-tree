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


