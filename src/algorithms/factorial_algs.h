// included by rem_factorial.tpp

#include <functional>
#include <vector>

#include <NTL/ZZ_pX.h>
#include <NTL/matrix.h>

using std::vector;

using NTL::ZZ_pX;
using NTL::ZZ_p;
using NTL::ZZ;
using NTL::Mat;

template<typename T, typename U>
T naive_factorial(long n, U& m, std::function<vector<T> (long, long)>& get_A);

ZZ_p poly_factorial(long n, ZZ& m, ZZ_pX& poly);

Mat<ZZ_p> matrix_factorial(long n, ZZ& m, Mat<ZZ_pX>& matrix);


