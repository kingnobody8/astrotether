#pragma once
#include "utility/helper/platform.h"

#include <SDL.h>

#if IS_WIN
#include <Windows.h>
#include <GL/gl.h>
#include "bridge/win/GL/glext.h"
#include "bridge/win/bridge-gl-win.h"
#elif IS_MAC
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include "bridge/mac/bridge-gl-mac.h"
#elif IS_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif IS_ANDROID
#include <EGL/egl.h>
//#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

