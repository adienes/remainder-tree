#ifndef REMAINDERTREE_SRC_ELEMENTS_ELEMENT_H_
#define REMAINDERTREE_SRC_ELEMENTS_ELEMENT_H_

#include <utility>
#include <ostream>


template <typename T>
class Elt
{
	public:
		T t;

		Elt() : t() {}

		template <typename A>
		Elt(A&& a) : t(std::forward<A>(a)) {}
		//~Elt() { delete t; }

		Elt operator * (const Elt& x) const { return Elt(this->t*x.t); }
		Elt operator % (const Elt& x) const { return Elt(this->t%x.t); }
		bool operator == (const Elt& x) { return this->t == x.t; }

		//void operator = (const Elt& x) { this->t = x.t; }
		void operator *= (const Elt& x) { this->t *= x.t; }
		void operator %= (const Elt& x) { this->t %= x.t; }

		//Probably wanna specialize these in most cases so
		//that it is actually done in-place.
		void mul(const Elt& x) { this->t *= x.t; }
		void mod(const Elt& x) { this->t %= x.t; }
		void mulmod(const Elt& x, const Elt& y) { this->t = (this->t*x.t)%y.t; }
};

//I recommend adding a specialized print function to display
template <typename T>
std::ostream &operator<<(std::ostream &os, const Elt<T>& x) { return os<<x.t; }

#include "elt_all.tpp"
#endif // REMAINDERTREE_SRC_ELEMENTS_ELEMENT_H_
