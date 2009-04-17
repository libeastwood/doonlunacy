#ifndef DUNE_PYTHON_H
#define DUNE_PYTHON_H

#include <boost/python.hpp>

namespace python = boost::python;

void InitPython();

inline std::string getPyObjectType(python::object obj, int level = 1) {
    return python::extract<std::string>(obj.attr("__class__").attr("__mro__")[level].attr("__name__"));
}

inline bool nonePyObject(python::object obj) {
    return getPyObjectType(obj, 0) == "NoneType";
}


template<typename T>
bool getPyObject(python::object obj, T *dest) {
    if(nonePyObject(obj))
	return false;

    *dest = python::extract<T>(obj);
    return true;
}

template<typename T>
inline std::vector<T> getPyObjectVector(python::object obj)
{
    ssize_t size = python::len(obj);
    std::vector<T> ret(size);
    for(ssize_t i = 0; i < size; i++)
	ret[i] = python::extract<T>(obj[i]);
    return ret;
}

#endif
