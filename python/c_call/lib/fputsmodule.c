// https://realpython.com/build-python-c-extension-module/
#include <Python.h>

static PyObject *method_fputs(PyObject *self, PyObject *args)
{

   char *str, *filename = NULL;
   int bytes_copied = -1;

   /* Parse arguments */
   if (!PyArg_ParseTuple(args, "ss", &str, &filename))
   {
      return NULL;
   }

   FILE *fp = fopen(filename, "w");
   bytes_copied = fputs(str, fp);
   fclose(fp);

   fprintf(stderr, "[C-LOG stderr] Calling c fputs\n");
   fprintf(stdout, "[C-LOG stdout] Calling c fputs\n");
   return PyLong_FromLong(bytes_copied);
}

static PyMethodDef FputsMethods[] = {
   {"fputs", method_fputs, METH_VARARGS, "Python interface for fputs C library function"},
   {NULL, NULL, 0, NULL}
};

static struct PyModuleDef fputsmodule = {
   PyModuleDef_HEAD_INIT,
   "fputs",
   "Python interface for the fputs C library function",
   -1,
   FputsMethods
};

PyMODINIT_FUNC PyInit_fputs(void) {
   return PyModule_Create(&fputsmodule);
}
