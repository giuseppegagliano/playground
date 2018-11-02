#include <unistd.h>
#include <ctime>
#include <cassert>
#include <vector>
#include <iostream>
#include <Python.h>
#include "numpy/arrayobject.h"
using namespace std;

vector<char *>list_to_char_p_vector(PyObject *list_obj);
vector<char *>nparray_to_Cvector(PyObject *list_obj);
void pyobj_print(PyObject *o);
PyObject *compute_lcs(PyObject *self, PyObject* args);
PyObject *test_func(PyObject *self, PyObject* args);
PyObject *parse_input_func(PyObject *self, PyObject* args);
PyObject *return_obj_func(PyObject *self, PyObject* args);
PyMODINIT_FUNC PyInit_glcr();

int import_numpy(){
	import_array(); // @suppress("No return value")
    return 0;
}

static PyMethodDef cpython_module_methods[] = {
//		{ "longest_common_subsequence", (PyCFunction)compute_lcs, METH_VARARGS,
//				"Given a list of strings computes the table of the longest common substrings"
//				"Parameters:"
//				"  -  input_list:			a list of strings, admitted characters are from ASCII 32 to 126."
//				"  -  filename:		the name of the file (without extension) used to save GSA and MAT. If present, the path MUST ALREADY EXISTS"
//				"  -  verbosity:	0 for nothing stored, 1 store GSA and MAT, 2 store GSA and extended MAT"},
				{ "test", (PyCFunction)test_func, METH_NOARGS, "A first no args test function"},
				{ "parse_input_func", (PyCFunction)parse_input_func, METH_VARARGS, "A function with arguments"},
				{ "return_object_func", (PyCFunction)return_obj_func, METH_VARARGS, "A function that returns a dict"},
				{ NULL, NULL, 0, NULL }
};

static PyModuleDef cpython_module = {
		PyModuleDef_HEAD_INIT, "cpython_module",
		"Provides the function to efficiently compute longest common subsequence",
		0, cpython_module_methods
};

PyMODINIT_FUNC PyInit_cpython_module() {
	PyObject *m = PyModule_Create(&cpython_module);
	if (m == NULL) {
		PyErr_SetString(PyExc_RuntimeError, "PyModule_Create() failed");
		return NULL;
	}
	import_numpy();
	return m;
}


PyObject *test_func(PyObject *self, PyObject* args){
	return Py_BuildValue("s", "Hello from a C Function");
}


PyObject *parse_input_func(PyObject *self, PyObject* args){
	char *a_string;
	int an_int;
	PyObject *a_list;
//	Parsing a string (s), an int(i), and a list(O), the pattern becomes "siO"
	if (!PyArg_ParseTuple( args, "siO", &a_string, &an_int, &a_list)) return NULL;
//	PyObject *arr;
//	unsigned int i = 0, j = 0; // defaults, if arguments not given
//	//	cout << "\nSize is " <<  PyArray_SIZE(arr);
//	npy_intp size = PyArray_SIZE(arr);
//	for(npy_intp i = 0; i < size; i++) {
//		cout << "\n" << i << ")";
//		char * s = static_cast<char *>PyArray_GETPTR1((PyArrayObject *)arr, i);
//		cout << "\nstring " << i << " is " << s << "\n";
//		//		data.push_back(s);
//	}
	cout << "the string is: " << a_string << endl;
	cout << "the int is: "  << an_int << endl;
	cout << "the list is: ";
	pyobj_print(a_list);
	cout << endl;
	return Py_BuildValue("s", "It, works! Check the jupyter log...");
}


