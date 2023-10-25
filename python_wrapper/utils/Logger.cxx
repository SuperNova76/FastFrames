#include "../../FastFrames/Logger.h"

#include <Python.h>

#include <string>
#include <map>
#include <stdexcept>

#include <iostream>

using namespace std;

static const map<int, LoggingLevel>  mapIntToLoggingLevel = {
    {0, LoggingLevel::ERROR},
    {1, LoggingLevel::WARNING},
    {2, LoggingLevel::INFO},
    {3, LoggingLevel::DEBUG}
};

static const map<LoggingLevel, int>  mapLoggingLevelToInt = {
    {LoggingLevel::ERROR,   0},
    {LoggingLevel::WARNING, 1},
    {LoggingLevel::INFO,    2},
    {LoggingLevel::DEBUG,   3}
};

static PyObject *setLogLevel([[__maybe_unused__]]PyObject *self, PyObject *args) {
    int logLevel;
    if (!PyArg_ParseTuple(args, "i", &logLevel))    {
        const std::string errorMessage = "Could not parse input as integer";
        return Py_BuildValue("s", errorMessage.c_str());
    };

    try {
        Logger &logger = Logger::get();
        logger.setLogLevel(mapIntToLoggingLevel.at(logLevel));
    }
    catch (const runtime_error &e) {
        const char *error = e.what();
        return Py_BuildValue("s", error);
    }
    return Py_BuildValue("");
}

static PyObject *logMessage([[__maybe_unused__]]PyObject *self, PyObject *args) {
    int logLevel;
    const char *message;
    if (!PyArg_ParseTuple(args, "is", &logLevel, &message))    {
        const std::string errorMessage = "Could not parse input as integer and string";
        return Py_BuildValue("s", errorMessage.c_str());
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

static PyObject *logLevel([[__maybe_unused__]]PyObject *self, [[__maybe_unused__]]PyObject *args) {
    int logLevelInt = mapLoggingLevelToInt.at(Logger::get().logLevel());
    return Py_BuildValue("i", logLevelInt);
}

static PyObject *currentLevel([[__maybe_unused__]]PyObject *self, [[__maybe_unused__]]PyObject *args) {
    int currentLevelInt = mapLoggingLevelToInt.at(Logger::get().currentLevel());
    return Py_BuildValue("i", currentLevelInt);
}

static PyMethodDef cppLoggerMethods[] = {
        {"set_log_level",   setLogLevel,    METH_VARARGS, " "},
        {"log_message",     logMessage,     METH_VARARGS, " "},
        {"log_level",       logLevel,       METH_VARARGS, " "},
        {"current_level",   currentLevel,   METH_VARARGS, " "},
        {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef cppLoggerModule = {
        PyModuleDef_HEAD_INIT,
        "cppLogger",
        nullptr,
        -1,
        cppLoggerMethods,
        nullptr,
        nullptr,
        nullptr,
        nullptr
};

PyMODINIT_FUNC PyInit_cppLogger(void)
{
    return PyModule_Create(&cppLoggerModule);
}
