#ifndef __FROG__DATETIME_H__
#define __FROG__DATETIME_H__

#include "FrogMemory.h"

namespace Webfoot {

//==============================================================================

/// The days of the week.
enum DayOfWeek
{
   DAY_OF_WEEK_SUNDAY,
   DAY_OF_WEEK_MONDAY,
   DAY_OF_WEEK_TUESDAY,
   DAY_OF_WEEK_WEDNESDAY,
   DAY_OF_WEEK_THURSDAY,
   DAY_OF_WEEK_FRIDAY,
   DAY_OF_WEEK_SATURDAY
};

/// Months of the year.
enum Month
{
   MONTH_JANUARY = 1,
   MONTH_FEBRUARY,
   MONTH_MARCH,
   MONTH_APRIL,
   MONTH_MAY,
   MONTH_JUNE,
   MONTH_JULY,
   MONTH_AUGUST,
   MONTH_SEPTEMBER,
   MONTH_OCTOBER,
   MONTH_NOVEMBER,
   MONTH_DECEMBER
};

/// Class for representing a date and time.
class DateTime
{
public:
   DateTime()
   {
      year = 1970;
      month = MONTH_JANUARY;
      day = 1;
      hour = 0;
      minute = 0;
      second = 0;
      millisecond = 0;
      dayOfWeek = DAY_OF_WEEK_THURSDAY;
   }

   /// Set the date and time.
   void Set(int _year, int _month, int _day, int _hour, int _minute, int _second, int _millisecond, int _dayOfWeek)
   {
      year = _year;
      month = _month;
      day = _day;
      hour = _hour;
      minute = _minute;
      second = _second;
      millisecond = _millisecond;
      dayOfWeek = _dayOfWeek;
   }

   /// Return the year. (ex: 1970)
   int YearGet() { return year; }
   /// Return the month. [1-12]
   int MonthGet() { return month; }
   /// Return the day of the month. [1-31]
   int DayGet() { return day; }

   /// Return the hour. [0-23]
   int HourGet() { return hour; }
   /// Return the minute. [0-59]
   int MinuteGet() { return minute; }
   /// Return the second. [0-59]
   int SecondGet() { return second; }
   /// Return the millisecond. [0-999]
   int MillisecondGet() { return millisecond; }

   /// Return the day of the week. (0-6)
   int DayOfWeekGet() { return dayOfWeek; }

protected:
   int year;
   int month;
   int day;
   int hour;
   int minute;
   int second;
   int millisecond;
   int dayOfWeek;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__DATETIME_H__
