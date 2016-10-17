#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
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
			virtual void Update(const sf::Time& dt){}
			virtual const std::string DebugRender(sf::RenderWindow* pRenWin) { return ""; }

			virtual void Transition(IBaseState* const pNextState){}

			const std::string& GetName() const { return m_name; }
		};
	}
}