PyObject *return_obj_func(PyObject *self, PyObject* args){
	PyObject *aList, *aTuple;

	// If the size is known
	int size = 10;
	aList = PyList_New(size);
	for (int j=0; j<size; j++)
		PyList_SetItem(aList, j, Py_BuildValue("i", j));
	// If size is unknown
	aList = PyList_New(0);
	PyList_Append(aList, Py_BuildValue("i", 4));


	aTuple = PyTuple_New(3);
	PyTuple_SET_ITEM(aTuple, (Py_ssize_t)0, Py_BuildValue("i", 0));
	PyTuple_SET_ITEM(aTuple, (Py_ssize_t)1, Py_BuildValue("i", 1));
	PyTuple_SET_ITEM(aTuple, (Py_ssize_t)2, Py_BuildValue("i", 2));


	return Py_BuildValue("{sOsOss}", "aList", aList, "aTuple", aTuple, "aString", "and so on...");
}


//PyObject *compute_lcs(PyObject *self, PyObject* args) {
//	PyObject *input_array, *res;
//	char *input_filename = RESULT_NAME;
//	char *output_filename = RESULT_NAME;
//	TC_reader *tc;
//	GSA *gsa;
//	if(!PyArg_ParseTuple( args, "Ossi", &input_array, &input_filename, &output_filename, &verbosity)) return NULL;
//	if (strlen(input_filename) != 0){
//		tc = new TC_reader();
//		gsa = tc->read_testcase(input_filename);
//	}
//	else{
//		vector<char *> v = nparray_to_Cvector(input_array);
//		sleep(5);
//		tc = new TC_reader();
//		gsa = tc->read_testcase(&v[0], (int)v.size());
//		v.clear();
//		vector<char *>().swap(v);
//	}
//	res = compute_sa(tc, gsa, output_filename);
//	return res;
//}

//
//PyObject *compute_sa(TC_reader *tc, GSA *gsa, char *output_file) {
//	bool detailed_output = true;
//	int result_offset = 2;
//	int* result = NULL;
//	Result_saver* rs = new Result_saver(gsa, detailed_output, tc->buf);
//	int result_size = gsa->num_words-1;
//	LCX* lcx = new LCS_last_visited(gsa, rs);
//	// execute algorithm
//	if (lcx!=NULL) 			lcx->get_lcs();
//	// write result
//	PyObject *ret = NULL;
//	result = rs->get_results();
//	if (result!=NULL)
//		if(verbosity == 1){
//			delete tc;
//			int num_words = gsa->num_words;
//			delete gsa;
//			delete lcx;
//			delete rs->preresults;
//			ret = (PyObject *)rs->write_results_to_memory(num_words);
//			delete[] rs->result_lengths;
//			delete[] rs->results;
//		}
//		else{
//			// write mat file (last visited list)
//			char tmp[100];
//			strcpy(tmp, output_file);
//			strcat(tmp, MAT_EXT);
//			if (remove(tmp) != 0) 		cerr << tmp << " does not exists, it will be created.\n";
//			rs->write_results(tmp, verbosity); // verbosity 2 for MAT 3 for exMAT
//			cout << tmp << " written.\n";
//			// write gsa file (last visited list)
//			strcpy(tmp, output_file);
//			strcat(tmp, GSA_EXT);
//			if (remove(tmp) != 0)		cerr << tmp << " does not exists, it will be created\n";
//			gsa->write(tmp, tc->buf);
//			cout << tmp << " written\n";
//			ret = Py_BuildValue("i", 0);
//			delete tc;
//			delete gsa;
//			delete lcx;
//			free(result);
//			delete rs;
//		}
//	return ret;
//}



