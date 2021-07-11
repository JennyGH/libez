#include "DateTime.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if !defined(WIN32) && !defined(_WIN32)
#    include <cassert>
#    include <cstdarg>
#    include <unistd.h>
#    include <sys/types.h>
#    ifndef ASSERT
#        if _DEBUG
#            define ASSERT(prediction) assert((prediction))
#        else
#            define ASSERT(prediction) // no-op
#        endif
#    endif // !ASSERT
#    ifndef sprintf_s
static inline int __sprintf_s(char* dest, std::size_t destSize, const char* format, ...)
{
    ASSERT((dest != NULL) && (format != NULL));
    ASSERT((destSize > 0));
    va_list args;
    va_start(args, format);
    int sprintfedSize = vsnprintf(dest, destSize, format, args);
    ASSERT((destSize >= sprintfedSize));
    va_end(args);
    return sprintfedSize;
}
#        define sprintf_s __sprintf_s
#    endif // !sprintf_s
#    ifndef localtime_s
#        define localtime_s(_Tm, _Time) localtime_r((_Time), (_Tm))
#    endif // !localtime_s
#    ifndef gmtime_s
#        define gmtime_s(_Tm, _Time) gmtime_r((_Time), (_Tm))
#    endif // !gmtime_s
#endif     // !WIN32

#define NOW                     time(NULL)
#define SECONDS_PER_MINUTE      (60)
#define SECONDS_PER_HOUR        (SECONDS_PER_MINUTE * 60)
#define SECONDS_PER_DAY         (SECONDS_PER_HOUR * 24)
#define MILLISECONDS_PER_SECOND (1000)
#define MILLISECONDS_PER_MINUTE (SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND)
#define MILLISECONDS_PER_HOUR   (SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND)
#define MILLISECONDS_PER_DAY    (SECONDS_PER_DAY * MILLISECONDS_PER_SECOND)

#define INIT_DATETIME_PROPERTIES                                                                                       \
    UnixTimeStamp((*(_DateTime*)m_data).m_unixTimeStamp), Year((*(_DateTime*)m_data).m_year),                          \
        Month((*(_DateTime*)m_data).m_month), Day((*(_DateTime*)m_data).m_day), Hour((*(_DateTime*)m_data).m_hour),    \
        Minute((*(_DateTime*)m_data).m_minute), Second((*(_DateTime*)m_data).m_second),                                \
        Millisecond((*(_DateTime*)m_data).m_millisecond), DayOfWeek((*(_DateTime*)m_data).m_dayOfWeek),                \
        Kind((*(_DateTime*)m_data).m_kind)

#define INIT_TIMESPAN_PROPERTIES                                                                                       \
    Ticks((*(_TimeSpan*)m_data).m_ticks), TotalDays((*(_TimeSpan*)m_data).m_totalDays),                                \
        TotalHours((*(_TimeSpan*)m_data).m_totalHours), TotalMinutes((*(_TimeSpan*)m_data).m_totalMinutes),            \
        TotalSeconds((*(_TimeSpan*)m_data).m_totalSeconds),                                                            \
        TotalMilliseconds((*(_TimeSpan*)m_data).m_totalMilliseconds), Days((*(_TimeSpan*)m_data).m_days),              \
        Hours((*(_TimeSpan*)m_data).m_hours), Minutes((*(_TimeSpan*)m_data).m_minutes),                                \
        Seconds((*(_TimeSpan*)m_data).m_seconds), Milliseconds((*(_TimeSpan*)m_data).m_milliseconds)

#define COMPARE_DATETIME(this, op, that)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        _DateTime*                           pDateTime = static_cast<_DateTime*>(this->m_data);                        \
        _DateTime*                           pThatData = static_cast<_DateTime*>(that.m_data);                         \
        return pDateTime->m_unixTimeStamp op pThatData->m_unixTimeStamp;                                               \
    } while (0)

