#ifndef REMAINDERTREE_SRC_ELEMENT_H_
#define REMAINDERTREE_SRC_ELEMENT_H_

class Element
{
	public:
		virtual ~Element() = default;
	protected:
		Element() = default;
		
		virtual void mul(const Element*) = 0;
		virtual void mod(const Element*) = 0;

		virtual Element* operator * (const Element* const) const = 0;
		virtual Element* operator % (const Element* const) const = 0;

		virtual void print(void) const = 0;
};

#endif // REMAINDERTREE_SRC_ELEMENT_H_