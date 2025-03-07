#pragma once

#include <Python.h>
#include <native/sensor/native_OutputSensor.hpp>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    PyObject_HEAD HUB_OUTPUT_SENSOR* outputSensor;
    HUB_ACQUISITION* acq;
    HUB_DOF6* dof6;
    HUB_CLOCK* start;
    HUB_CLOCK* end;
} OutputSensor;

static PyObject* outputSensor_new( PyTypeObject* subtype, PyObject* args, PyObject* keywords ) {
    OutputSensor* const self = (OutputSensor*)subtype->tp_alloc( subtype, 0 );
    char* sensorName;
    char* streamName;
    PyArg_ParseTuple( args, "ss", &sensorName, &streamName );

    self->outputSensor = createOutputSensorDof6( sensorName, streamName );
    self->acq          = outputSensorGetAcq( self->outputSensor );
    self->dof6         = acquisition_getDof6( self->acq );
    self->start        = acquisition_getStart( self->acq );
    self->end          = acquisition_getEnd( self->acq );

    return (PyObject*)self;
}

static void outputSensor_free( OutputSensor* self ) {
    freeAcquisition( self->acq );
    freeOutputSensor( self->outputSensor );
    Py_TYPE( self )->tp_free( self );
}

static int outputSensor_set_start( OutputSensor* self, PyObject* value, void* closure ) {
    HUB_CLOCK a  = PyLong_AS_LONG( value );
    *self->start = a;
    return 0;
}
static int outputSensor_set_end( OutputSensor* self, PyObject* value, void* closure ) {
    HUB_CLOCK a = PyLong_AS_LONG( value );
    *self->end  = a;
    return 0;
}
static int outputSensor_set_x( OutputSensor* self, PyObject* value, void* closure ) {
    float a       = PyFloat_AsDouble( value );
    self->dof6->x = a;
    return 0;
}
static int outputSensor_set_y( OutputSensor* self, PyObject* value, void* closure ) {
    float a       = PyFloat_AsDouble( value );
    self->dof6->y = a;
    return 0;
}
static int outputSensor_set_z( OutputSensor* self, PyObject* value, void* closure ) {
    float a       = PyFloat_AsDouble( value );
    self->dof6->z = a;
    return 0;
}

static int outputSensor_set_w0( OutputSensor* self, PyObject* value, void* closure ) {
    float a        = PyFloat_AsDouble( value );
    self->dof6->w0 = a;
    return 0;
}
static int outputSensor_set_w1( OutputSensor* self, PyObject* value, void* closure ) {
    float a        = PyFloat_AsDouble( value );
    self->dof6->w1 = a;
    return 0;
}
static int outputSensor_set_w2( OutputSensor* self, PyObject* value, void* closure ) {
    float a        = PyFloat_AsDouble( value );
    self->dof6->w2 = a;
    return 0;
}
static int outputSensor_set_w3( OutputSensor* self, PyObject* value, void* closure ) {
    float a        = PyFloat_AsDouble( value );
    self->dof6->w3 = a;
    return 0;
}

static PyObject* outputSensor_newAcq( OutputSensor* self, PyObject* args ) {
    outputSensorSendAcq( self->outputSensor, self->acq );
    Py_INCREF( Py_None );
    return Py_None;
}

static PyGetSetDef outputSensor_get_set[] = {
    { "start", NULL, (setter)outputSensor_set_start, "", NULL },
    { "end", NULL, (setter)outputSensor_set_end, "", NULL },
    { "x", NULL, (setter)outputSensor_set_x, "", NULL },
    { "y", NULL, (setter)outputSensor_set_y, "", NULL },
    { "z", NULL, (setter)outputSensor_set_z, "", NULL },
    { "w0", NULL, (setter)outputSensor_set_w0, "", NULL },
    { "w1", NULL, (setter)outputSensor_set_w1, "", NULL },
    { "w2", NULL, (setter)outputSensor_set_w2, "", NULL },
    { "w3", NULL, (setter)outputSensor_set_w3, "", NULL },
    { NULL } /* sentinel */
};

static PyMethodDef outputSensor_methods[] = {
    { "newAcq", (PyCFunction)outputSensor_newAcq, METH_NOARGS, "" },
    { NULL } /* sentinel */
};

static PyTypeObject outputSensor_object = {
    PyVarObject_HEAD_INIT( NULL, 0 ).tp_name = "hubPy.OutputSensor",
    .tp_basicsize                            = sizeof( OutputSensor ),
    .tp_dealloc                              = (destructor)outputSensor_free,
    .tp_new                                  = outputSensor_new,
    .tp_getset                               = outputSensor_get_set,
    .tp_methods                              = outputSensor_methods,
};

// #endif
