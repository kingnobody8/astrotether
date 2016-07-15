#pragma once
#include "object.h"
//#include "part.h"

namespace engine
{
	namespace component
	{
		template<typename type>
		type* CObject::FindPart() const
		{
			return static_cast<type*>(this->FindPart(type::Type));
		}
	}
}