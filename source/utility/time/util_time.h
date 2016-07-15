#pragma once
#include "types/types.h"

namespace util
{
	//TIME format??
	// 2015y:09m:04d:22h:03m:46s:32ms
	// 15:09:04:22:03:46:32
	// YYYY:MM:DD:HH:MM:SS:MS
	// 2015:09:04:22:03:46:32

	//forward declaration
	class Time;

	//NOTE each field is zero based ie: January 1st == 0,0
	struct Date
	{
		int year;
		int month;
		int day;
		int hour;
		int min;
		int sec;
		int milli;

		Date();
		//Date(const Time& t);
		void ParseTime(const Time& t);
		const Time ToTimeAsUtc(void) const;
		const Time ToTimeAsLocal(void) const;
	};

	class Time
	{
	public:
		typedef u64 timeval_t;

		static const timeval_t SEC = 1000;
		static const timeval_t MIN = SEC * 60;
		static const timeval_t HR = MIN * 60;
		static const timeval_t DAY = HR * 24;
		static const timeval_t WEEK = DAY * 7;
		static const timeval_t YEAR = DAY * 365;

		__todo()
		//static const Date EPOCH;

		static const int JAN_DAYS = 31;
		static const int FEB_DAYS = 28;
		static const int FEB_LEAP_DAYS = 29;
		static const int MAR_DAYS = 31;
		static const int APR_DAYS = 30;
		static const int MAY_DAYS = 31;
		static const int JUNE_DAYS = 30;
		static const int JULY_DAYS = 31;
		static const int AUG_DAYS = 31;
		static const int SEPT_DAYS = 30;
		static const int OCT_DAYS = 31;
		static const int NOV_DAYS = 30;
		static const int DEC_DAYS = 31;

		enum EMonth
		{
			EM_JAN = 0,
			EM_FEB,
			EM_MAR,
			EM_APR,
			EM_MAY,
			EM_JUNE,
			EM_JULY,
			EM_AUG,
			EM_SEPT,
			EM_OCT,
			EM_NOV,
			EM_DEC,
			EM_COUNT
		};

		static const int DAYS_IN_MONTH[EM_COUNT];

		static const Time GetTimeSinceEpoch();
		static const float GetTimezone();
		static const bool DaylightSavings();

		Time();
		Time(const u64& milli);
		Time(const Date& d);
		const timeval_t& Milli() const { return m_time; }
		const double ToDouble() const { return (double)m_time; }
		const Date ToUtc() const;
		const Date ToLocal() const;
		
		// Increment (++, --)
		inline Time& operator++() { ++m_time; return *this; }
		inline Time operator++(int) { Time r = *this; ++m_time; return r; }
		inline Time& operator--() { TimeDiff(1); return *this; }
		inline Time operator--(int) { Time r = *this; TimeDiff(1); return r; }

		// Arithmetic (+, -, *, /, %)
		inline Time operator+(const Time& rhs) const { return Time(m_time + rhs.m_time); }
		inline Time operator-(const Time& rhs) const { Time r(m_time); r.TimeDiff(rhs.m_time); return r; }
		inline Time operator*(const Time& rhs) const { return Time(m_time * rhs.m_time); }
		inline Time operator*(double rhs) const { return Time((timeval_t)(m_time * rhs)); }
		// The return value from div is no longer a time - it's more like a ratio or percentage
		inline double operator/(const Time& rhs) const { return (m_time / (double)rhs.m_time); }
		inline Time operator%(const Time& rhs) const { return Time(m_time % rhs.m_time); }

		inline Time& operator+=(const Time& rhs) { m_time += rhs.m_time; return *this; }
		inline Time& operator-=(const Time& rhs) { TimeDiff(rhs.m_time); return *this; }
		inline Time& operator*=(const Time& rhs) { m_time *= rhs.m_time; return *this; }
		// /= performs a scaling operation
		inline Time& operator/=(double rhs) { m_time = (timeval_t)(m_time * (1.0 / rhs)); return *this; }
		inline Time& operator%=(const Time& rhs) { m_time %= rhs.m_time; return *this; }

		// Bitwise (&, |, ^, <<, >>)
		// (not supported)

		// Logical (!, &&, ||)
		// (not supported)

		// Compare (<, >, <=, >=)
		inline bool operator< (const Time& rhs) const { return m_time < rhs.m_time; }
		inline bool operator>(const Time& rhs) const { return rhs < *this; }
		inline bool operator<=(const Time& rhs) const { return !(*this > rhs); }
		inline bool operator>=(const Time& rhs) const { return !(*this < rhs); }

		// Equality (==, !=)
		inline bool operator==(const Time& rhs) const { return m_time == rhs.m_time; }
		inline bool operator!=(const Time& rhs) const { return !(*this == rhs); }

	private:
		timeval_t m_time;

		// Subtract v from m_time, with underflow clamp
		inline void TimeDiff(timeval_t t)
		{
			// if underflow clamp to zero
			if (t > m_time)
			{
				m_time = 0;
			}
			else
			{
				m_time -= t;
			}
		}
	};
}