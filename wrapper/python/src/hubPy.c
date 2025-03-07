
#include "Hello.h"
#include "OutputSensor.h"
#include <Python.h>

static struct PyModuleDef config = {
    PyModuleDef_HEAD_INIT,
    "hubPy",
    "",
    -1,
};

bool add_object( PyObject* const module, PyTypeObject* const type_object, const char* const name ) {
    if ( PyType_Ready( type_object ) == 0 ) {
        Py_INCREF( type_object );
        if ( PyModule_AddObject( module, name, (PyObject*)type_object ) == 0 ) { return true; }
        Py_DECREF( type_object );
        return false;
    }
    return false;
}

PyMODINIT_FUNC PyInit_hubPy() {

    PyObject* const module = PyModule_Create( &config );

    if ( module != NULL
         && add_object( module, &hello_object, "Hello" )
         && add_object( module, &outputSensor_object, "OutputSensor" )
         ) {
        return module;
    }
    Py_DECREF( module );
    return NULL;
}
