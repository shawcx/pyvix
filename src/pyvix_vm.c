#include "pyvix.h"

int        PyVixVM_Type_init    ( PyVixVM * self, PyObject * args, PyObject * kwds );
void       PyVixVM_Type_dealloc ( PyVixVM * self );

PyObject * PyVixVM_Close        ( PyVixVM * self );
PyObject * PyVixVM_PowerOn      ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_PowerOff     ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_Reset        ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_WaitForTools ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_Login        ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_Logout       ( PyVixVM * self );
PyObject * PyVixVM_Run          ( PyVixVM * self , PyObject * params );
PyObject * PyVixVM_TaskList     ( PyVixVM * self );
PyObject * PyVixVM_Property     ( PyVixVM * self, PyObject * prop );


PyMethodDef PyVixVM_methods[] = {
    {
        "close",        (PyCFunction)PyVixVM_Close,        METH_NOARGS,
        "close() -> None\n"
        "  close handle to VM"
    },{
        "waitfortools", (PyCFunction)PyVixVM_WaitForTools, METH_VARARGS,
        "waitfortools([seconds]) -> None\n"
        "  wait for VM tools to start"
    },{
        "poweron",      (PyCFunction)PyVixVM_PowerOn,      METH_VARARGS,
        "poweron() -> \n"
        "  turn on a VM"
    },{
        "poweroff",     (PyCFunction)PyVixVM_PowerOff,     METH_VARARGS,
        "poweroff() -> \n"
        "  turn off a VM"
    },{
        "reset",        (PyCFunction)PyVixVM_Reset,        METH_VARARGS,
        "reset() -> \n"
        "  reset a VM"
    },{
        "login",        (PyCFunction)PyVixVM_Login,        METH_VARARGS,
        "login() -> \n"
        "  login to a VM"
    },{
        "logout",       (PyCFunction)PyVixVM_Logout,       METH_NOARGS,
        "logout() -> \n"
        "  logout of a VM"
    },{
        "run",          (PyCFunction)PyVixVM_Run,          METH_VARARGS,
        "run() -> \n"
        "  run a task in a VM"
    },{
        "tasklist",     (PyCFunction)PyVixVM_TaskList,     METH_NOARGS,
        "tasklist() -> []\n"
        "  get a tasklist in a VM"
    },{
        "property",   (PyCFunction)PyVixVM_Property,       METH_O,
        "property(property_id) -> value\n"
        "  read a property value"
    },{
        NULL
    }
};

PyTypeObject PyVixVM_Type = {
    PyVarObject_HEAD_INIT(0, 0)
    .tp_name      = "pyvix.vm",
    .tp_basicsize = sizeof(PyVixVM),
    .tp_dealloc   = (destructor)PyVixVM_Type_dealloc,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc       = "PyVix VM Class",
    .tp_methods   = PyVixVM_methods,
    .tp_init      = (initproc)PyVixVM_Type_init,
    .tp_new       = PyType_GenericNew,
};


int PyVixVM_Type_init(PyVixVM *self, PyObject *args, PyObject *kwds) {
    self->vm = VIX_INVALID_HANDLE;
    return 0;
}


