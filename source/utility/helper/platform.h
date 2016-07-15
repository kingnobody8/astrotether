#pragma once

#ifdef _WIN64 //define something for Windows (64-bit)
#define IS_WIN 1
#define IS_PC 1
#define IS_x64 1
#elif _WIN32 //define something for Windows (32-bit)
#define IS_WIN 1
#define IS_PC 1
#define IS_x32 1
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR // define something for simulator
#define IS_IOS_SIM 1
#define IS_IOS 1
#define IS_MOBILE 1
#define IS_APPLE 1
#define IS_x64 1
#elif TARGET_OS_IPHONE // define something for iphone
#define IS_IOS_DEV 1
#define IS_IOS 1
#define IS_MOBILE 1
#define IS_APPLE 1
#define IS_x64 1
#else
#define TARGET_OS_OSX 1// define something for OSX
#define IS_MAC 1
#define IS_PC 1
#define IS_APPLE 1
#define IS_x64 1
#endif
#elif __linux	// linux - android
#define IS_ANDROID 1
#define IS_MOBILE 1
#elif __unix	// all unices not caught above // Unix
#elif __posix	// POSIX
#endif