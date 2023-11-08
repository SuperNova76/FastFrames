/**
 * @file Logger.cxx
 * @brief Source file for python wrapper around Logger class
 *
 */
#include "FastFrames/Logger.h"

#include <Python.h>

#include <string>
#include <map>
#include <stdexcept>

#include <iostream>

using namespace std;

/**
 * @brief map<int, LoggingLevel> translating integer (obtained from python) to LoggingLevel enum in C++
 *
 */
static const map<int, LoggingLevel>  mapIntToLoggingLevel = {
    {0, LoggingLevel::ERROR},
    {1, LoggingLevel::WARNING},
    {2, LoggingLevel::INFO},
    {3, LoggingLevel::DEBUG},
    {4, LoggingLevel::VERBOSE}
};

/**
 * @brief map<LoggingLevel, int> translating LoggingLevel enum in C++ to integer (sent to python)
 *
 */
static const map<LoggingLevel, int>  mapLoggingLevelToInt = {
    {LoggingLevel::ERROR,   0},
    {LoggingLevel::WARNING, 1},
    {LoggingLevel::INFO,    2},
    {LoggingLevel::DEBUG,   3},
    {LoggingLevel::VERBOSE, 4}
};

/**
 * @brief Set the Log Level
 *
 * @param PyObject - self
 * @param PyObject - args - python tuple containing logging level
 * @return PyObject*
 */
static PyObject *setLogLevel([[__maybe_unused__]]PyObject *self, PyObject *args) {
    int logLevelVar;
    if (!PyArg_ParseTuple(args, "i", &logLevelVar))    {
        const std::string errorMessage = "Could not parse input as integer";
        LOG(ERROR) << errorMessage << "\n";
        return Py_BuildValue("s", errorMessage.c_str());
    };

    try {
        Logger &logger = Logger::get();
        logger.setLogLevel(mapIntToLoggingLevel.at(logLevelVar));
    }
    catch (const runtime_error &e) {
        const char *error = e.what();
        return Py_BuildValue("s", error);
    }
    return Py_BuildValue("");
}

/**
 * @brief Get message, logging level and line number from python and log it
 *
 * @param self
 * @param args - python tuple containing logging level, message, file name and line number
 * @return PyObject*
 */
static PyObject *logMessage([[__maybe_unused__]]PyObject *self, PyObject *args) {
    int logLevelVar,lineNumber;
    const char *message, *fileName;
    if (!PyArg_ParseTuple(args, "issi", &logLevelVar, &message, &fileName, &lineNumber))    {
        const std::string errorMessage = "Could not parse inputs";
        return Py_BuildValue("s", errorMessage.c_str());
    };

    try {
        Logger &logger = Logger::get();
        logger(mapIntToLoggingLevel.at(logLevelVar), fileName, lineNumber) << std::string(message);
    }
    catch (const runtime_error &e) {
        const char *error = e.what();
        return Py_BuildValue("s", error);
    }
    return Py_BuildValue("");
}

/**
 * @brief Get logging level
 *
 * @param PyObject*  self - unused
 * @param PyObject*  args - unused
 * @return PyObject* - python tuple with logging level information
 */
static PyObject *logLevel([[__maybe_unused__]]PyObject *self, [[__maybe_unused__]]PyObject *args) {
    int logLevelInt = mapLoggingLevelToInt.at(Logger::get().logLevel());
    return Py_BuildValue("i", logLevelInt);
}

/**
 * @brief Get current logging level
 *
 * @param PyObject* self - unused
 * @param PyObject* args - unused
 * @return PyObject* - python tuple with logging level information
 */
static PyObject *currentLevel([[__maybe_unused__]]PyObject *self, [[__maybe_unused__]]PyObject *args) {
    int currentLevelInt = mapLoggingLevelToInt.at(Logger::get().currentLevel());
    return Py_BuildValue("i", currentLevelInt);
}

/**
 * @brief Methods of the module
 *
 */
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
