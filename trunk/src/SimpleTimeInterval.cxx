#ifndef CondDBMySQL_SCRAM
#include "ConditionsDB/SimpleTimeInterval.h"
#include "ConditionsDB/SimpleTimeDuration.h"

///////////////
// Construction
///////////////
SimpleTimeInterval::SimpleTimeInterval(const SimpleTimeInterval &i) {
  setInterval(i);
}

SimpleTimeInterval::SimpleTimeInterval(const SimpleTime & start,
				       const SimpleTime & end) {
  setInterval(start, end);
}

/////////////
// Comparison
/////////////
bool SimpleTimeInterval::
contains(const SimpleTimeInterval& comparison) const {
  return (startPoint.isBefore(comparison.start()) &&
	  comparison.end().isBefore(endPoint));
}

bool SimpleTimeInterval::contains(const SimpleTime& comparison) const {
  return (startPoint.isBefore(comparison) &&
	  comparison.isBefore(endPoint));
}

bool SimpleTimeInterval::
overlapsWith(const SimpleTimeInterval& comparison) const {
  return (!endsBefore(comparison) || !startsAfter(comparison));
}

bool SimpleTimeInterval::
endsBefore(const SimpleTimeInterval& comparison) const {
  return(endPoint.isBefore(comparison.start()));
}

bool SimpleTimeInterval::
startsAfter(const SimpleTimeInterval& comparison) const {
  return(comparison.end().isBefore(startPoint));
}

////////////
// Aux. fcns
////////////

void SimpleTimeInterval::setInterval(const SimpleTimeInterval & i) {
  startPoint=i.start();
  endPoint=i.end();
}

void SimpleTimeInterval::setInterval(const SimpleTime & start,
				     const SimpleTime & end) {
  if (end.isBefore(start)) {
    setInterval(end, start); // Reverse the order
    return;
  }
  setStart(start);
  setEnd(end);
}

SimpleTimeDuration SimpleTimeInterval::duration() const {
  SimpleTimeDuration d;
  d.setDuration(*this);
  return d;
}

SimpleTime SimpleTimeInterval::start() const {
  return startPoint;
}

SimpleTime SimpleTimeInterval::end() const {
  return endPoint;
}

void SimpleTimeInterval::setStart(const SimpleTime & newStart) {
  if (end().isBefore(newStart)) {
    return; // TODO: Throw an exception
  }
  startPoint = newStart;
}

void SimpleTimeInterval::setEnd(const SimpleTime & newEnd) {
  if (newEnd.isBefore(start())) {
    return; // TODO: Throw an exception
  }
  endPoint = newEnd;
}
#endif // ifndef CondDBMySQL_SCRAM
