#ifndef CondDBMySQL_SCRAM
#include "ConditionsDB/SimpleTimeDuration.h"
#include "ConditionsDB/SimpleTimeInterval.h"
#include <math.h>
#include <limits.h>


// DinoFM: use ceil() rather than rint()
#ifdef __i386__
//-ap this seems not to be defined everywhere !??!
//extern "C" { double ceil(double ); }
#endif

// DinoFM: Win32 does not support properly unsigned 64 bit integers
#ifdef _WIN32
// 2^63-1 ("infinity"), 292 years
const DurationT SIMPLEDURATION_MAX = 9223372036854775807;  
#else
// 2^64-1 ("infinity"), 584 years
//ULL  const DurationT SIMPLEDURATION_MAX = 18446744073709551615LL;    // 2^64-1
const DurationT SIMPLEDURATION_MAX = 9223372036854775807LL;     // 2^63-1 ("infinity"), sometime in 2262
#endif
///////////////
// Construction
///////////////


SimpleTimeDuration::SimpleTimeDuration(const SimpleTime & start,
				       const SimpleTime & end) {
  setDuration(start, end);
}

SimpleTimeDuration::SimpleTimeDuration(const SimpleTimeInterval & i) {
  setDuration(i);
}

void SimpleTimeDuration::setDuration(const SimpleTimeDuration & d) {
  durationval=d.duration();
}

void SimpleTimeDuration::setDuration(const SimpleTimeInterval & i) {
  durationval=i.end().toTimeT()-i.start().toTimeT();
}

void SimpleTimeDuration::setDuration(const SimpleTime & start,
				     const SimpleTime & end) {
  // set Duration from two points in time:
  // Take the difference between them. Always nonnegative.

  // Reverse the interval (and call the fcn again) if end is before start
  if (end.isBefore(start)) {
    setDuration(end, start);
    return;
  } 

  // If either of the points is inf and they don't coincide,
  // set the duration to inf.
  if (end.isPlusInf()) {
    if (start.isPlusInf()) {
      durationval=0;
    } else {
      setPlusInf();
    }
    return;
  }
  if (start.isMinusInf()) {
    if (end.isMinusInf()) {
      durationval=0;
    } else {
      setPlusInf();
    }
    return;
  }

  // The standard case: Finite points, start before end.
  durationval=end.toTimeT()-start.toTimeT();
}

/////////////////
// Aux. functions
/////////////////

DurationT SimpleTimeDuration::duration() const {
  return durationval;
}

void SimpleTimeDuration::setPlusInf() {
  durationval = SIMPLEDURATION_MAX;
}

bool SimpleTimeDuration::isPlusInf() const {
  return (durationval == SIMPLEDURATION_MAX);
}

/////////////
// Comparison
/////////////

bool SimpleTimeDuration::
isShorterThan(const SimpleTimeDuration & comparison) const {
  return (durationval < comparison.duration());
}

////////////
// Operators
////////////

void SimpleTimeDuration::operator = (const SimpleTimeDuration & d) {
  setDuration(d);
}

void SimpleTimeDuration::operator += (const SimpleTimeDuration & d) {
  if (isPlusInf()) { // Leave unchanged if infinity
    return;
  } else if (d.isPlusInf()) { // Go to inf if inf is added
    setPlusInf();
  } else if(durationval/2 + d.duration()/2 > SIMPLEDURATION_MAX/2) { // Overflow
    setPlusInf();
  } else { // Normal case
    durationval+=d.duration();
  }
}

void SimpleTimeDuration::operator -= (const SimpleTimeDuration & d) {
  if (isPlusInf()) { // Leave unchanged if inf, even if inf is subtracted
    return;
  } else if (d.isPlusInf()) { // Infinity subtracted from finite value
    durationval=0; // This is not really the best solution!
    // TODO: Throw an exception
  } else if (isShorterThan(d)) { // The subtractor is larger than the operand
    durationval=0; // TODO: Throw an exception
  } else {
    durationval-=d.duration(); // Normal case
  }
}

void SimpleTimeDuration::operator *= (const double & factor) {
  if (factor < 0) {
    durationval=0;
    return; // No negative values allowed! TODO: Throw an exception
  }
  if (isPlusInf()) { // Leave unchanged if infinity
    return;
  } else if (SIMPLEDURATION_MAX/factor < (double)durationval) { // Overflow!
    setPlusInf();
  } else { // Normal case
    durationval = (DurationT) ceil(durationval*factor);
  }
}

void SimpleTimeDuration::operator /= (const double & denom) {
  if (denom < 0) {
    durationval=0;
    return; // No negative values allowed! TODO: Throw an exception
  }
  if (isPlusInf()) { // Leave unchanged if infinity
    return;
  } else if ((denom < 1) && (durationval > denom*SIMPLEDURATION_MAX)) {
    setPlusInf(); // Overflow! TODO: Throw an exception
  } else {
    durationval = (DurationT) ceil(durationval/denom);
  }
}

///////////////////
// Global operators
///////////////////

SimpleTimeDuration operator * (const SimpleTimeDuration & d,
			       const double & factor) {
  SimpleTimeDuration retval(d);
  retval *= factor;
  return retval;
}

SimpleTimeDuration operator / (const SimpleTimeDuration & d,
			       const double & denom) {
  SimpleTimeDuration retval(d);
  retval /= denom;
  return retval;
}

double operator / (const SimpleTimeDuration & d,
		   const SimpleTimeDuration & denom) {
  if (d.isPlusInf()) {
    return SIMPLEDURATION_MAX;
  } else if (denom.isPlusInf()) {
    return .0;
  } else {
    return (double)d.duration()/(double)denom.duration();
  }
}

SimpleTimeDuration operator + (const SimpleTimeDuration & arg1,
			       const SimpleTimeDuration & arg2) {
  SimpleTimeDuration retval(arg1);
  retval += arg2;
  return retval;
}

SimpleTimeDuration operator - (const SimpleTimeDuration & arg1,
			       const SimpleTimeDuration & arg2) {
  SimpleTimeDuration retval(arg1);
  retval -= arg2;
  return retval;
}

bool operator < (const SimpleTimeDuration & left,
		 const SimpleTimeDuration & right) {
  return (left.duration() < right.duration());
}

bool operator > (const SimpleTimeDuration & left,
		 const SimpleTimeDuration & right) {
  return (left.duration() > right.duration());
}

#ifndef _WIN32
AIDA_STD::ostream & operator << (AIDA_STD::ostream & out, const SimpleTimeDuration & d) {
  out << d.duration();
  return out;
}
#endif
#endif // ifndef CondDBMySQL_SCRAM
