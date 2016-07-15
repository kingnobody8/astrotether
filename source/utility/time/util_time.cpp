#include "util_time.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <chrono>

#undef min
#undef max

using namespace util;

//STATIC const Date EPOCH = Date();

STATIC const int Time::DAYS_IN_MONTH[12] =
{
	Time::JAN_DAYS,
	Time::FEB_DAYS,
	Time::MAR_DAYS,
	Time::APR_DAYS,
	Time::MAY_DAYS,
	Time::JUNE_DAYS,
	Time::JULY_DAYS,
	Time::AUG_DAYS,
	Time::SEPT_DAYS,
	Time::OCT_DAYS,
	Time::NOV_DAYS,
	Time::DEC_DAYS
};

Date::Date()
	: year(1970)
	, month(0)
	, day(0)
	, hour(0)
	, min(0)
	, sec(0)
	, milli(0)
{
}

//Date(const Time& t)
//{
//}

void Date::ParseTime(const Time& t)
{
	__not_implemented(Date::ParseTime);
	//Time::timeval_t tmp = t.Milli();
	//
	//const lldiv_t fract = lldiv(tmp, Time::YEAR);
	//year = (int)(fract.quot + 1970);
	//tmp = fract.rem;
	//
	////Remove the leap days
	//if (year >= 1972)
	//{
	//	int leaps = (year - 1972) / 4 + 1; //leap years happen every 4 years and the first one after epoch was 1972
	//	tmp -= leaps * Time::DAY;
	//}
	//
	//for (int i = Time::EMonth::EM_COUNT - 1; i >= 0; --i)
	//{
	//	if (tmp > Time::DAYS_IN_MONTH[i])
	//	{
	//
	//	}
	//}
}

const Time Date::ToTimeAsUtc() const
{
	__not_implemented(Date::ToTimeAsUtc);
	return 0;
	//assert(year > 1970 && (month >= 0 && month < 12) && (hour >= 0 && hour < 24) && (min >= 0 && min < 60) && (sec >= 0 && sec < 60) && (milli >= 0 && milli < 1000) && "One of these parameters is invalid");
	//
	//Time ret;
	//ret = (year - 1970) * Time::YEAR;
	//
	////add leap days
	//if (year >= 1972)
	//{
	//	int leaps = (year - 1972) / 4 + 1; //leap years happen every 4 years and the first one after epoch was 1972
	//	ret += leaps * Time::DAY;
	//}
	//
	//for (int i = 0; i < month; ++i)
	//{
	//	ret += Time::DAYS_IN_MONTH[i] * Time::DAY;
	//}
	//
	//ret += day * Time::DAY;
	//ret += hour * Time::HR;
	//ret += min * Time::MIN;
	//ret += sec * Time::SEC;
	//ret += milli;
	//
	//return ret;
}

const Time Date::ToTimeAsLocal(void) const
{
	__not_implemented(Date::ToTimeAsLocal);
	return 0;

	//tm ts;
	//ts.tm_hour = hour;
	//ts.tm_isdst = -1;
	//ts.tm_mday = day;
	//ts.tm_min = min;
	//ts.tm_mon = month;
	//ts.tm_sec = sec;
	//ts.tm_year = year;
	//
	//time_t time = mktime(&ts);
	//
	////TODO create a temp date using our fields to set it to utc then call ToTimeAsUtc on it
	//return ToTimeAsUtc();
}


STATIC const Time Time::GetTimeSinceEpoch()
{
	ulonglong ret = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return ret;
}

//STATIC const Time Time::GetLocalOffset()
//{
//	long before = timezone;
//
//	tzset();
//
//	long after = timezone;
//	//time_t t = mktime(timeinfo) - timezone;
//
//
//	//time_t now;
//	//std::time(&now);
//
//	//localtime(&now)->tm_gmtimeoff;
//
//	Time ret;
//
//	time_t now = std::time(nullptr); // UTC
//	time_t diff;
//	struct tm *ptmgm = gmtime(&now); // further convert to GMT presuming now in local
//	time_t gmnow = mktime(ptmgm);
//	diff = (gmnow - now) * 1000; //time_t is in seconds???
//	if (ptmgm->tm_isdst > 0) {
//		diff = diff - Time::HR;
//	}
//
//	return ret;
//}

STATIC const bool Time::DaylightSavings()
{
	tzset(); //load timezone information (this can be called just once) (timezone is value to subract from UTC)
	return daylight == 1;
}

STATIC const float Time::GetTimezone()
{
	tzset(); //load timezone information (this can be called just once) (timezone is value to subract from UTC)
	float zone = -timezone / float(HR / 1000);

	//int light = daylight;
	//tm ts;
	//time_t now;
	//std::time(&now);
	//localtime_s(&ts, &now);
	//
	//if (daylight == 0 && ts.tm_isdst != 0)
	//	++zone;
	//else if (daylight != 0 && ts.tm_isdst == 0)
	//	--zone;
	
	return zone;
}

Time::Time()
	: m_time(0)
{
}

Time::Time(const u64& milli)
	: m_time(milli)
{
}

Time::Time(const Date& d)
{
	__not_implemented(Date::Time(const Date& d));
}

const Date Time::ToUtc() const
{
	Date ret;
	time_t tmp = m_time / 1000;
	ret.milli = (int)(m_time - (tmp * 1000));
	tm ts;
#ifdef IS_WIN
	gmtime_s(&ts, &tmp);
#else
	gmtime_r(&tmp, &ts);
#endif
	ret.day = ts.tm_mday;
	ret.hour = ts.tm_hour;
	ret.min = ts.tm_min;
	ret.month = ts.tm_mon;
	ret.sec = ts.tm_sec;
	ret.year = ts.tm_year + 1900;
	
	return ret;
}

const Date Time::ToLocal() const
{
	Date ret;
	time_t tmp = m_time / 1000;
	tm ts;
#ifdef IS_WIN
	localtime_s(&ts, &tmp);
#else
	localtime_r(&tmp, &ts);
#endif
	
	ret.day = ts.tm_mday;
	ret.hour = ts.tm_hour;
	ret.min = ts.tm_min;
	ret.month = ts.tm_mon;
	ret.sec = ts.tm_sec;
	ret.year = ts.tm_year + 1900;
	return ret;
}