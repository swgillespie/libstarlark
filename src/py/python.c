#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "starlark.h"
#include "util.h"

// =============================================================================
// starlark.VirtualMachine
// =============================================================================

typedef struct
{
  PyObject_HEAD starlark_vm_t* vm;
} VirtualMachine;

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

static void
VirtualMachine_dealloc(VirtualMachine* self)
{
  if (self->vm != NULL) {
    starlark_vm_free(self->vm);
  }
  self->vm = NULL;
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

// =============================================================================
// starlark.Thread
// =============================================================================

typedef struct
{
  PyObject_HEAD starlark_thread_t* thread;
  PyObject* vm;
} Thread;

static int
Thread_init(Thread* self, PyObject* args, PyObject* kwargs)
{
  UNUSED_PARAMETER(kwargs);
  if (!PyArg_ParseTuple(args, "O!", &VirtualMachineType, &self->vm)) {
    return -1;
  }

  Py_INCREF(self->vm);
  starlark_vm_t* vm = ((VirtualMachine*)self->vm)->vm;
  starlark_thread_t* thread = starlark_thread_new(vm);
  if (!thread) {
    PyErr_NoMemory();
    return -1;
  }

  self->thread = thread;
  return 0;
}

static void
Thread_dealloc(Thread* self)
{
  Py_XDECREF(self->vm);
  if (self->thread != NULL) {
    starlark_thread_free(self->thread);
  }
  self->thread = NULL;
}

static PyObject*
Thread_slot_count(Thread* self, PyObject* Py_UNUSED(args))
{
  int slots = starlark_thread_get_slot_count(self->thread);
  return PyLong_FromLong(slots);
}

static PyMethodDef Thread_methods[] = {
  { "slot_count", (PyCFunction)Thread_slot_count, METH_NOARGS },
  { NULL },
};

static PyTypeObject ThreadType = {
  PyVarObject_HEAD_INIT(NULL, 0).tp_name = "starlark.Thread",
  .tp_doc = "Starlark Thread",
  .tp_basicsize = sizeof(Thread),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyType_GenericNew,
  .tp_init = (initproc)Thread_init,
  .tp_dealloc = (destructor)Thread_dealloc,
  .tp_methods = Thread_methods,
};

// =============================================================================
// starlark module setup
// =============================================================================

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

  if (PyType_Ready(&ThreadType) < 0)
    return NULL;
  Py_INCREF(&ThreadType);
  if (PyModule_AddObject(m, "Thread", (PyObject*)&ThreadType) < 0) {
    Py_DECREF(&ThreadType);
    Py_DECREF(m);
    return NULL;
  }

  return m;
}