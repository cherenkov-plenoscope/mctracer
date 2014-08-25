//#include <Python.h> // calling mctracer from python

/*
static PyObject *
spam_system(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return Py_BuildValue("i", sts);
}
*/

 #include <time.h>
 double My_variable = 3.0;
 
 int fact(int n) {
     if (n <= 1) return 1;
     else return n*fact(n-1);
 }
 
 int my_mod(int x, int y) {
     return (x%y);
 }
 	
 char *get_time()
 {
     time_t ltime;
     time(&ltime);
     return ctime(&ltime);
 }