#define COMPARE_TIMESPAN(this, op, that)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        _TimeSpan*                       pThisTimeSpan = static_cast<_TimeSpan*>(this->m_data);                        \
        _TimeSpan*                       pThatTimeSpan = static_cast<_TimeSpan*>(that.m_data);                         \
        return pThisTimeSpan->m_ticks op pThatTimeSpan->m_ticks;                                                       \
    } while (0)

class _DateTime
{
public:
    _DateTime(
        time_t            ticks,
        int               year,
        int               month,
        int               day,
        int               hour,
        int               minute,
        int               seconds,
        int               millisecond,
        enum DayOfWeek    wday,
        enum DateTimeKind kind)
        : m_unixTimeStamp(ticks)
        , m_year(year)
        , m_month(month)
        , m_day(day)
        , m_hour(hour)
        , m_minute(minute)
        , m_second(seconds)
        , m_millisecond(millisecond)
        , m_dayOfWeek(wday)
        , m_kind(kind)
    {
    }

public:
    time_t            m_unixTimeStamp;
    int               m_year;
    int               m_month;
    int               m_day;
    int               m_hour;
    int               m_minute;
    int               m_second;
    int               m_millisecond;
    enum DayOfWeek    m_dayOfWeek;
    enum DateTimeKind m_kind;
};

class _TimeSpan
{
public:
    _TimeSpan(time_t milliseconds)
        : m_ticks(milliseconds)
        , m_days(milliseconds / MILLISECONDS_PER_DAY)
        , m_hours((milliseconds % MILLISECONDS_PER_DAY) / MILLISECONDS_PER_HOUR)
        , m_minutes((((milliseconds % MILLISECONDS_PER_DAY)) % MILLISECONDS_PER_HOUR) / MILLISECONDS_PER_MINUTE)
        , m_seconds(
              ((((milliseconds % MILLISECONDS_PER_DAY)) % MILLISECONDS_PER_HOUR) % MILLISECONDS_PER_MINUTE) /
              MILLISECONDS_PER_SECOND)
        , m_milliseconds(
              ((((milliseconds % MILLISECONDS_PER_DAY)) % MILLISECONDS_PER_HOUR) % MILLISECONDS_PER_MINUTE) %
              MILLISECONDS_PER_SECOND)
        , m_totalDays(double(milliseconds) / MILLISECONDS_PER_DAY)
        , m_totalHours(double(milliseconds) / MILLISECONDS_PER_HOUR)
        , m_totalMinutes(double(milliseconds) / MILLISECONDS_PER_MINUTE)
        , m_totalSeconds(double(milliseconds) / MILLISECONDS_PER_SECOND)
        , m_totalMilliseconds(milliseconds)
    {
    }
    _TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
        : m_ticks(
              days * MILLISECONDS_PER_DAY + hours * MILLISECONDS_PER_HOUR + minutes * MILLISECONDS_PER_MINUTE +
              seconds * MILLISECONDS_PER_SECOND + milliseconds)
        , m_days(days)
        , m_hours(hours)
        , m_minutes(minutes)
        , m_seconds(seconds)
        , m_milliseconds(milliseconds)
        , m_totalDays(double(m_ticks) / MILLISECONDS_PER_DAY)
        , m_totalHours(double(m_ticks) / MILLISECONDS_PER_HOUR)
        , m_totalMinutes(double(m_ticks) / MILLISECONDS_PER_MINUTE)
        , m_totalSeconds(double(m_ticks) / MILLISECONDS_PER_SECOND)
        , m_totalMilliseconds(m_ticks)
    {
    }

    void FromTicks(time_t ticks);
    void FromDateTime(int days, int hours, int minutes, int seconds, int milliseconds);

public:
    time_t m_ticks;
    int    m_days;
    int    m_hours;
    int    m_minutes;
    int    m_seconds;
    int    m_milliseconds;
    double m_totalDays;
    double m_totalHours;
    double m_totalMinutes;
    double m_totalSeconds;
    double m_totalMilliseconds;
};