void PyVixVM_Type_dealloc(PyVixVM *self) {
    if(VIX_INVALID_HANDLE != self->vm) {
        Vix_ReleaseHandle(self->vm);
        self->vm = VIX_INVALID_HANDLE;
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}


PyObject * PyVixVM_Close(PyVixVM *self) {
    if(VIX_INVALID_HANDLE != self->vm) {
        Vix_ReleaseHandle(self->vm);
        self->vm = VIX_INVALID_HANDLE;
    }

    Py_RETURN_NONE;
}


PyObject * PyVixVM_PowerOn(PyVixVM *self, PyObject *params) {
    VixHandle job;
    VixError error;
    int op;
    int ok;

    op = VIX_VMPOWEROP_NORMAL;

    ok = PyArg_ParseTuple(params, "|i", &op);
    if(FALSE == ok) {
        return NULL;
    }

    job = VixVM_PowerOn(
        self->vm,
        op,
        VIX_INVALID_HANDLE,
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


PyObject * PyVixVM_PowerOff(PyVixVM *self, PyObject *params) {
    VixHandle job;
    VixError error;
    int op;
    int ok;

    op = VIX_VMPOWEROP_NORMAL;

    ok = PyArg_ParseTuple(params, "|i", &op);
    if(FALSE == ok) {
        return NULL;
    }

    job = VixVM_PowerOff(
        self->vm,
        op,
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


PyObject * PyVixVM_Reset(PyVixVM *self, PyObject *params) {
    VixHandle job;
    VixError error;
    int op;
    int ok;

    op = VIX_VMPOWEROP_NORMAL;

    ok = PyArg_ParseTuple(params, "|i", &op);
    if(FALSE == ok) {
        return NULL;
    }

    job = VixVM_Reset(
        self->vm,
        op,
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


PyObject * PyVixVM_WaitForTools(PyVixVM *self, PyObject *params) {
    VixHandle job;
    VixError error;
    int timeout;
    int ok;

    timeout = 5;

    ok = PyArg_ParseTuple(params, "|i", &timeout);
    if(FALSE == ok) {
        return NULL;
    }

    job = VixVM_WaitForToolsInGuest(
        self->vm,
        timeout,
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


PyObject * PyVixVM_Login(PyVixVM *self, PyObject *params) {
    VixHandle job;
    VixError error;
    char *user;
    char *passwd;
    int options;
    int ok;

    user = NULL;
    passwd = NULL;
    options = 0;

    ok = PyArg_ParseTuple(params, "ss|i", &user, &passwd, &options);
    if(FALSE == ok) {
        return NULL;
    }

    job = VixVM_LoginInGuest(
        self->vm,
        user,
        passwd,
        options,
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


PyObject * PyVixVM_Logout(PyVixVM *self) {
    VixHandle job;
    VixError error;

    job = VixVM_LogoutFromGuest(
        self->vm,
        NULL,
        NULL
        );

    error = VixJob_Wait(job, VIX_PROPERTY_NONE);
    if(VIX_FAILED(error)) {
        PyErr_SetString(PyVix_Error, Vix_GetErrorText(error, NULL));
        return NULL;
    }

    Py_RETURN_NONE;
}


PyObject * PyVixVM_Run(PyVixVM *self, PyObject *params) {
    VixHandle job;
    VixRunProgramOptions opts;
    VixError error;
    char *app;
    char *args;
    int wait;
    int ok;

    app = NULL;
    args = NULL;
    wait = TRUE;

    ok = PyArg_ParseTuple(params, "s|si", &app, &args, &wait);
    if(FALSE == ok) {
        return NULL;
    }

    opts = 0;
    if(FALSE == wait) {
        opts |= VIX_RUNPROGRAM_RETURN_IMMEDIATELY;
    }

    // Run the target program.
    job = VixVM_RunProgramInGuest(
        self->vm,
        app,
        args,
        opts,
        VIX_INVALID_HANDLE,
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


PyObject * PyVixVM_TaskList(PyVixVM *self) {
    PyObject *list;
    PyObject *item;
    //PyObject *value;
    VixHandle job;
    VixError error;
    int len;
    int idx;

    job = VixVM_ListProcessesInGuest(
        self->vm,
        0,
        NULL,
        NULL
        );

    Py_BEGIN_ALLOW_THREADS
        error = VixJob_Wait(job, VIX_PROPERTY_NONE);
    Py_END_ALLOW_THREADS

    if(VIX_FAILED(error)) {
        PyErr_SetString(PyVix_Error, Vix_GetErrorText(error, NULL));
        return NULL;
    }


    len = VixJob_GetNumProperties(job, VIX_PROPERTY_JOB_RESULT_PROCESS_ID);
    list = PyList_New(len);
    for(idx = 0; idx < len; ++idx) {
        char *procname;
        char *cmdline;
        uint64 pid;

        error = VixJob_GetNthProperties(
            job,
            idx,
            VIX_PROPERTY_JOB_RESULT_ITEM_NAME,
            &procname,
            VIX_PROPERTY_JOB_RESULT_PROCESS_COMMAND,
            &cmdline,
            VIX_PROPERTY_JOB_RESULT_PROCESS_ID,
            &pid,
            VIX_PROPERTY_NONE
            );

        //printf("process #%d %d %s\n", idx, pid, cmdline);

        item = PyTuple_New(3);
        PyTuple_SetItem(item, 0, PyLong_FromSize_t(pid));
        PyTuple_SetItem(item, 1, PyUnicode_FromString(procname));
        PyTuple_SetItem(item, 2, PyUnicode_FromString(cmdline));
        PyList_SET_ITEM(list, idx, item);

        Vix_FreeBuffer(procname);
        Vix_FreeBuffer(cmdline);
    }

    Vix_ReleaseHandle(job);

    return list;
}


PyObject * PyVixVM_Property(PyVixVM *self, PyObject *prop) {
    return _PyVix_GetProperty(self->vm, prop);
}
