#pragma once

//windows:

//linux :
//

//使用log4cpp
#ifdef USR_LOG4CPP

#include <log4cpp/Category.hh>
#include <log4cpp/CategoryStream.hh>
#include <log4cpp/PropertyConfigurator.hh>
using namespace log4cpp;

LOG4CPP_LOGGER("Root")
#define LOG_R_N(msg) LOG4CPP_NOTICE_S((logger)) << "[" << __FUNCTION__ << ":" << __LINE__ << "] =>: " << msg;
#define LOG_R_D(msg) LOG4CPP_DEBUG_S ((logger)) << "[" << __FUNCTION__ << ":" << __LINE__ << "] =>: " << msg;
#define LOG_R_I(msg) LOG4CPP_INFO_S  ((logger)) << "[" << __FUNCTION__ << ":" << __LINE__ << "] =>: " << msg;
#define LOG_R_W(msg) LOG4CPP_WARN_S  ((logger)) << "[" << __FUNCTION__ << ":" << __LINE__ << "] =>: " << msg;
#define LOG_R_E(msg) LOG4CPP_ERROR_S ((logger)) << "[" << __FUNCTION__ << ":" << __LINE__ << "] =>: " << msg;
#define LOG_R_F(msg) LOG4CPP_FATAL_S ((logger)) << "[" << __FUNCTION__ << ":" << __LINE__ << "] =>: " << msg;

#else

#include <cstdio>
#include <chrono>
#include <iomanip>
#include <thread>
#include <string>
#include <sstream>
#include <ctime>
#if 0
const char* _func_log_print(const char* func, const int line, const char* tag, ...)
{
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss; 
    ss << std::put_time(std::localtime(&t), "[%Y-%m-%d %X]");
    ss << "[" << std::this_thread::get_id() << "]["<< tag << "]";
    ss << "[" << func<<":"<<line<<"] =>: ";
    return ss.str().c_str();
}
#endif
#define _log_printf(func, line, tag, fmt,...) do{\
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());\
    std::stringstream ss; \
    ss << std::put_time(std::localtime(&t), "[%Y-%m-%d %X]");\
    ss << "[" << std::this_thread::get_id() << "]["<< tag << "]";\
    ss << "[" << func<<":"<<line<<"] =>: ";\
	fprintf(stderr, "%s", ss.str().c_str());\
	fprintf(stderr, "" fmt "\n", ##__VA_ARGS__);fflush (stderr);\
}while(0)

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define LOG_R_I(...) _log_printf(__FUNCTION__, __LINE__, "INFO",  __VA_ARGS__)
#define LOG_R_D(...) _log_printf(__FUNCTION__, __LINE__, "DEBUG", __VA_ARGS__)
#define LOG_R_W(...) _log_printf(__FUNCTION__, __LINE__, "WARN",  __VA_ARGS__)
#define LOG_R_E(...) _log_printf(__FUNCTION__, __LINE__, "ERROR", __VA_ARGS__)
#define LOG_R_F(...) _log_printf(__FUNCTION__, __LINE__, "FATAL", __VA_ARGS__)

#define LOG(tag, msg) do{\
	auto t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());\
    stringstream ss; ss << std::put_time(std::localtime(&t), "[%Y-%m-%d %X]");\
    ss << "[" << std::this_thread::get_id() << "]["<<tag<<"]";\
    ss << "[" << __FUNCTION__<<":"<<__LINE__<<"] =>: " << msg;\
	fprintf(stderr, "%s\n", ss.str().c_str());fflush(stderr);\
}while(0)

#define LOG_I(msg) LOG("INFO",  msg)
#define LOG_D(msg) LOG("DEBUG", msg)
#define LOG_W(msg) LOG("WARN",  msg)
#define LOG_E(msg) LOG("ERROR", msg)
#define LOG_F(msg) LOG("FATAL", msg)

#if 0
#define LOG(fmt, ...) do{\
	fprintf(stderr, __FILE__ ":" STR(__LINE__) "] =>: " fmt "\n", ##__VA_ARGS__);\
	fflush(stderr);\
}while(0)

#define LOG_R(fmt, ...) do{\
	auto t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());\
    stringstream ss; ss << std::put_time(std::localtime(&t), "[%Y-%m-%d %X]");\
    ss << "[" << std::this_thread::get_id() << "][DEBUG]";\
    ss << "[" << __FUNCTION__<<":"<<__LINE__<<"] =>: ";\
    fprintf(stderr, "%s", ss.str().c_str());\
	fprintf(stderr, "" fmt "\n", ##__VA_ARGS__);\
	fflush(stderr);\
}while(0)

#define ERR(fmt, ...) \
	fprintf(stderr, "Error: " __FILE__ ":" STR(__LINE__) " " fmt "\n", ##__VA_ARGS__)
#endif

#endif