void _TimeSpan::FromTicks(time_t ticks)
{
    m_ticks   = (ticks);
    m_days    = (ticks / MILLISECONDS_PER_DAY);
    m_hours   = ((ticks % MILLISECONDS_PER_DAY) / MILLISECONDS_PER_HOUR);
    m_minutes = ((((ticks % MILLISECONDS_PER_DAY)) % MILLISECONDS_PER_HOUR) / MILLISECONDS_PER_MINUTE);
    m_seconds =
        (((((ticks % MILLISECONDS_PER_DAY)) % MILLISECONDS_PER_HOUR) % MILLISECONDS_PER_MINUTE) /
         MILLISECONDS_PER_SECOND);
    m_milliseconds =
        (((((ticks % MILLISECONDS_PER_DAY)) % MILLISECONDS_PER_HOUR) % MILLISECONDS_PER_MINUTE) %
         MILLISECONDS_PER_SECOND);
    m_totalDays         = (double(ticks) / MILLISECONDS_PER_DAY);
    m_totalHours        = (double(ticks) / MILLISECONDS_PER_HOUR);
    m_totalMinutes      = (double(ticks) / MILLISECONDS_PER_MINUTE);
    m_totalSeconds      = (double(ticks) / MILLISECONDS_PER_SECOND);
    m_totalMilliseconds = (ticks);
}

void _TimeSpan::FromDateTime(int days, int hours, int minutes, int seconds, int milliseconds)
{
    m_ticks =
        (days * MILLISECONDS_PER_DAY + hours * MILLISECONDS_PER_HOUR + minutes * MILLISECONDS_PER_MINUTE +
         seconds * MILLISECONDS_PER_SECOND + milliseconds);
    m_days              = (days);
    m_hours             = (hours);
    m_minutes           = (minutes);
    m_seconds           = (seconds);
    m_milliseconds      = (milliseconds);
    m_totalDays         = (double(m_ticks) / MILLISECONDS_PER_DAY);
    m_totalHours        = (double(m_ticks) / MILLISECONDS_PER_HOUR);
    m_totalMinutes      = (double(m_ticks) / MILLISECONDS_PER_MINUTE);
    m_totalSeconds      = (double(m_ticks) / MILLISECONDS_PER_SECOND);
    m_totalMilliseconds = (m_ticks);
}

time_t ParseTicksFromDateTime(int year, int month, int day, int hour, int minute, int second, enum DayOfWeek& wday)
{
    tm temp;
    memset(&temp, 0, sizeof(temp));
    temp.tm_year = year - 1900;
    temp.tm_mon  = month - 1;
    temp.tm_mday = day;
    temp.tm_hour = hour;
    temp.tm_min  = minute;
    temp.tm_sec  = second;

    //锟斤拷锟斤拷锟斤拷锟节硷拷
    time_t ticks = mktime(&temp);
    localtime_s(&temp, &ticks);
    wday = static_cast<enum DayOfWeek>(temp.tm_wday);

    return ticks;
}

void ParseDateTimeFromTicks(
    DateTimeKind    kind,
    time_t          ticks,
    int&            year,
    int&            month,
    int&            day,
    int&            hour,
    int&            minute,
    int&            second,
    enum DayOfWeek& wday)
{
    tm temp;
    memset(&temp, 0, sizeof(temp));

    switch (kind)
    {
        case Utc:
            gmtime_s(&temp, &ticks);
            break;
        case Local:
            localtime_s(&temp, &ticks);
            break;
        default:
            localtime_s(&temp, &ticks);
            break;
    }

    year   = temp.tm_year + 1900;
    month  = temp.tm_mon + 1;
    day    = temp.tm_mday;
    hour   = temp.tm_hour;
    minute = temp.tm_min;
    second = temp.tm_sec;
    wday   = static_cast<enum DayOfWeek>(temp.tm_wday);
}

DateTime::DateTime()
    : m_data(new _DateTime(NOW, 0, 0, 0, 0, 0, 0, 0, Sunday, Unspecified))
    , INIT_DATETIME_PROPERTIES
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    ParseDateTimeFromTicks(
        pDateTime->m_kind,
        pDateTime->m_unixTimeStamp,
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
}

