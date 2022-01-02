#pragma once

#ifdef BINPACKING_EXPORTS
#define BINPACKING_API __declspec(dllexport)
#else
#define BINPACKING_API __declspec(dllimport)
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
