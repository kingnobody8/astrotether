#pragma once
#include "utility/helper/platform.h"
#include <string>


#ifdef IS_WIN
		static const std::string getResourcePath(void) { return "../../../"; }
#elif IS_APPLE
		std::string getResourcePath(void);
#elif IS_ANDROID
		static const std::string getResourcePath(void) { return ""; }
#endif
