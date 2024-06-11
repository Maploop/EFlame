#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>

#define YES 1
#define NO 0

#define RELEASE NO

#define LOG_WARN_ENABLED YES
#define LOG_INFO_ENABLED YES
#define LOG_DEBUG_ENABLED YES
#define LOG_TRACE_ENABLED YES

#if RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
    LOG_LEVEL_UH_OH = 6
} log_level;

static std::string CRESOURCE(std::string category, std::string filename) {
    return "resources/" + category + "/" + filename;
}

static const char* CRESOURCE1(std::string category, std::string filename) {
    return CRESOURCE(category, filename).c_str();
}

inline void log_output(log_level level, const char* message, ...) {
    const char* level_strings[7] = { "[FATAL]: ", "[ERROR]: ", "[WARNING]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: ", "" };
    bool is_error = level < LOG_LEVEL_WARN;

    char out_message[32000];
    memset(out_message, 0, sizeof(out_message));

    // Format orignal message
    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, 32000, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[32000];
    sprintf_s(out_message2, "%s%s\n", level_strings[level], out_message);

    // NOTE: This will be platform specific!
    std::cout << out_message2;
}

#define SHFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#define SHEMPTY(message, ...) log_output(LOG_LEVEL_UH_OH, message, ##__VA_ARGS__);

#ifndef SERROR
#define SHERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_INFO_ENABLED == 1
#define SHINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define SHINFO(message, ...)
#endif

#if LOG_WARN_ENABLED == 1
#define SHWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define SHWARN(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define SHDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define SHDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define SHTRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define SHTRACE(message, ...)
#endif

inline void report_assertion_failure(const char* expression, const char* message, const char* file, int line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, in line: %d\n", expression, message, file, line);
}