#pragma once
#include "util_time.h"

namespace util
{
	struct Tick
	{
		u64			m_frame;
		util::Time	m_elapsedTime;
		util::Time	m_prevTime;
		util::Time	m_currTime;

		Tick() : m_frame(0) {}

		const util::Time Delta()
		{
			return m_currTime - m_prevTime;
		}

		void Restart(void)
		{
			m_frame = 0;
			m_elapsedTime = 0;
			m_prevTime = 0;
			m_currTime = Time::GetTimeSinceEpoch();
		}

		void Signal(void)
		{
			++m_frame;
			m_prevTime = m_currTime;
			m_currTime = Time::GetTimeSinceEpoch();
			m_elapsedTime += Delta();
		}
	};
}
