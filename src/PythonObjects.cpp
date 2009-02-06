#include <boost/python.hpp>

#include "Point.h"
#include "Rect.h"

static void * PointFloatExtract(PyObject *intermediate)
{
    float x, y;
    if(!PyArg_ParseTuple(intermediate, "ff", &x, &y))
	python::throw_error_already_set();

    PointFloat tmp(x, y);
    void *ret = &tmp;

    return ret;
}

static void * UPointExtract(PyObject *intermediate)
{
    Uint16 x, y;
    if(!PyArg_ParseTuple(intermediate, "HH", &x, &y))
	python::throw_error_already_set();

    UPoint tmp(x, y);
    void *ret = &tmp;

    return ret;
}

static void * SPointExtract(PyObject *intermediate)
{
    Sint16 x, y;
    if(!PyArg_ParseTuple(intermediate, "hh", &x, &y))
	python::throw_error_already_set();

    SPoint tmp(x, y);
    void *ret = &tmp;

    return ret;
}

static void * RectExtract(PyObject *intermediate)
{
    Sint16 x, y;
    Uint16 w, h;
    if(!PyArg_ParseTuple(intermediate, "hhHH", &x, &y, &w, &h))
	python::throw_error_already_set();

    Rect tmp(x, y, w, h);
    void *ret = &tmp;

    return ret;
}


void InitPython()
{
    setenv("PYTHONPATH", "python", 1);

    Py_Initialize();

    python::converter::registry::insert(&SPointExtract,python::type_id<SPoint>(),&python::converter::wrap_pytype<&PyTuple_Type>::get_pytype);
    python::converter::registry::insert(&UPointExtract,python::type_id<UPoint>(),&python::converter::wrap_pytype<&PyTuple_Type>::get_pytype);
    python::converter::registry::insert(&PointFloatExtract,python::type_id<PointFloat>(),&python::converter::wrap_pytype<&PyTuple_Type>::get_pytype);
    python::converter::registry::insert(&RectExtract,python::type_id<Rect>(),&python::converter::wrap_pytype<&PyTuple_Type>::get_pytype);
}