DateTime::DateTime(time_t unixTimeStamp)
    : m_data(new _DateTime(unixTimeStamp, 0, 0, 0, 0, 0, 0, 0, Sunday, Unspecified))
    , INIT_DATETIME_PROPERTIES
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    ParseDateTimeFromTicks(
        pDateTime->m_kind,
        pDateTime->m_unixTimeStamp,
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
}

DateTime::DateTime(time_t unixTimeStamp, DateTimeKind kind)
    : m_data(new _DateTime(unixTimeStamp, 0, 0, 0, 0, 0, 0, 0, Sunday, kind))
    , INIT_DATETIME_PROPERTIES
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    ParseDateTimeFromTicks(
        pDateTime->m_kind,
        pDateTime->m_unixTimeStamp,
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
}

DateTime::DateTime(int year, int month, int day)
    : m_data(new _DateTime(0, year, month, day, 0, 0, 0, 0, Sunday, Unspecified))
    , INIT_DATETIME_PROPERTIES
{
    _DateTime* pDateTime       = static_cast<_DateTime*>(m_data);
    pDateTime->m_unixTimeStamp = ParseTicksFromDateTime(
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
    : m_data(new _DateTime(0, year, month, day, hour, minute, second, 0, Sunday, Unspecified))
    , INIT_DATETIME_PROPERTIES
{
    _DateTime* pDateTime       = static_cast<_DateTime*>(m_data);
    pDateTime->m_unixTimeStamp = ParseTicksFromDateTime(
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
    : m_data(new _DateTime(0, year, month, day, hour, minute, second, 0, Sunday, kind))
    , INIT_DATETIME_PROPERTIES
{
    _DateTime* pDateTime       = static_cast<_DateTime*>(m_data);
    pDateTime->m_unixTimeStamp = ParseTicksFromDateTime(
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
    : m_data(new _DateTime(0, year, month, day, hour, minute, second, millisecond, Sunday, Unspecified))
    , INIT_DATETIME_PROPERTIES
{
    _DateTime* pDateTime       = static_cast<_DateTime*>(m_data);
    pDateTime->m_unixTimeStamp = ParseTicksFromDateTime(
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
    : m_data(new _DateTime(0, year, month, day, hour, minute, second, millisecond, Sunday, kind))
    , INIT_DATETIME_PROPERTIES
{
    _DateTime* pDateTime       = static_cast<_DateTime*>(m_data);
    pDateTime->m_unixTimeStamp = ParseTicksFromDateTime(
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
}

DateTime::DateTime(const DateTime& that)
    : m_data(new _DateTime(
          that.UnixTimeStamp,
          that.Year,
          that.Month,
          that.Day,
          that.Hour,
          that.Minute,
          that.Second,
          that.Millisecond,
          that.DayOfWeek,
          that.Kind))
    , INIT_DATETIME_PROPERTIES
{
}

DateTime::~DateTime()
{
    if (NULL != m_data)
    {
        _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
        delete pDateTime;
        pDateTime = NULL;
        m_data    = NULL;
    }
}

DateTime& DateTime::operator=(const DateTime& that)
{
    if (&that != this)
    {
        _DateTime* pThisData = static_cast<_DateTime*>(this->m_data);
        _DateTime* pThatData = static_cast<_DateTime*>(that.m_data);
        if (NULL != pThisData)
        {
            delete pThisData;
            pThisData = NULL;
        }
        if (NULL != pThatData)
        {
            m_data = pThisData = new _DateTime(
                pThatData->m_unixTimeStamp,
                pThatData->m_year,
                pThatData->m_month,
                pThatData->m_day,
                pThatData->m_hour,
                pThatData->m_minute,
                pThatData->m_second,
                pThatData->m_millisecond,
                pThatData->m_dayOfWeek,
                pThatData->m_kind);
        }
    }
    return *this;
}

DateTime& DateTime::operator+(const TimeSpan& that)
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    _TimeSpan* pTimeSpan = static_cast<_TimeSpan*>(that.m_data);

    pDateTime->m_unixTimeStamp += pTimeSpan->m_ticks;

    if (pDateTime->m_unixTimeStamp < 0)
    {
        pDateTime->m_unixTimeStamp = 0;
    }

    ParseDateTimeFromTicks(
        pDateTime->m_kind,
        pDateTime->m_unixTimeStamp,
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);

    return *this;
}

DateTime& DateTime::operator-(const TimeSpan& that)
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    _TimeSpan* pTimeSpan = static_cast<_TimeSpan*>(that.m_data);

    pDateTime->m_unixTimeStamp -= pTimeSpan->m_ticks;

    if (pDateTime->m_unixTimeStamp < 0)
    {
        pDateTime->m_unixTimeStamp = 0;
    }

    ParseDateTimeFromTicks(
        pDateTime->m_kind,
        pDateTime->m_unixTimeStamp,
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);

    return *this;
}

TimeSpan DateTime::operator-(const DateTime& that)
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    _DateTime* pThatData = static_cast<_DateTime*>(that.m_data);
    return (pDateTime->m_unixTimeStamp - pThatData->m_unixTimeStamp) * MILLISECONDS_PER_SECOND;
}

bool DateTime::operator<(const DateTime& that) const
{
    COMPARE_DATETIME(this, <, that);
}

bool DateTime::operator>(const DateTime& that) const
{
    COMPARE_DATETIME(this, >, that);
}

bool DateTime::operator<=(const DateTime& that) const
{
    COMPARE_DATETIME(this, <=, that);
}

bool DateTime::operator>=(const DateTime& that) const
{
    COMPARE_DATETIME(this, >=, that);
}

bool DateTime::operator==(const DateTime& that) const
{
    COMPARE_DATETIME(this, ==, that);
}

bool DateTime::operator!=(const DateTime& that) const
{
    COMPARE_DATETIME(this, !=, that);
}

DateTime& DateTime::AddYears(int value)
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    pDateTime->m_year += value;
    pDateTime->m_unixTimeStamp = ParseTicksFromDateTime(
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
    return *this;
}

DateTime& DateTime::AddMonths(int value)
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    pDateTime->m_month += value;
    pDateTime->m_unixTimeStamp = ParseTicksFromDateTime(
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);
    return *this;
}

DateTime& DateTime::AddDays(double value)
{
    return AddSeconds(value * SECONDS_PER_DAY);
}

DateTime& DateTime::AddHours(double value)
{
    return AddSeconds(value * SECONDS_PER_HOUR);
}

DateTime& DateTime::AddMinutes(double value)
{
    return AddSeconds(value * SECONDS_PER_MINUTE);
}

DateTime& DateTime::AddSeconds(double value)
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);

    double seconds             = (pDateTime->m_unixTimeStamp + value);
    pDateTime->m_unixTimeStamp = seconds;
    pDateTime->m_millisecond   = (seconds - pDateTime->m_unixTimeStamp) * 1000;

    ParseDateTimeFromTicks(
        pDateTime->m_kind,
        pDateTime->m_unixTimeStamp,
        pDateTime->m_year,
        pDateTime->m_month,
        pDateTime->m_day,
        pDateTime->m_hour,
        pDateTime->m_minute,
        pDateTime->m_second,
        pDateTime->m_dayOfWeek);

    return *this;
}

DateTime& DateTime::AddMilliseconds(double value)
{
    return AddSeconds(value / 1000);
}

DateTime DateTime::ToLocalTime() const
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    return DateTime(pDateTime->m_unixTimeStamp, Local);
}

DateTime DateTime::ToUniversalTime() const
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    return DateTime(pDateTime->m_unixTimeStamp, Utc);
}

std::string DateTime::ToLongDateString() const
{
    _DateTime* pDateTime   = static_cast<_DateTime*>(m_data);
    char       buffer[256] = {0};
    sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d", pDateTime->m_year, pDateTime->m_month, pDateTime->m_day);
    return buffer;
}

std::string DateTime::ToLongTimeString() const
{
    _DateTime* pDateTime   = static_cast<_DateTime*>(m_data);
    char       buffer[256] = {0};
    sprintf_s(buffer, sizeof(buffer), "%02d:%02d:%02d", pDateTime->m_hour, pDateTime->m_minute, pDateTime->m_second);
    return buffer;
}

std::string DateTime::ToShortDateString() const
{
    _DateTime* pDateTime   = static_cast<_DateTime*>(m_data);
    char       buffer[256] = {0};
    sprintf_s(buffer, sizeof(buffer), "%d/%02d/%02d", pDateTime->m_year, pDateTime->m_month, pDateTime->m_day);
    return buffer;
}

std::string DateTime::ToShortTimeString() const
{
    _DateTime* pDateTime   = static_cast<_DateTime*>(m_data);
    char       buffer[256] = {0};
    sprintf_s(buffer, sizeof(buffer), "%02d:%02d", pDateTime->m_hour, pDateTime->m_minute);
    return buffer;
}

std::string DateTime::ToString(const std::string& format) const
{
    _DateTime* pDateTime = static_cast<_DateTime*>(m_data);
    if (format.empty())
    {
        return "";
    }

    char buffer[256] = {0};
    tm   temp;
    switch (pDateTime->m_kind)
    {
        case Utc:
            gmtime_s(&temp, &pDateTime->m_unixTimeStamp);
            break;
        case Local:
            localtime_s(&temp, &pDateTime->m_unixTimeStamp);
            break;
        default:
            localtime_s(&temp, &pDateTime->m_unixTimeStamp);
            break;
    }
    size_t rv = strftime(buffer, 256, format.c_str(), &temp);
    return std::string(buffer, rv);
}

DateTime DateTime::Now()
{
    return DateTime(NOW);
}

DateTime DateTime::Today()
{
    DateTime now(NOW);
    return DateTime(now.Year, now.Month, now.Day);
}

DateTime DateTime::UtcNow()
{
    return DateTime(NOW, Utc);
}

const TimeSpan& TimeSpan::MaxValue = ((1LL << (sizeof(time_t) * 8 - 1)) - 1) / (MILLISECONDS_PER_SECOND * 10);
const TimeSpan& TimeSpan::MinValue = ((1LL << (sizeof(time_t) * 8 - 1)) + 1) / (MILLISECONDS_PER_SECOND * 10);

TimeSpan::TimeSpan(time_t ticks)
    : m_data(new _TimeSpan(ticks))
    , INIT_TIMESPAN_PROPERTIES
{
}

TimeSpan::TimeSpan(int hours, int minutes, int seconds)
    : m_data(new _TimeSpan(0, hours, minutes, seconds, 0))
    , INIT_TIMESPAN_PROPERTIES
{
}

TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds)
    : m_data(new _TimeSpan(days, hours, minutes, seconds, 0))
    , INIT_TIMESPAN_PROPERTIES
{
}

TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
    : m_data(new _TimeSpan(days, hours, minutes, seconds, milliseconds))
    , INIT_TIMESPAN_PROPERTIES
{
}

TimeSpan::TimeSpan(const TimeSpan& that)
    : m_data(new _TimeSpan(that.Days, that.Hours, that.Minutes, that.Seconds, that.Milliseconds))
    , INIT_TIMESPAN_PROPERTIES
{
}

TimeSpan& TimeSpan::operator=(const TimeSpan& that)
{
    if (&that != this)
    {
        _TimeSpan* pThisTimespan = static_cast<_TimeSpan*>(m_data);
        _TimeSpan* pThatTimespan = static_cast<_TimeSpan*>(that.m_data);
        if (pThatTimespan != NULL)
        {
            pThisTimespan->FromTicks(pThatTimespan->m_ticks);
        }
    }
    return *this;
}

TimeSpan TimeSpan::operator+(const TimeSpan& that) const
{
    _TimeSpan* pThisTimespan = static_cast<_TimeSpan*>(m_data);
    _TimeSpan* pThatTimespan = static_cast<_TimeSpan*>(that.m_data);
    if (pThisTimespan == NULL || pThatTimespan == NULL)
    {
        return 0;
    }
    return pThisTimespan->m_ticks + pThatTimespan->m_ticks;
}

