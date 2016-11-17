#include "pyvix.h"

PyObject *PyVix_Error;

static PyMethodDef PyVix_methods[] = {
    {
        "connect", PyVix_Connect, METH_VARARGS,
        "connect([host, port, user, password, serviceprovider]) -> pyvix.host\n"
        "  Connect to VMware Instance, all parameters are optional"
    },{
        NULL
    }
};

static PyModuleDef PyVIX_module = {
    PyModuleDef_HEAD_INIT,
    "pyvix",
    DOC_MOD,
    -1,
    PyVix_methods
};

PyMODINIT_FUNC PyInit_pyvix() {
    PyObject *mod;
    int ok;

    ok = PyType_Ready(&PyVixHost_Type);
    if(0 > ok) {
        return NULL;
    }

    ok = PyType_Ready(&PyVixVM_Type);
    if(0 > ok) {
        return NULL;
    }

    mod = PyModule_Create(&PyVIX_module);
    if(NULL == mod) {
        return NULL;
    }

    Py_INCREF(&PyVixHost_Type);
    Py_INCREF(&PyVixVM_Type);

    PyModule_AddIntConstant(mod, "API_VERSION",                                     VIX_API_VERSION                                    );
    PyModule_AddIntConstant(mod, "HOSTOPTION_VERIFY_SSL_CERT",                      VIX_HOSTOPTION_VERIFY_SSL_CERT                     );
    PyModule_AddIntConstant(mod, "SERVICEPROVIDER_DEFAULT",                         VIX_SERVICEPROVIDER_DEFAULT                        );
    PyModule_AddIntConstant(mod, "SERVICEPROVIDER_SERVER",                          VIX_SERVICEPROVIDER_VMWARE_SERVER                  );
    PyModule_AddIntConstant(mod, "SERVICEPROVIDER_WORKSTATION",                     VIX_SERVICEPROVIDER_VMWARE_WORKSTATION             );
    PyModule_AddIntConstant(mod, "SERVICEPROVIDER_VMWARE_PLAYER",                   VIX_SERVICEPROVIDER_VMWARE_PLAYER                  );
    PyModule_AddIntConstant(mod, "SERVICEPROVIDER_VI_SERVER",                       VIX_SERVICEPROVIDER_VMWARE_VI_SERVER               );
    PyModule_AddIntConstant(mod, "SERVICEPROVIDER_VMWARE_WORKSTATION_SHARED",       VIX_SERVICEPROVIDER_VMWARE_WORKSTATION_SHARED      );
    PyModule_AddIntConstant(mod, "VMPOWEROP_NORMAL",                                VIX_VMPOWEROP_NORMAL                               );
    PyModule_AddIntConstant(mod, "VMPOWEROP_GUI",                                   VIX_VMPOWEROP_LAUNCH_GUI                           );
    PyModule_AddIntConstant(mod, "VMPOWEROP_FROM_GUEST",                            VIX_VMPOWEROP_FROM_GUEST                           );
    PyModule_AddIntConstant(mod, "POWERSTATE_POWERING_OFF",                         VIX_POWERSTATE_POWERING_OFF                        );
    PyModule_AddIntConstant(mod, "POWERSTATE_POWERED_OFF",                          VIX_POWERSTATE_POWERED_OFF                         );
    PyModule_AddIntConstant(mod, "POWERSTATE_POWERING_ON",                          VIX_POWERSTATE_POWERING_ON                         );
    PyModule_AddIntConstant(mod, "POWERSTATE_POWERED_ON",                           VIX_POWERSTATE_POWERED_ON                          );
    PyModule_AddIntConstant(mod, "POWERSTATE_SUSPENDING",                           VIX_POWERSTATE_SUSPENDING                          );
    PyModule_AddIntConstant(mod, "POWERSTATE_SUSPENDED",                            VIX_POWERSTATE_SUSPENDED                           );
    PyModule_AddIntConstant(mod, "POWERSTATE_TOOLS_RUNNING",                        VIX_POWERSTATE_TOOLS_RUNNING                       );
    PyModule_AddIntConstant(mod, "POWERSTATE_RESETTING",                            VIX_POWERSTATE_RESETTING                           );
    PyModule_AddIntConstant(mod, "POWERSTATE_BLOCKED_ON_MSG",                       VIX_POWERSTATE_BLOCKED_ON_MSG                      );
    PyModule_AddIntConstant(mod, "POWERSTATE_PAUSED",                               VIX_POWERSTATE_PAUSED                              );
    PyModule_AddIntConstant(mod, "POWERSTATE_RESUMING",                             VIX_POWERSTATE_RESUMING                            );
    PyModule_AddIntConstant(mod, "TOOLSSTATE_UNKNOWN",                              VIX_TOOLSSTATE_UNKNOWN                             );
    PyModule_AddIntConstant(mod, "TOOLSSTATE_RUNNING",                              VIX_TOOLSSTATE_RUNNING                             );
    PyModule_AddIntConstant(mod, "TOOLSSTATE_NOT_INSTALLED",                        VIX_TOOLSSTATE_NOT_INSTALLED                       );
    PyModule_AddIntConstant(mod, "LOGIN_IN_GUEST_REQUIRE_INTERACTIVE_ENVIRONMENT",  VIX_LOGIN_IN_GUEST_REQUIRE_INTERACTIVE_ENVIRONMENT );
    PyModule_AddIntConstant(mod, "RUNPROGRAM_RETURN_IMMEDIATELY",                   VIX_RUNPROGRAM_RETURN_IMMEDIATELY                  );
    PyModule_AddIntConstant(mod, "RUNPROGRAM_ACTIVATE_WINDOW",                      VIX_RUNPROGRAM_ACTIVATE_WINDOW                     );
    PyModule_AddIntConstant(mod, "VM_GUEST_VARIABLE",                               VIX_VM_GUEST_VARIABLE                              );
    PyModule_AddIntConstant(mod, "VM_CONFIG_RUNTIME_ONLY",                          VIX_VM_CONFIG_RUNTIME_ONLY                         );
    PyModule_AddIntConstant(mod, "GUEST_ENVIRONMENT_VARIABLE",                      VIX_GUEST_ENVIRONMENT_VARIABLE                     );
    PyModule_AddIntConstant(mod, "SNAPSHOT_REMOVE_CHILDREN",                        VIX_SNAPSHOT_REMOVE_CHILDREN                       );
    PyModule_AddIntConstant(mod, "SNAPSHOT_INCLUDE_MEMORY",                         VIX_SNAPSHOT_INCLUDE_MEMORY                        );
    PyModule_AddIntConstant(mod, "SHAREDFOLDER_WRITE_ACCESS",                       VIX_SHAREDFOLDER_WRITE_ACCESS                      );
    PyModule_AddIntConstant(mod, "CAPTURESCREENFORMAT_PNG",                         VIX_CAPTURESCREENFORMAT_PNG                        );
    PyModule_AddIntConstant(mod, "CAPTURESCREENFORMAT_PNG_NOCOMPRESS",              VIX_CAPTURESCREENFORMAT_PNG_NOCOMPRESS             );
    PyModule_AddIntConstant(mod, "CLONETYPE_FULL",                                  VIX_CLONETYPE_FULL                                 );
    PyModule_AddIntConstant(mod, "CLONETYPE_LINKED",                                VIX_CLONETYPE_LINKED                               );
    PyModule_AddIntConstant(mod, "INSTALLTOOLS_MOUNT_TOOLS_INSTALLER",              VIX_INSTALLTOOLS_MOUNT_TOOLS_INSTALLER             );
    PyModule_AddIntConstant(mod, "INSTALLTOOLS_AUTO_UPGRADE",                       VIX_INSTALLTOOLS_AUTO_UPGRADE                      );
    PyModule_AddIntConstant(mod, "INSTALLTOOLS_RETURN_IMMEDIATELY",                 VIX_INSTALLTOOLS_RETURN_IMMEDIATELY                );
    PyModule_AddIntConstant(mod, "VMDELETE_DISK_FILES",                             VIX_VMDELETE_DISK_FILES                            );

    PyModule_AddIntConstant(mod, "PROPERTY_NONE",                                   VIX_PROPERTY_NONE                                  );
    PyModule_AddIntConstant(mod, "PROPERTY_META_DATA_CONTAINER",                    VIX_PROPERTY_META_DATA_CONTAINER                   );
    PyModule_AddIntConstant(mod, "PROPERTY_HOST_HOSTTYPE",                          VIX_PROPERTY_HOST_HOSTTYPE                         );
    PyModule_AddIntConstant(mod, "PROPERTY_HOST_API_VERSION",                       VIX_PROPERTY_HOST_API_VERSION                      );
    PyModule_AddIntConstant(mod, "PROPERTY_HOST_SOFTWARE_VERSION",                  VIX_PROPERTY_HOST_SOFTWARE_VERSION                 );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_NUM_VCPUS",                           VIX_PROPERTY_VM_NUM_VCPUS                          );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_VMX_PATHNAME",                        VIX_PROPERTY_VM_VMX_PATHNAME                       );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_VMTEAM_PATHNAME",                     VIX_PROPERTY_VM_VMTEAM_PATHNAME                    );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_MEMORY_SIZE",                         VIX_PROPERTY_VM_MEMORY_SIZE                        );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_READ_ONLY",                           VIX_PROPERTY_VM_READ_ONLY                          );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_NAME",                                VIX_PROPERTY_VM_NAME                               );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_GUESTOS",                             VIX_PROPERTY_VM_GUESTOS                            );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_IN_VMTEAM",                           VIX_PROPERTY_VM_IN_VMTEAM                          );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_POWER_STATE",                         VIX_PROPERTY_VM_POWER_STATE                        );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_TOOLS_STATE",                         VIX_PROPERTY_VM_TOOLS_STATE                        );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_IS_RUNNING",                          VIX_PROPERTY_VM_IS_RUNNING                         );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_SUPPORTED_FEATURES",                  VIX_PROPERTY_VM_SUPPORTED_FEATURES                 );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_SSL_ERROR",                           VIX_PROPERTY_VM_SSL_ERROR                          );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_ERROR_CODE",                  VIX_PROPERTY_JOB_RESULT_ERROR_CODE                 );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_VM_IN_GROUP",                 VIX_PROPERTY_JOB_RESULT_VM_IN_GROUP                );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_USER_MESSAGE",                VIX_PROPERTY_JOB_RESULT_USER_MESSAGE               );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_EXIT_CODE",                   VIX_PROPERTY_JOB_RESULT_EXIT_CODE                  );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_COMMAND_OUTPUT",              VIX_PROPERTY_JOB_RESULT_COMMAND_OUTPUT             );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_HANDLE",                      VIX_PROPERTY_JOB_RESULT_HANDLE                     );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_GUEST_OBJECT_EXISTS",         VIX_PROPERTY_JOB_RESULT_GUEST_OBJECT_EXISTS        );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_GUEST_PROGRAM_ELAPSED_TIME",  VIX_PROPERTY_JOB_RESULT_GUEST_PROGRAM_ELAPSED_TIME );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_GUEST_PROGRAM_EXIT_CODE",     VIX_PROPERTY_JOB_RESULT_GUEST_PROGRAM_EXIT_CODE    );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_ITEM_NAME",                   VIX_PROPERTY_JOB_RESULT_ITEM_NAME                  );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_FOUND_ITEM_DESCRIPTION",      VIX_PROPERTY_JOB_RESULT_FOUND_ITEM_DESCRIPTION     );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_SHARED_FOLDER_COUNT",         VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_COUNT        );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_SHARED_FOLDER_HOST",          VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_HOST         );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_SHARED_FOLDER_FLAGS",         VIX_PROPERTY_JOB_RESULT_SHARED_FOLDER_FLAGS        );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_PROCESS_ID",                  VIX_PROPERTY_JOB_RESULT_PROCESS_ID                 );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_PROCESS_OWNER",               VIX_PROPERTY_JOB_RESULT_PROCESS_OWNER              );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_PROCESS_COMMAND",             VIX_PROPERTY_JOB_RESULT_PROCESS_COMMAND            );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_FILE_FLAGS",                  VIX_PROPERTY_JOB_RESULT_FILE_FLAGS                 );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_PROCESS_START_TIME",          VIX_PROPERTY_JOB_RESULT_PROCESS_START_TIME         );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_VM_VARIABLE_STRING",          VIX_PROPERTY_JOB_RESULT_VM_VARIABLE_STRING         );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_PROCESS_BEING_DEBUGGED",      VIX_PROPERTY_JOB_RESULT_PROCESS_BEING_DEBUGGED     );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_SCREEN_IMAGE_SIZE",           VIX_PROPERTY_JOB_RESULT_SCREEN_IMAGE_SIZE          );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_SCREEN_IMAGE_DATA",           VIX_PROPERTY_JOB_RESULT_SCREEN_IMAGE_DATA          );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_FILE_SIZE",                   VIX_PROPERTY_JOB_RESULT_FILE_SIZE                  );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_FILE_MOD_TIME",               VIX_PROPERTY_JOB_RESULT_FILE_MOD_TIME              );
    PyModule_AddIntConstant(mod, "PROPERTY_JOB_RESULT_EXTRA_ERROR_INFO",            VIX_PROPERTY_JOB_RESULT_EXTRA_ERROR_INFO           );
    PyModule_AddIntConstant(mod, "PROPERTY_FOUND_ITEM_LOCATION",                    VIX_PROPERTY_FOUND_ITEM_LOCATION                   );
    PyModule_AddIntConstant(mod, "PROPERTY_SNAPSHOT_DISPLAYNAME",                   VIX_PROPERTY_SNAPSHOT_DISPLAYNAME                  );
    PyModule_AddIntConstant(mod, "PROPERTY_SNAPSHOT_DESCRIPTION",                   VIX_PROPERTY_SNAPSHOT_DESCRIPTION                  );
    PyModule_AddIntConstant(mod, "PROPERTY_SNAPSHOT_POWERSTATE",                    VIX_PROPERTY_SNAPSHOT_POWERSTATE                   );
    PyModule_AddIntConstant(mod, "PROPERTY_GUEST_SHAREDFOLDERS_SHARES_PATH",        VIX_PROPERTY_GUEST_SHAREDFOLDERS_SHARES_PATH       );
    PyModule_AddIntConstant(mod, "PROPERTY_VM_ENCRYPTION_PASSWORD",                 VIX_PROPERTY_VM_ENCRYPTION_PASSWORD                );

    PyVix_Error = PyErr_NewException("pyvix.error", NULL, NULL);
    Py_INCREF(PyVix_Error);
    PyModule_AddObject(mod, "error", PyVix_Error);

    return mod;
}

