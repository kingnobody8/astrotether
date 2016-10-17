#pragma once
#include "platform.h"

// STD
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <string.h>

#if IS_WIN
#include <ctime>
#elif IS_IOS
#include <sys/time.h>
#endif

#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <functional>
#include <sstream>
#include <memory>
#include <iomanip>
#include <fstream>