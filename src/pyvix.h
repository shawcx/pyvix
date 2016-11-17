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

// ****************************************************************************
// Init Function
// ****************************************************************************

PyMODINIT_FUNC PyInit_pyvix(void);

PyObject * PyVix_Connect( PyObject *self , PyObject *pyoParams );

void VixDiscoveryProc(VixHandle hJob, VixEventType evtType, VixHandle evtInfo, void *data);

// ****************************************************************************
// Host Type
// ****************************************************************************

typedef struct {
    PyObject_HEAD
    VixHandle host;
} PyVixHost;

extern PyTypeObject PyVixHost_Type;

PyObject * PyVixHost_Type_new     ( PyTypeObject * type, PyObject * args, PyObject * kwds );
int        PyVixHost_Type_init    ( PyVixHost * self, PyObject * args, PyObject * kwds );
void       PyVixHost_Type_dealloc ( PyVixHost * self );

PyObject * PyVixHost_Disconnect ( PyVixHost * self );
PyObject * PyVixHost_Running    ( PyVixHost * self );
PyObject * PyVixHost_Registered ( PyVixHost * self );
PyObject * PyVixHost_Register   ( PyVixHost * self , PyObject * path );
PyObject * PyVixHost_Unregister ( PyVixHost * self , PyObject * path );
PyObject * PyVixHost_Open       ( PyVixHost * self , PyObject * path );

// ****************************************************************************
// VM Type
// ****************************************************************************

typedef struct {
    PyObject_HEAD
    VixHandle vm;
} PyVixVM;

extern PyTypeObject PyVixVM_Type;

PyObject * PyVixVM_Type_new     ( PyTypeObject * type, PyObject * args, PyObject * kwds );
int        PyVixVM_Type_init    ( PyVixVM * self, PyObject * args, PyObject * kwds );
void       PyVixVM_Type_dealloc ( PyVixVM * self );

PyObject * PyVixVM_Close        ( PyVixVM * self );
PyObject * PyVixVM_PowerOn      ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_PowerOff     ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_Reset        ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_WaitForTools ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_IsRunning    ( PyVixVM * self );
PyObject * PyVixVM_Login        ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_Logout       ( PyVixVM * self );
PyObject * PyVixVM_Run          ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_TaskList     ( PyVixVM * self );

#endif // __PYVIX_H__
