#include <Python.h>
#include <Windows.h>
#include <msiquery.h>

// Stolen from _msi.c from CPython
typedef struct
{
	PyObject_HEAD
	MSIHANDLE h;
} msiobj;

PyObject *MSIError;

static PyObject*
msierror(int status)
{
	int code;
	char buf[2000];
	char *res = buf;
	DWORD size = sizeof(buf);
	MSIHANDLE err = MsiGetLastErrorRecord();

	if (err == 0) {
		switch(status) {
		case ERROR_ACCESS_DENIED:
			PyErr_SetString(MSIError, "access denied");
			return NULL;
		case ERROR_FUNCTION_FAILED:
			PyErr_SetString(MSIError, "function failed");
			return NULL;
		case ERROR_INVALID_DATA:
			PyErr_SetString(MSIError, "invalid data");
			return NULL;
		case ERROR_INVALID_HANDLE:
			PyErr_SetString(MSIError, "invalid handle");
			return NULL;
		case ERROR_INVALID_STATE:
			PyErr_SetString(MSIError, "invalid state");
			return NULL;
		case ERROR_INVALID_PARAMETER:
			PyErr_SetString(MSIError, "invalid parameter");
			return NULL;
		default:
			PyErr_Format(MSIError, "unknown error %x", status);
			return NULL;
		}
	}

	code = MsiRecordGetInteger(err, 1); /* XXX code */
	if (MsiFormatRecord(0, err, res, &size) == ERROR_MORE_DATA) {
		res = malloc(size+1);
		MsiFormatRecord(0, err, res, &size);
		res[size]='\0';
	}
	MsiCloseHandle(err);
	PyErr_SetString(MSIError, res);
	if (res != buf)
		free(res);
	return NULL;
}

// End theft

static PyObject *msi_read_stream_record_readstream(PyObject *self, PyObject *args)
{
	msiobj *record;
	unsigned int field;
	if (!PyArg_ParseTuple(args, "OI", &record, &field))
	{
		return NULL;
	}

	unsigned int data_size = MsiRecordDataSize(record->h, field);
	if (data_size == 0) {
		PyErr_SetString(MSIError, "could not get data size of record");
		return NULL;
	}

	long idx = 0;
	PyObject *ret_buf = PyList_New(data_size);
	if (ret_buf == NULL) {
		return NULL;
	}

	unsigned char buf[1];
	DWORD bytes_written = 1;
	while (1)
	{
		UINT status;
		if ((status = MsiRecordReadStream(record->h, field, buf, &bytes_written)) != ERROR_SUCCESS) {
			return msierror(status);
		}

		if (bytes_written == 1) {
			PyList_SetItem(ret_buf, idx++, PyLong_FromLong((long) buf[0]));
		} else {
			break;
		}
	}

	return PyBytes_FromObject(ret_buf);
}

static PyMethodDef msi_read_stream_methods[] =
{
	{"ReadStream", msi_read_stream_record_readstream, METH_VARARGS, "ReadStream"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef msi_read_stream =
{
	PyModuleDef_HEAD_INIT,
	"msi_read_stream",
	"Extension methods to the msilib class to allow reading streams",
	-1,
	msi_read_stream_methods
};

PyMODINIT_FUNC PyInit__msi_read_stream()
{
	PyObject *m = PyModule_Create(&msi_read_stream);

	MSIError = PyErr_NewException("_msi_read_stream.MSIError", NULL, NULL);

	if (!MSIError)
	{
		return NULL;
	}

	PyModule_AddObject(m, "MSIError", MSIError);

	return m;
}
