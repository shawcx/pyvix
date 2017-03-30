#ifndef __PYVIX_H__
#define __PYVIX_H__

#include <Python.h>
#include <structmember.h>

#include <vix.h>

#ifndef VIX_SERVICEPROVIDER_DEFAULT
#define VIX_SERVICEPROVIDER_DEFAULT VIX_SERVICEPROVIDER_VMWARE_WORKSTATION
#endif // VIX_SERVICEPROVIDER_DEFAULT

#define  DOC_MOD  "Python wrapper for VMware VIX API."

extern PyObject *PyVix_Error;

// Init Function

PyMODINIT_FUNC PyInit_pyvix(void);

PyObject * PyVix_Connect( PyObject *self , PyObject *pyoParams );

void VixDiscoveryProc(VixHandle hJob, VixEventType evtType, VixHandle evtInfo, void *data);
PyObject * _PyVix_GetProperty(VixHandle handle, PyObject *prop);


// Python Types

typedef struct {
    PyObject_HEAD
    VixHandle host;
} PyVixHost;

extern PyTypeObject PyVixHost_Type;

typedef struct {
    PyObject_HEAD
    VixHandle vm;
} PyVixVM;

extern PyTypeObject PyVixVM_Type;

#endif // __PYVIX_H__
