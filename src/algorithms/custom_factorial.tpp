// included by rem_factorial.hpp (in case we want to change the file location)

#include <NTL/ZZ.h>
#include "../elements/Element.hpp"

template<>
Elt<NTL::ZZ> calculate_initial(long n, Elt<NTL::ZZ> m, std::function<Elt<NTL::ZZ> (long)> get_A, const PolyMatrix& formula = {}){

}
