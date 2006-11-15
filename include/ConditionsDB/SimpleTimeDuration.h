/////////////////////////////////////////////////
// simpleduration.h
//
// Purpose: SimpleTimeDuration is a nonnegative
// distance in time. Various comparison and
// arithmetic operations are supported.
// 
// Author: Erik Zeitler  Date: 2000-09-06
//
/////////////////////////////////////////////////

// TODO:
// - Make functions to convert to/from more human readable strings i.e. 
//   day(), hour(), min(), sec(), ns(). 

#ifndef SIMPLE_DURATION_H
#define SIMPLE_DURATION_H
#include <iostream>

// Win32 does not support properly unsigned 64 bit integers
#ifdef _WIN32
typedef __int64 DurationT;
const DurationT Billion=1000000000;
#else
//-ap, 17-may-2001: 
//     On request from the ConditionsDB people (forwarded by Stefano Paoli)
//     the type of "DurationT" has been changed from "unsigned long long" to "long long"
//-ap end
typedef long long DurationT;
const DurationT Billion=1000000000LL;
#endif

#include "ConditionsDB/SimpleTime.h"

class SimpleTimeInterval;

class SimpleTimeDuration {
 public:
  // Construction/Destruction
  SimpleTimeDuration(DurationT nsec=Billion) {durationval=nsec;};
  SimpleTimeDuration(const SimpleTimeDuration &d) {durationval=d.duration();};
  SimpleTimeDuration(const SimpleTime &, const SimpleTime &);
  SimpleTimeDuration(const SimpleTimeInterval &);
  ~SimpleTimeDuration() {};

  // Auxiliary functions
  DurationT duration() const;
  void setDuration(const SimpleTimeDuration &);
  void setDuration(const SimpleTimeInterval &);
  void setDuration(const SimpleTime &, const SimpleTime &);
  void setPlusInf();
  bool isPlusInf() const;

  // Comparison
  bool isShorterThan(const SimpleTimeDuration &) const;

  // Operators
  void operator  = (const SimpleTimeDuration &);
  void operator += (const SimpleTimeDuration &);
  void operator -= (const SimpleTimeDuration &);
  void operator *= (const double &);
  void operator /= (const double &);

 private:
  DurationT durationval;
};

bool operator < (const SimpleTimeDuration &, const SimpleTimeDuration &);
bool operator > (const SimpleTimeDuration &, const SimpleTimeDuration &);
SimpleTimeDuration operator * (const SimpleTimeDuration &, const double &);
SimpleTimeDuration operator / (const SimpleTimeDuration &, const double &);
double operator / (const SimpleTimeDuration &, const SimpleTimeDuration &);
SimpleTimeDuration operator + (const SimpleTimeDuration &,
			       const SimpleTimeDuration &);
SimpleTimeDuration operator - (const SimpleTimeDuration &,
			       const SimpleTimeDuration &);
// Streaming __int64 is not supported on NT
#ifndef _WIN32
AIDA_STD::ostream & operator << (AIDA_STD::ostream & out, const SimpleTimeDuration &);
#endif

#endif
