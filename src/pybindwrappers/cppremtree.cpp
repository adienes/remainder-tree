#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "Element.hpp"
#include <string>
#include <memory>
#include <NTL/ZZ.h>

Elt<NTL::ZZ> sq(int a)
{
	Elt<NTL::ZZ> x(a);
	return x*x;
}

int add(int a, int b)
{
	return a+b;
}

namespace py = pybind11;

// template <typename T>
// static void declare_element(py::module &m, const std::string& suffix)
// {	
// 	using Class = Elt<T>;
// 	using PyClass = py::class_<Class, std::unique_ptr<Class> >;
// 	PyClass cls (m, ("Elt_"+suffix).c_str());

// 	cls.def(py::init<A&&>());

// 	cls.def(py::self * py::self);
// 	cls.def(py::self % py::self);
// 	cls.def(py::self *= py::self);
// 	cls.def(py::self %= py::self);
// 	cls.def(py::self == py::self);

// 	cls.def("mul", &Class::mul);
// 	cls.def("mod", &Class::mod);
// 	cls.def("mulmod", &Class::mulmod);

// 	cls.def("__repr__", [](const Class &a) -> std::string { return std::to_string(a.t); });
// }


PYBIND11_MODULE(cppremtree, m)
{

	m.doc() = "pybind11 example plugin";

	m.def("add", &add, "A function which adds two numbers");

	//declare_element<NTL::ZZ>(m, "ZZ");
}