#ifndef ZGL_GLBASE_H
#define ZGL_GLBASE_H

#ifdef _MSC_VER
    #define ZGL_API __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
    #define ZGL_API __attribute__((dllimport))
#endif
#endif // ZGL_GLBASE_H
