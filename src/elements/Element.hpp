#ifndef REMAINDERTREE_SRC_ELEMENTS_ELEMENT_H_
#define REMAINDERTREE_SRC_ELEMENTS_ELEMENT_H_

#include <utility>
#include <ostream>
#include <type_traits>

//Got this from the following blog:
//http://ericniebler.com/2013/08/07/universal-references-and-the-copy-constructo/
//Deals with copy constructor and perfect forwarding problem
template<typename A, typename B>
using disable_if_same_or_derived =
    typename std::enable_if<
        !std::is_base_of<A,typename
             std::remove_reference<B>::type
        >::value
    >::type;


template <typename T>
class Elt
{
	public:
		T t;

		Elt() : t() {}

		template <typename A, typename X = disable_if_same_or_derived<Elt, A>>
		Elt(A&& a) : t(std::forward<A>(a)) {}
		//~Elt() { delete t; }

		Elt operator * (const Elt& x) const { return Elt(this->t*x.t); }
		Elt operator % (const Elt& x) const { return Elt(this->t%x.t); }
		bool operator == (const Elt& x) const { return this->t == x.t; }

		//void operator = (const Elt& x) { this->t = x.t; }
		void operator *= (const Elt& x) { this->mul(x); }
		void operator %= (const Elt& x) { this->mod(x); }
		void operator /= (const Elt& x) { this->div(x); }

		//Probably wanna specialize these in most cases so
		//that it is actually done in-place.
		void mul(const Elt& x) { this->t *= x.t; }
		void mod(const Elt& x) { this->t %= x.t; }
		void mulmod(const Elt& x, const Elt& y) { this->t = (this->t*x.t)%y.t; }

		//This must be overwritten! Not every type is guaranteed to have nice division
		void div(const Elt& x) { this->t /= x.t; }
};

//I recommend adding a specialized print function to display
template <typename T>
std::ostream &operator<<(std::ostream &os, const Elt<T>& x) { return os<<x.t; }

#include "elt_all.tpp"
#endif // REMAINDERTREE_SRC_ELEMENTS_ELEMENT_H_
