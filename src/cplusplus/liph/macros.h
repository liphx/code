#ifndef LIPH_MACROS_H_
#define LIPH_MACROS_H_

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(*(arr)))

#if defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS
#elif defined(__linux__)
#define OS_LINUX
#elif defined(__APPLE__)
#define OS_APPLE
#endif

#if defined(OS_LINUX) || defined(OS_APPLE) || defined(unix) || defined(__unix__) || defined(__unix)
#define OS_UNIX
#endif

#if __cplusplus < 201103L
#define USE_CPP98
#define USE_CPP03
#elif __cplusplus >= 201103L && __cplusplus < 201402L
#define USE_CPP11
#elif __cplusplus >= 201402L && __cplusplus < 201703L
#define USE_CPP14
#elif __cplusplus >= 201703L && __cplusplus < 202002L
#define USE_CPP17
#elif __cplusplus >= 202002L && __cplusplus < 202302L
#define USE_CPP20
#elif __cplusplus >= 202302L
#define USE_CPP23
#endif

#endif  // LIPH_MACROS_H_
