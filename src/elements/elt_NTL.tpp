#include <NTL/ZZ.h>


template<>
void Elt<NTL::ZZ>::mul(const Elt& x) { NTL::mul(this->t, this->t, x.t); }

template<>
void Elt<NTL::ZZ>::mod(const Elt& x) { NTL::QuickRem(this->t, x.t); }

template<>
void Elt<NTL::ZZ>::div(const Elt& x) { NTL::div(this->t, this->t, x.t); }

template<>
void Elt<NTL::ZZ>::mulmod(const Elt& x, const Elt& y) { NTL::MulMod(this->t, this->t, x.t, y.t); }