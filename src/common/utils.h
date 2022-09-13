#pragma once

#ifdef NDEBUG

#define Assert(expression) ((void)0)
#define Check(expression) expression

#else

#include <assert.h>

#define Assert(expression) assert(expression)
#define Check(expression) assert(expression)

#endif // NDEBUG