TimeSpan TimeSpan::operator-(const TimeSpan& that) const
{
    _TimeSpan* pThisTimespan = static_cast<_TimeSpan*>(m_data);
    _TimeSpan* pThatTimespan = static_cast<_TimeSpan*>(that.m_data);
    if (pThisTimespan == NULL || pThatTimespan == NULL)
    {
        return 0;
    }
    return pThisTimespan->m_ticks - pThatTimespan->m_ticks;
}

bool TimeSpan::operator>(const TimeSpan& that) const
{
    COMPARE_TIMESPAN(this, >, that);
}

bool TimeSpan::operator<(const TimeSpan& that) const
{
    COMPARE_TIMESPAN(this, <, that);
}

bool TimeSpan::operator>=(const TimeSpan& that) const
{
    COMPARE_TIMESPAN(this, >=, that);
}

bool TimeSpan::operator<=(const TimeSpan& that) const
{
    COMPARE_TIMESPAN(this, <=, that);
}

bool TimeSpan::operator==(const TimeSpan& that) const
{
    COMPARE_TIMESPAN(this, ==, that);
}

bool TimeSpan::operator!=(const TimeSpan& that) const
{
    COMPARE_TIMESPAN(this, !=, that);
}

TimeSpan& TimeSpan::Add(const TimeSpan& timespan)
{
    _TimeSpan* pThisTimespan = static_cast<_TimeSpan*>(m_data);
    _TimeSpan* pThatTimespan = static_cast<_TimeSpan*>(timespan.m_data);
    if (pThisTimespan != NULL && pThatTimespan != NULL)
    {
        pThisTimespan->FromTicks(pThisTimespan->m_ticks + pThatTimespan->m_ticks);
    }
    return *this;
}

