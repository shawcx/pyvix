#include "pyvix.h"

int        PyVixHost_Type_init    ( PyVixHost * self, PyObject * args, PyObject * kwds );
void       PyVixHost_Type_dealloc ( PyVixHost * self );

PyObject * PyVixHost_Disconnect ( PyVixHost * self );
PyObject * PyVixHost_Running    ( PyVixHost * self );
PyObject * PyVixHost_Registered ( PyVixHost * self );
PyObject * PyVixHost_Register   ( PyVixHost * self , PyObject * path );
PyObject * PyVixHost_Unregister ( PyVixHost * self , PyObject * path );
PyObject * PyVixHost_Open       ( PyVixHost * self , PyObject * path );
PyObject * PyVixHost_Property   ( PyVixHost * self , PyObject * prop );


PyMethodDef PyVixHost_methods[] = {
    {
        "disconnect", (PyCFunction)PyVixHost_Disconnect, METH_NOARGS,
        "disconnect() -> None\n"
        "  disconnect from host"
    },{
        "running",    (PyCFunction)PyVixHost_Running,    METH_NOARGS,
        "running() -> []\n"
        "  return list of running VMs"
    },{
        "registered", (PyCFunction)PyVixHost_Registered, METH_NOARGS,
        "registered() -> []\n"
        "  return list of registered VMs"
    },{
        "register",   (PyCFunction)PyVixHost_Register,   METH_O,
        "register(vmxpath) -> None\n"
        "  register a VM with VMware Server"
    },{
        "unregister", (PyCFunction)PyVixHost_Unregister, METH_O,
        "unregister(vmxpath) -> None\n"
        "  unregister a VM with VMware Server"
    },{
        "open",       (PyCFunction)PyVixHost_Open,       METH_O,
        "open(vmxpath) -> pyvix.vm\n"
        "  open a vmx and return a VM object"
    },{
        "property",   (PyCFunction)PyVixHost_Property,   METH_O,
        "property(property_id) -> value\n"
        "  read a property value"
    },{
        NULL
    }
};


PyTypeObject PyVixHost_Type = {
    PyVarObject_HEAD_INIT(0, 0)
    .tp_name      = "pyvix.host",
    .tp_basicsize = sizeof(PyVixHost),
    .tp_dealloc   = (destructor)PyVixHost_Type_dealloc,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc       = "PyVix Host Class",
    .tp_methods   = PyVixHost_methods,
    .tp_init      = (initproc)PyVixHost_Type_init,
    .tp_new       = PyType_GenericNew
};


int PyVixHost_Type_init(PyVixHost *self, PyObject *args, PyObject *kwds) {
    self->host = VIX_INVALID_HANDLE;
    return 0;
}


