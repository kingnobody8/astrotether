#pragma once
#include "platform.h"
//#include "sdl.h"

#define __internal_stringify(X) #X
#define __stringify(X) __internal_stringify(X)

// Reminds you what you need to do
#if IS_WIN
#	define __todo() __pragma(message("TODO: " __FILE__ ":(" __stringify(__LINE__) ")"))
#else
#	define __todo()
#endif

// Flag a function as not implemented
#define __not_implemented(ROUTINE) __todo(); 
//SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "NOT IMPLEMENTED "#ROUTINE)