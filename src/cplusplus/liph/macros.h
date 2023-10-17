#ifndef LIPH_MACROS_H_
#define LIPH_MACROS_H_

#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(*(arr)))

#if defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS
#elif defined(__linux__)
#define OS_LINUX
#elif define(__APPLE__)
#define OS_APPLE
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#define OS_UNIX
#endif

#endif  // LIPH_MACROS_H_
