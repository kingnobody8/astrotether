#pragma once
#include "utility/time/util_time.h"
#include <string>

namespace baka
{
	namespace state
	{
		class IBaseState
		{
		protected:
			std::string m_name;

		public:
			IBaseState() {}
			virtual ~IBaseState(){};

			virtual void Init(){}
			virtual void Exit(){}
			virtual void Update(const util::Time& dt){}

			virtual void Transition(IBaseState* const pNextState){}

			const std::string& GetName() const { return m_name; }
		};
	}
}