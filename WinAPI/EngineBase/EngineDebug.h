#pragma once
#include <Windows.h>
#include <assert.h>

#define MSGASSERT(VALUE) MessageBoxA(nullptr, VALUE, "¿¡·¯", MB_OK); assert(false);