void PyVixHost_Type_dealloc(PyVixHost *self) {
    if(VIX_INVALID_HANDLE != self->host) {
        VixHost_Disconnect(self->host);
        self->host = VIX_INVALID_HANDLE;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}


PyObject * PyVixHost_Disconnect(PyVixHost *self) {
    VixHost_Disconnect(self->host);
    self->host = VIX_INVALID_HANDLE;

    Py_RETURN_NONE;
}


PyObject * PyVixHost_Running(PyVixHost *self) {
    PyObject *list;
    VixHandle job;
    VixError error;

    list = PyList_New(0);

    if(VIX_INVALID_HANDLE == self->host) {
        return list;
    }

    Py_BEGIN_ALLOW_THREADS
        job = VixHost_FindItems(
            self->host,
            VIX_FIND_RUNNING_VMS,
            VIX_INVALID_HANDLE,
            -1,
            VixDiscoveryProc,
            list
            );

        error = VixJob_Wait(job, VIX_PROPERTY_NONE);

        Vix_ReleaseHandle(job);
    Py_END_ALLOW_THREADS

    if(VIX_OK != error) {
        Py_DECREF(list);
        PyErr_SetString(PyVix_Error, Vix_GetErrorText(error, NULL));
        return NULL;
    }


    return list;
}


PyObject * PyVixHost_Registered(PyVixHost *self) {
    PyObject *list;
    VixHandle job;
    VixError error;

    list = PyList_New(0);

    if(VIX_INVALID_HANDLE == self->host) {
        return list;
    }

    Py_BEGIN_ALLOW_THREADS
        job = VixHost_FindItems(
            self->host,
            VIX_FIND_REGISTERED_VMS,
            VIX_INVALID_HANDLE,
            -1,
            VixDiscoveryProc,
            list
            );

        error = VixJob_Wait(job, VIX_PROPERTY_NONE);

        Vix_ReleaseHandle(job);
    Py_END_ALLOW_THREADS

    if(VIX_OK != error) {
        Py_DECREF(list);
        PyErr_SetString(PyVix_Error, Vix_GetErrorText(error, NULL));
        return NULL;
    }

    return list;
}


PyObject * PyVixHost_Register(PyVixHost *self, PyObject *path) {
    VixHandle job;
    VixError error;
    const char *vmx = NULL;
    int ok;

    ok = PyUnicode_Check(path);
    if(FALSE == ok) {
        PyErr_SetString(PyExc_TypeError, "Value must be a string.");
        return NULL;
    }

    vmx = PyUnicode_AsUTF8(path);

    Py_BEGIN_ALLOW_THREADS
        job = VixHost_RegisterVM(
            self->host,
            vmx,
            NULL,
            NULL
            );

        error = VixJob_Wait(job, VIX_PROPERTY_NONE);

        Vix_ReleaseHandle(job);
    Py_END_ALLOW_THREADS

    if(VIX_FAILED(error)) {
        PyErr_SetString(PyVix_Error, Vix_GetErrorText(error, NULL));
        return NULL;
    }

    Py_RETURN_NONE;
}


PyObject * PyVixHost_Unregister(PyVixHost *self, PyObject *path) {
    VixHandle job;
    VixError error;
    const char *vmx = NULL;
    int ok;

    ok = PyUnicode_Check(path);
    if(FALSE == ok) {
        PyErr_SetString(PyExc_TypeError, "Value must be a string.");
        return NULL;
    }

    vmx = PyUnicode_AsUTF8(path);

    job = VixHost_UnregisterVM(
        self->host,
        vmx,
        NULL,
        NULL
        );

    Py_BEGIN_ALLOW_THREADS
        error = VixJob_Wait(job, VIX_PROPERTY_NONE);
    Py_END_ALLOW_THREADS

    Vix_ReleaseHandle(job);

    if(VIX_FAILED(error)) {
        PyErr_SetString(PyVix_Error, Vix_GetErrorText(error, NULL));
        return NULL;
    }

    Py_RETURN_NONE;
}


PyObject * PyVixHost_Open(PyVixHost *self, PyObject *path) {
    PyVixVM *pyvixvm;
    VixHandle vm;
    VixHandle job;
    VixError error;
    const char *vmx = NULL;
    int ok;

    if(VIX_INVALID_HANDLE == self->host) {
        Py_RETURN_NONE;
    }

    ok = PyUnicode_Check(path);
    if(FALSE == ok) {
        PyErr_SetString(PyExc_TypeError, "Value must be a string.");
        return NULL;
    }

    vmx = PyUnicode_AsUTF8(path);

    Py_BEGIN_ALLOW_THREADS
        job = VixHost_OpenVM(
            self->host,
            vmx,
            VIX_VMOPEN_NORMAL,
            VIX_INVALID_HANDLE,
            NULL,
            NULL
            );

        error = VixJob_Wait(job, VIX_PROPERTY_JOB_RESULT_HANDLE, &vm, VIX_PROPERTY_NONE);

        Vix_ReleaseHandle(job);
    Py_END_ALLOW_THREADS

    if(VIX_FAILED(error)) {
        PyErr_SetString(PyVix_Error, Vix_GetErrorText(error, NULL));
        return NULL;
    }

    // allocate a pyvixvm object
    pyvixvm = (PyVixVM *)PyObject_CallObject((PyObject *)&PyVixVM_Type, NULL);
    if(NULL == pyvixvm) {
        PyErr_SetString(PyVix_Error, "Could not create pyvix.vm object");
        return NULL;
    }

    pyvixvm->vm = vm;

    return (PyObject *)pyvixvm;
}


PyObject * PyVixHost_Property(PyVixHost *self, PyObject *prop) {
    return _PyVix_GetProperty(self->host, prop);
}
