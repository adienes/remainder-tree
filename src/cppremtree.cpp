#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "Element.hpp"
#include <string>
#include <memory>

int add(int a, int b)
{
	//Elt<int,int> x {5};
	return a+b;
}


namespace py = pybind11;

template <typename T, typename A>
static void declare_element(py::module &m, const std::string& suffix)
{	
	using Class = Elt<T, A>;
	using PyClass = py::class_<Class, std::unique_ptr<Class> >;
	PyClass cls (m, ("Elt_"+suffix).c_str());

	cls.def(py::init<A&&>());

	cls.def(py::self * py::self);
	cls.def(py::self % py::self);
	cls.def(py::self *= py::self);
	cls.def(py::self %= py::self);

	cls.def("mul", &Class::mul);
	cls.def("mod", &Class::mod);
	cls.def("mulmod", &Class::mulmod);

	cls.def("__repr__", [](const Class &a) -> std::string { return std::to_string(a.t); });
}


PYBIND11_MODULE(cppremtree, m)
{

	m.doc() = "pybind11 example plugin";

	m.def("add", &add, "A function which adds two numbers");

	declare_element<int, int>(m, "int");
}