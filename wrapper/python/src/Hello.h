#pragma once

#include <Python.h>
// #include <stdbool.h>
// #include <stdlib.h>


typedef struct {
    PyObject_HEAD
} Hello;

static PyObject* hello_new( PyTypeObject* subtype, PyObject* args, PyObject* keywords ) {
    Hello* const self = (Hello*)subtype->tp_alloc( subtype, 0 );

    printf("Hello from hub\n");

    return (PyObject*)self;
}

static void hello_free( Hello* self ) {
    Py_TYPE( self )->tp_free( self );
}


static PyGetSetDef hello_get_set[] = {
    { NULL } /* sentinel */
};

static PyMethodDef hello_methods[] = {
    { NULL } /* sentinel */
};

static PyTypeObject hello_object = {
    PyVarObject_HEAD_INIT( NULL, 0 ).tp_name = "hubPy.Hello",
    .tp_basicsize                            = sizeof( Hello ),
    .tp_dealloc                              = (destructor)hello_free,
    .tp_new                                  = hello_new,
    .tp_getset                               = hello_get_set,
    .tp_methods                              = hello_methods,
};

// #endif