TimeSpan& TimeSpan::Subtract(const TimeSpan& timespan)
{
    _TimeSpan* pThisTimespan = static_cast<_TimeSpan*>(m_data);
    _TimeSpan* pThatTimespan = static_cast<_TimeSpan*>(timespan.m_data);
    if (pThisTimespan != NULL && pThatTimespan != NULL)
    {
        pThisTimespan->FromTicks(pThisTimespan->m_ticks - pThatTimespan->m_ticks);
    }
    return *this;
}

TimeSpan TimeSpan::Duration() const
{
    _TimeSpan* pThisTimespan = static_cast<_TimeSpan*>(m_data);
    return pThisTimespan->m_ticks < 0 ? -(pThisTimespan->m_ticks) : pThisTimespan->m_ticks;
}

TimeSpan TimeSpan::Negate() const
{
    _TimeSpan* pThisTimespan = static_cast<_TimeSpan*>(m_data);
    return -pThisTimespan->m_ticks;
}

TimeSpan::~TimeSpan()
{
    _TimeSpan* pThisTimespan = static_cast<_TimeSpan*>(m_data);
    if (NULL != pThisTimespan)
    {
        delete pThisTimespan;
        m_data = pThisTimespan = NULL;
    }
}

TimeSpan TimeSpan::FromDays(double value)
{
    return value * MILLISECONDS_PER_DAY + 0.5;
}

TimeSpan TimeSpan::FromHours(double value)
{
    return value * MILLISECONDS_PER_HOUR + 0.5;
}

TimeSpan TimeSpan::FromMilliseconds(double value)
{
    return value;
}

TimeSpan TimeSpan::FromMinutes(double value)
{
    return value * MILLISECONDS_PER_MINUTE + 0.5;
}

TimeSpan TimeSpan::FromSeconds(double value)
{
    return value * MILLISECONDS_PER_SECOND + 0.5;
}

TimeSpan TimeSpan::FromTicks(time_t value)
{
    return (value + 0.5) / (MILLISECONDS_PER_SECOND * 10);
}
