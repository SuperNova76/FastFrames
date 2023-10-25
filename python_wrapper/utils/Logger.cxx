#include "../../FastFrames/Logger.h"

#include <Python.h>

#include <string>
#include <map>
#include <stdexcept>

using namespace std;

map<int, LoggingLevel>  mapIntToLoggingLevel = {
    {0, LoggingLevel::ERROR},
    {1, LoggingLevel::WARNING},
    {2, LoggingLevel::INFO},
    {3, LoggingLevel::DEBUG}
};

map<LoggingLevel, int>  mapLoggingLevelToInt = {
    {LoggingLevel::ERROR,   0},
    {LoggingLevel::WARNING, 1},
    {LoggingLevel::INFO,    2},
    {LoggingLevel::DEBUG,   3}
};

static PyObject *setLogLevel(PyObject *self, PyObject *args) {
    // Parse Input
    int logLevel;
    if (!PyArg_ParseTuple(args, "p", &logLevel))    {
        return nullptr;
    };

    try {
        Logger &logger = Logger::get();
        logger.setLogLevel(mapIntToLoggingLevel.at(logLevel));
        return Py_BuildValue("");
    }
    catch (const runtime_error &e) {
        const char *error = e.what();
        return Py_BuildValue("s", error);
    }
    return Py_BuildValue("");
}

static PyObject *logMessage(PyObject *self, PyObject *args) {
    // Parse Input
    int logLevel;
    const char *message;
    if (!PyArg_ParseTuple(args, "ps", &message, &logLevel))    {
        return nullptr;
    };

    try {
        LOG_ENUM(mapIntToLoggingLevel.at(logLevel)) << std::string(message);
    }
    catch (const runtime_error &e) {
        const char *error = e.what();
        return Py_BuildValue("s", error);
    }
    return Py_BuildValue("");
}


static PyMethodDef cppLoggerMethods[] = {
        {"set_log_level", setLogLevel, METH_VARARGS, " "},
        {nullptr, nullptr, 0, nullptr}};

static struct PyModuleDef cppLoggerModule = {
        PyModuleDef_HEAD_INIT,
        "cppLogger",
        nullptr,
        -1,
        cppLoggerMethods};

PyMODINIT_FUNC PyInit_cppLogger(void)
{
    return PyModule_Create(&cppLoggerModule);
}
