#include <NTL/ZZ.h>

template<typename A>
class Elt <NTL::ZZ, A>
{
	public:
		NTL::ZZ t;

		Elt(A&& a) : t(std::forward<A>(a)) {}
		//~elt() { delete t; }

		Elt operator * (const Elt& x) const { return Elt(this->t*x.t); }
		Elt operator % (const Elt& x) const { return Elt(this->t%x.t); }

		void operator *= (const Elt& x) { this->mul(x); }
		void operator %= (const Elt& x) { this->mod(x); }
		
		void mul(const Elt& x) { NTL::mul(this->t, this->t, x.t); }
		void mod(const Elt& x) { NTL::div(this->t, this->t, x.t); }
		void mulmod(const Elt& x, const Elt& y) { NTL::MulMod(this->t, this->t, x.t, y.t); }
};