#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "starlark.h"
#include "util.h"

typedef struct
{
  PyObject_HEAD starlark_vm_t* vm;
} VirtualMachine;

static void
VirtualMachine_dealloc(VirtualMachine* self)
{
  starlark_vm_free(self->vm);
  self->vm = NULL;
}

static int
VirtualMachine_init(VirtualMachine* self, PyObject* args, PyObject* kwargs)
{
  UNUSED_PARAMETER(kwargs);
  if (!PyArg_ParseTuple(args, "")) {
    return -1;
  }

  self->vm = starlark_vm_new();
  if (!self->vm) {
    PyErr_NoMemory();
    return -1;
  }

  return 0;
}

static PyTypeObject VirtualMachineType = {
  PyVarObject_HEAD_INIT(NULL, 0).tp_name = "starlark.VirtualMachine",
  .tp_doc = "Starlark Virtual Machine",
  .tp_basicsize = sizeof(VirtualMachine),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyType_GenericNew,
  .tp_init = (initproc)VirtualMachine_init,
  .tp_dealloc = (destructor)VirtualMachine_dealloc,
};

static PyMethodDef starlark_methods[] = {
  { NULL, NULL, 0, NULL } /* Sentinel */
};

static struct PyModuleDef starlark_module = {
  PyModuleDef_HEAD_INIT, "starlark", "Starlark module", -1, starlark_methods,
};

PyMODINIT_FUNC
PyInit_starlark(void)
{
  PyObject* m = PyModule_Create(&starlark_module);
  if (PyType_Ready(&VirtualMachineType) < 0)
    return NULL;
  Py_INCREF(&VirtualMachineType);
  if (PyModule_AddObject(m, "VirtualMachine", (PyObject*)&VirtualMachineType) <
      0) {
    Py_DECREF(&VirtualMachineType);
    Py_DECREF(m);
    return NULL;
  }

  return m;
}