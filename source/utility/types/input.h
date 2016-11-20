#pragma once
namespace util
{
	class _Key
	{
	private:
		double	m_dTimer;		//Current state timer
		bool	m_bPrev;		//Previous frame's key state
		bool	m_bCurr;		//Current frame's key state
		bool	m_bNext;		//Future frame's key state (Used with Flush)
		bool	m_bSet;			//Has next been called this frame (Used for event based input)

	public:
		_Key(void)
			: m_dTimer(0)
			, m_bPrev(false)
			, m_bCurr(false)
			, m_bNext(false)
			, m_bSet(false)
		{
		}

		//Set the state the key will be in when it is flushed
		inline void		Next(const bool bDown)
		{
			this->m_bNext = bDown;
			this->m_bSet = true;
		}
		//Flush all the data and update the timer
		inline void		Flush(const double dDelta = 0.0f)
		{
			//Curr -> Prev
			this->m_bPrev = this->m_bCurr;
			//Next -> Curr
			if (this->m_bSet)
				this->m_bCurr = this->m_bNext;
			this->m_bSet = false;
			//Timer
			if (this->Diff())
				this->m_dTimer = 0.0f;
			else
				m_dTimer += dDelta;
		}
		//Has key changed from previous state
		inline bool		Diff(void)	const	{ return this->m_bCurr != m_bPrev; }
		//How long has the key been in its current state (up or down)
		inline double	Time(void)	const	{ return m_dTimer; }

		//Key Down
		inline bool		Held(void)	const	{ return this->m_bCurr; }
		//Key Down (no repeats)	
		inline bool		Push(void) const	{ return this->m_bCurr && !this->m_bPrev; }
		//Key Up (no repeats)
		inline bool		Pull(void) const	{ return !this->m_bCurr && this->m_bPrev; }
	};
}