PyObject * PyVix_Connect(PyObject *self, PyObject *params) {
    PyVixHost *pyvixhost;

    VixHandle job;
    VixHandle handle;
    VixServiceProvider sp;
    VixError error;

    char *host;
    char *user;
    char *passwd;
    int port;
    int ok;

    host = NULL;
    user = NULL;
    passwd = NULL;
    port = 0;
    sp = VIX_SERVICEPROVIDER_DEFAULT;

    ok = PyArg_ParseTuple(params, "|sissi", &host, &port, &user, &passwd, &sp);
    if(FALSE == ok) {
        PyErr_SetString(PyVix_Error, "Bad arguments");
        return NULL;
    }

    job = VixHost_Connect(
        VIX_API_VERSION,
        sp,
        host,
        port,
        user,
        passwd,
        0,
        VIX_INVALID_HANDLE,
        NULL,
        NULL
        );

    Py_BEGIN_ALLOW_THREADS
        error = VixJob_Wait(
            job,
            VIX_PROPERTY_JOB_RESULT_HANDLE,
            &handle,
            VIX_PROPERTY_NONE
            );
    Py_END_ALLOW_THREADS

    Vix_ReleaseHandle(job);

    if(VIX_FAILED(error)) {
        PyErr_SetString(PyVix_Error, "Could not connect");
        return NULL;
    }

    // allocate a pyvixhost object
    pyvixhost = (PyVixHost *)PyObject_CallObject((PyObject *)&PyVixHost_Type, NULL);
    if(NULL == pyvixhost) {
        PyErr_SetString(PyVix_Error, "Could not create pyvix.host object");
        return NULL;
    }

    pyvixhost->host = handle;

    return (PyObject *)pyvixhost;
}
