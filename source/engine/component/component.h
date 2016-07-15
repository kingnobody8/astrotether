#pragma once
#include "utility/helper/func.h"
#include "utility/types/types.h"
#include "utility/resource/json.h"
#include "utility/types/bit_flag.h"
#include "utility/math/util_math.h"
#include "utility/helper/enums.h"
#include <map>
#include <list>
#include <vector>

namespace engine
{
	namespace component
	{
		class IPart;
		typedef std::list<IPart*> PartList;
		typedef std::list<IPart*>::iterator PartIter;
		typedef std::map<int, PartList> PartPriorityMap;
		typedef std::map<int, PartList>::iterator PartPriorityIter;

		typedef ulonglong						PartTypeKey;
		typedef std::function<IPart*(void)>		PartFunctor;

		class CObject;
		class CGroup;
		typedef std::vector<CObject*> ObjectList;
		typedef std::vector<CObject*>::iterator ObjectIter;

		enum EFlag
		{
			ACTIVE = 0x01,
			SELF_VISIBLE = 0x02,	//part doesn't use this
			PARENT_VISIBLE = 0x04,	//part doesn't use this
			STARTED = 0x08,
			STOPPED = 0x10,
			DIRTY = 0x20,
			NUKED = 0x40
		};
	}
}