//	PyObject *arr;
//	unsigned int i = 0, j = 0; // defaults, if arguments not given
//	if (! PyArg_ParseTuple( args, "O", &arr)) return NULL;
//	//	cout << "\nSize is " <<  PyArray_SIZE(arr);
//	npy_intp size = PyArray_SIZE(arr);
//	for(npy_intp i = 0; i < size; i++) {
//		cout << "\n" << i << ")";
//		char * s = static_cast<char *>PyArray_GETPTR1((PyArrayObject *)arr, i);
//		cout << "\nstring " << i << " is " << s << "\n";
//		//		data.push_back(s);
//	}

	//		double *vector = (double *)calloc(2, sizeof(double));
	//		vector[0] = 1.0;
	//		vector[1] = 2.0;
	//		int nd1 = 1;
	//		npy_intp dims1[1] = {2};
	//		PyObject *ret = PyArray_SimpleNewFromData(nd1, dims1, NPY_DOUBLE, vector);
	//		return Py_BuildValue("O", ret);
	//	npy_intp dims[2] = { (npy_intp)3, (npy_intp)2 };
	//	 PyObject *op, *array;
	//	 PyArray_Descr *descr;
	//	 Py_Initialize();
	//	 op = Py_BuildValue("[(s, s),(s, s),(s, s)]", "integer_1", "i4",
	//			 "integer_2", "i4", "integer3", "i4");
	//	 PyArray_DescrConverter(op, &descr);
	//	 Py_DECREF(op);
	//	 array = PyArray_SimpleNewFromDescr(2, dims, descr);
	//	 void *p = PyArray_GETPTR2(array, (npy_intp) 0, (npy_intp) 0);
	//	 PyArray_SETITEM(array, p, Py_BuildValue("iii", 1, 2, 5));
	//	 p = PyArray_GETPTR2(array, (npy_intp) 0, (npy_intp) 1);
	//	 PyArray_SETITEM(array, p, Py_BuildValue("iii", 3, 4, 5));
	//
	//	 PyObject_Print(array, stdout, 0);



	//	int nd1 = 1;
	//	npy_intp dims1[1] = {2};
	//	double *vector1 = (double *)calloc(2, sizeof(double));
	//	vector1[0] = 1.0;
	//	vector1[1] = 2.0;
	//	int size = 3;
	//
	//	PyObject *list = PyList_New(size);
	//	for (int j=0; j<size; j++)
	//		PyList_SetItem(list, j, PyArray_SimpleNewFromData(nd1, dims1, NPY_DOUBLE, vector1));
	//	PyObject *array1 = PyArray_FromObject(list, NPY_DOUBLE, 1, 2);
	//// i.e.	array([[1., 2.],
	////	           [1., 2.],
	////	           [1., 2.]])
	//
	//
	//	int nd2 = 1;
	//	npy_intp dims2[1] = {3};
	//	double *vector2 = (double *)calloc(3, sizeof(double));
	//	vector2[0] = 3.0;
	//	vector2[1] = 4.0;
	//	vector2[2] = 5.0;
	//	PyObject *array2 = PyArray_SimpleNewFromData(nd2, dims2, NPY_DOUBLE, vector2);
	//// i.e. array([3., 4., 5.])
	//
	//	PyObject *array_of_arrays = PyArray_Transpose((PyArrayObject*)PyArray_Concatenate(
	//			Py_BuildValue("[[O],O]", array2,
	//					PyArray_Transpose((PyArrayObject*)array1, NULL)), 0), NULL);

//	return Py_BuildValue("i", 0);
//}

void pyobj_print(PyObject *o){
	PyObject* repr = PyObject_Repr(o);
	PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
	const char *bytes = PyBytes_AS_STRING(str);
	printf("%s", bytes);
	Py_XDECREF(repr);
	Py_XDECREF(str);
}


vector<char *>list_to_char_p_vector(PyObject *list_obj) {
	vector<char *> data;
	for(Py_ssize_t i = 0; i < PyList_Size(list_obj); i++) {
		PyObject *py_str = PyUnicode_AsUTF8String(PyList_GetItem(list_obj, i));
		char* tmp_str = PyBytes_AsString(py_str);
		char* dup_str = strdup(tmp_str);
		Py_DecRef(py_str);
		data.push_back(dup_str);
	}
	return data;
}


vector <char *> nparray_to_Cvector(PyObject *np_array) {
	vector<char *> data;
	npy_intp size = PyArray_SIZE(np_array);
	for(npy_intp i = 0; i < size; i++) {
		char * s = static_cast<char *>PyArray_GETPTR1((PyArrayObject *)np_array, i);
		data.push_back(s);
	}
	return data;
}
