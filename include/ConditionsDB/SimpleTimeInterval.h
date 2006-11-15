/////////////////////////////////////////////////
// simpleinterval.h
//
// Purpose: SimpleTimeInterval has two SimpleTime
// objects, startPoint being before endPoint.
// distance in time. Various comparison and
//  conversion functions are supported.
// 
// Author: Erik Zeitler  Date: 2000-09-06
//
/////////////////////////////////////////////////

#ifndef SIMPLE_INTERVAL_H
#define SIMPLE_INTERVAL_H
#include "ConditionsDB/SimpleTime.h"

class SimpleTimeDuration;

class SimpleTimeInterval {
 public:
  // Construction/Destruction
  SimpleTimeInterval() {};
  SimpleTimeInterval(const SimpleTimeInterval &i);
  SimpleTimeInterval(const SimpleTime & start,
		     const SimpleTime & end);
  ~SimpleTimeInterval() {};

  // Operators
  void operator = (const SimpleTimeInterval &);

  // Comparison
  // Other comparison operators not listed here,
  // like startsBefore and endsAfter, can be implemented on demand
  bool contains(const SimpleTimeInterval&) const;
  bool contains(const SimpleTime&) const;
  bool overlapsWith(const SimpleTimeInterval&) const;
  bool endsBefore(const SimpleTimeInterval&) const;
  bool startsAfter(const SimpleTimeInterval&) const;

  // Aux. methods
  SimpleTime start() const;
  SimpleTime end() const;
  void setStart(const SimpleTime &);
  void setEnd(const SimpleTime &);
  void setInterval(const SimpleTimeInterval &);
  void setInterval(const SimpleTime &, const SimpleTime &);
  SimpleTimeDuration duration() const;

 private:
  SimpleTime startPoint;
  SimpleTime endPoint;
};
#endif
