#include <NTL/ZZ.h>

template <>
class Elt <NTL::ZZ>
{
	public:
		NTL::ZZ t;

		template <typename A>
		Elt(A&& a) : t(std::forward<A>(a)) {}
		//~elt() { delete t; }

		Elt operator * (const Elt& x) const { return Elt(this->t*x.t); }
		Elt operator % (const Elt& x) const { return Elt(this->t%x.t); }
		bool operator == (const Elt& x) const { return this->t == x.t; }

		void operator *= (const Elt& x) { this->mul(x); }
		void operator %= (const Elt& x) { this->mod(x); }
		void operator /= (const Elt& x) { this->div(x); }

		void mul(const Elt& x) { NTL::mul(this->t, this->t, x.t); }
		void mod(const Elt& x) { NTL::QuickRem(this->t, x.t); }
		void div(const Elt& x) { NTL::div(this->t, this->t, x.t); }
		void mulmod(const Elt& x, const Elt& y) { NTL::MulMod(this->t, this->t, x.t, y.t); }
};