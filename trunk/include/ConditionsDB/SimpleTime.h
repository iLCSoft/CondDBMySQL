/* $Id: SimpleTime.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
/////////////////////////////////////////////////
// simpleTime.h
//
// Purpose: SimpleTime is with nanosecond 
// precision representing a point in time 
// (1900-01-01 .. 2484-07-02). Various
// arithmetic, conversion and comparison 
// operations are supported.
// 
// Author: Erik Zeitler  Date: 2000-09-06
//
/////////////////////////////////////////////////

// TODO: 
// - Make conversion operations to/from *NIX time_b structs (ms precision).
// - Include exceptions when errors occur

#ifndef SimpleTime_h
#define SimpleTime_h

#ifdef AIDA_STD
# undef AIDA_STD
#endif

#ifdef AIDA_DONT_USE_STD
# define AIDA_STD
#else
# define AIDA_STD std
#endif

// for time_t
#include <time.h>

// for NetLogger output
#include <string>

// SimpleTime
// Erik Zeitler 2000
//
// __References__
// Number of days per year (365.25637):
// - Nordling, Österman:
//   Physics Handbook For Science And Engineering,
//   Studentlitteratur AB, 1999. ISBN 9144008236
// For time_t:
// - Kernighan, Ritchie: The C Programming Language,
//   Second edition, Prentice Hall 1988.

// Win32 does not support properly unsigned 64 bit integers
#ifdef _WIN32
typedef __int64 TimeT;
#else
//-ap, 17-may-2001: 
//     On request from the ConditionsDB people (forwarded by Stefano Paoli)
//     the type of "TimeT" has been changed from "unsigned long long" to "long long"
//-ap end
typedef long long TimeT;
#endif

typedef struct {
  unsigned short year;
  unsigned short month;
  unsigned short day;
  unsigned short hour;
  unsigned short min;
  unsigned short sec;
  unsigned long ns;
  bool plusInf;
  bool minusInf;
} timeAndDate_t;

class SimpleTimeDuration;
class SimpleTimeInterval;

class SimpleTime { 
 public:
  // Construction/Destruction
  SimpleTime();
  SimpleTime(const SimpleTime & t) {timeval=t.toTimeT();};
  SimpleTime(const time_t &);
  SimpleTime(const TimeT &);
  SimpleTime(const timeAndDate_t &);
  SimpleTime(const unsigned short year,
	     const unsigned short month,
	     const unsigned short day,
	     const unsigned short hour,
	     const unsigned short min,
	     const unsigned short sec,
	     const unsigned long ns=0,
	     const bool plusInf=0,
	     const bool minusInf=0);
  SimpleTime(const AIDA_STD::string &);
  ~SimpleTime() {};

  // Operators
  void operator = (const SimpleTime &);
  void operator = (const TimeT &);
  void operator += (const SimpleTimeDuration &);
  void operator -= (const SimpleTimeDuration &);

  operator TimeT() const { return toTimeT(); }

  // Comparison
  bool operator == (const SimpleTime &other) { return timeval == other.toTimeT(); }
  bool isBefore(const SimpleTime&) const;

  // Deprecated: don't use this method, use toTimeT() instead.
  // This method will be removed at the next release !
  TimeT time() const;

  // Auxiliary functions
  void setPlusInf();
  void setMinusInf();
  bool isPlusInf() const;
  bool isMinusInf() const;

  // Conversion and exchange
  TimeT  toTimeT() const;
  time_t toUnixTime() const;
  void fromUnixTime(const time_t &unix_time);
  AIDA_STD::string toNetLogger() const;
  void fromNetLogger(const AIDA_STD::string &);
  void fromCalendarTime(const timeAndDate_t &);
  void fromCalendarTime(const unsigned short year,
			const unsigned short month,
			const unsigned short day,
			const unsigned short hour,
			const unsigned short minute,
			const unsigned short sec,
			const unsigned long nanosec=0,
			const bool plusInf=0,
			const bool minusInf=0);
  timeAndDate_t toCalendarTime() const;
  unsigned short year() const;
  unsigned short month() const;
  unsigned short day() const;
  unsigned short hour() const;
  unsigned short minute() const;
  unsigned short second() const;
  unsigned long nanosec() const;

 private:
  TimeT timeval;
};

// Streaming __int64 is not supported on NT
#ifndef _WIN32
AIDA_STD::ostream & operator << (AIDA_STD::ostream & out, const SimpleTime &);
#endif
SimpleTime operator + (const SimpleTime &, const SimpleTimeDuration &);
SimpleTime operator - (const SimpleTime &, const SimpleTimeDuration &);

#endif // SimpleTime_h

