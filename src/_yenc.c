#include <stdint.h>
#include <Python.h>

static PyObject* decode(PyObject* self, PyObject* args);

static PyMethodDef _YencMethods[] = {
    {
        "decode", decode, METH_VARARGS,
        "decoe yEncoded data"
    },
    {
        NULL, NULL, 0, NULL
    }
};

static struct PyModuleDef _yenc_module =
{
    PyModuleDef_HEAD_INIT,
    "_yenc",
    "decoder",
    -1,
    _YencMethods
};

PyMODINIT_FUNC
PyInit__yenc(void)
{
    return PyModule_Create(&_yenc_module);
}

static PyObject* decode(PyObject* self, PyObject* args)
{
    Py_buffer buffer;
    PyObject* retval;
    char* decoded;
    char ch;
    size_t i = 0, j = 0;

    if (!PyArg_ParseTuple(args, "y*", &buffer)) return NULL;
    if ((decoded = (char*) PyMem_Malloc(buffer.len)) == NULL) return PyErr_NoMemory();

    for (i = 0, ch = ((char*)buffer.buf)[i]; i < buffer.len; ch = ((char*)buffer.buf)[++i])
    {
        if (0x0A == ch || 0x0D == ch) continue;

        if (0x3D == ch)
            ch = ((char*)buffer.buf)[++i] - 106;
        else
            ch -= 42;

        decoded[j++] = ch;
    }

    retval = PyBytes_FromStringAndSize(decoded, j);
    PyMem_Free(decoded);
    return retval;
}
