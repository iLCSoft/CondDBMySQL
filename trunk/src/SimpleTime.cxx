#ifndef CondDBMySQL_SCRAM
#include "ConditionsDB/SimpleTime.h"
#include "ConditionsDB/SimpleTimeDuration.h"
#include "IntegerConversion.h"

#include <time.h>

#define DEBUG

//-ap these give warnings as enums, so we simply move them here ...

// DinoFM: Win32 does not support properly unsigned 64 bit integers
#ifdef _WIN32
const TimeT SIMPLETIME_BILLION     = 1000000000;              // #nanoseconds per second
const TimeT SIMPLETIME_UNIX_ORIGIN = 2208988800000000000;     // 1.1.1970
const TimeT SIMPLETIME_UNIX_MAX    = 4356476047000000000;     // 19.01.2038
const TimeT SIMPLETIME_MIN         = -9223372036854775808;    // -2^63
const TimeT SIMPLETIME_MAX         = 9223372036854775807;     // 2^63-1 ("infinity"), sometime in 2262
#else
const TimeT SIMPLETIME_BILLION     = 1000000000LL;              // #nanoseconds per second
const TimeT SIMPLETIME_UNIX_ORIGIN = 2208988800000000000LL;     // 1.1.1970
const TimeT SIMPLETIME_UNIX_MAX    = 4356476047000000000LL;     // 19.01.2038
//-ULL const TimeT SIMPLETIME_MAX         = 18446744073709551615LL;    // 2^64-1 ("infinity"), 02.07.2484
const TimeT SIMPLETIME_MIN         = -9223372036854775808LL;    // -2^63
const TimeT SIMPLETIME_MAX         = 9223372036854775807LL;     // 2^63-1 ("infinity"), sometime in 2262
#endif


///////////////
// Constructors
///////////////

SimpleTime::SimpleTime() {
  // default: now.
  fromUnixTime(::time(0));
}



SimpleTime::SimpleTime(const time_t &unix_time) {
  fromUnixTime(unix_time);
}

SimpleTime::SimpleTime(const TimeT &TimeT_time) {
  timeval = TimeT_time;
}

SimpleTime::SimpleTime(const unsigned short year,
		       const unsigned short month,
		       const unsigned short day,
		       const unsigned short hour,
		       const unsigned short min,
		       const unsigned short sec,
		       const unsigned long ns,
		       const bool plusInf,
		       const bool minusInf) {
  fromCalendarTime(year, month, day,
		   hour, min, sec, ns, plusInf, minusInf);
}

SimpleTime::SimpleTime(const timeAndDate_t & in) {
  fromCalendarTime(in);
}

SimpleTime::SimpleTime(const AIDA_STD::string & netloggerstring) {
  fromNetLogger(netloggerstring);
}

////////////
// Operators
////////////
void SimpleTime::operator = (const SimpleTime & t) {
  timeval=t.toTimeT();
}

void SimpleTime::operator = (const TimeT & t) {
  timeval=t;
}

void SimpleTime::operator += (const SimpleTimeDuration & d) {
  if (isMinusInf() || isPlusInf()) { // Leave unchanged if at infinity
    return;
  } else if (d.isPlusInf()) { // Set to inf if inf added
    setPlusInf();
  } else {
    timeval+=d.duration();
  }
}

void SimpleTime::operator -= (const SimpleTimeDuration & d) {
  if (isMinusInf() || isPlusInf()) { // Leave unchanged if at infinity
    return;
  } else if (d.isPlusInf()) { // Set to -inf if inf subtracted
    setMinusInf();
  } else {
    timeval-=d.duration();
  }
}

#ifndef _WIN32
AIDA_STD::ostream & operator << (AIDA_STD::ostream & out, const SimpleTime & t) {
  //  out << t.toTimeT();
  if (t.isPlusInf()) {
    out << "+Inf";
    return out;
  }
  if (t.isMinusInf()) {
    out << "-Inf";
    return out;
  }
  const char * mon[] = { "NONE", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" } ;
  out << t.year()       << "-"
      << mon[t.month()] << "-"
      << t.day()        << " "
      << t.hour()       << ":"
      << t.minute()     << ":"
      << t.second()     << "."
      << t.nanosec()    << " (GMT)";
  return out;
}
#endif

////////////
// Aux. fcns
////////////
TimeT SimpleTime::time() const {
  std::cerr << "SimpleTime::time()> use of this method is deprecated, use toTimeT() instead" << std::endl;
  return timeval;
}

void SimpleTime::setPlusInf() {
  timeval = SIMPLETIME_MAX;
}

void SimpleTime::setMinusInf() {
  timeval = SIMPLETIME_MIN;
}

bool SimpleTime::isPlusInf() const {
  return (timeval == SIMPLETIME_MAX);
}

bool SimpleTime::isMinusInf() const {
  return (timeval == SIMPLETIME_MIN);
}

//////////////////
// Comparison fcns
//////////////////
bool SimpleTime::isBefore(const SimpleTime& comparison) const {
  return (timeval < comparison.toTimeT());
}

//////////////////
// Conversion fcns
//////////////////

void SimpleTime::fromNetLogger(const AIDA_STD::string & timestring) {
  timeAndDate_t timestruct;
  timestruct.plusInf=0;
  timestruct.minusInf=0;

  if (timestring=="+inf") {
    timestruct.plusInf=1;
  } else if (timestring=="-inf") {
    timestruct.minusInf=1;
  } else {
    UnsignedIntegerConversion<unsigned short> ushortConv;
    UnsignedIntegerConversion<unsigned long> ulongConv;
    timestruct.year = ushortConv.strtoi(timestring.substr(0,4));
    timestruct.month= ushortConv.strtoi(timestring.substr(4,2));
    timestruct.day  = ushortConv.strtoi(timestring.substr(6,2));
    timestruct.hour = ushortConv.strtoi(timestring.substr(8,2));
    timestruct.min  = ushortConv.strtoi(timestring.substr(10,2));
    timestruct.sec  = ushortConv.strtoi(timestring.substr(12,2));
    timestruct.ns   = ulongConv.strtoi(timestring.substr(15));
  }
  fromCalendarTime(timestruct);
}

AIDA_STD::string SimpleTime::toNetLogger() const {
  // Convert the internal representation to a human readable
  // (and sortable) string.

  // Infinity handling
  if (isPlusInf()) {
    return "+inf";
  }
  if (isMinusInf()) {
    return "-inf";
  }

  AIDA_STD::string timestring;
  timeAndDate_t calendarTime=toCalendarTime();
  UnsignedIntegerConversion<unsigned short> ushortConv;
  UnsignedIntegerConversion<unsigned long>  ulongConv;

  // When converting to strings, insert possibly leading zeros
  AIDA_STD::string tmp_string=ushortConv.itostr(calendarTime.year);
  while(tmp_string.length() < 4) // This should never be needed
    tmp_string.insert((AIDA_STD::string::size_type)0,1,(char)('0'));
  timestring=tmp_string;
  tmp_string=ushortConv.itostr(calendarTime.month);
  while(tmp_string.length() < 2)
    tmp_string.insert((AIDA_STD::string::size_type)0,1,(char)('0'));
  timestring+=tmp_string;
  tmp_string=ushortConv.itostr(calendarTime.day);
  while(tmp_string.length() < 2)
    tmp_string.insert((AIDA_STD::string::size_type)0,1,(char)('0'));
  timestring+=tmp_string;
  tmp_string=ushortConv.itostr(calendarTime.hour);
  while(tmp_string.length() < 2)
    tmp_string.insert((AIDA_STD::string::size_type)0,1,(char)('0'));
  timestring+=tmp_string;
  tmp_string=ushortConv.itostr(calendarTime.min);
  while(tmp_string.length() < 2)
    tmp_string.insert((AIDA_STD::string::size_type)0,1,(char)('0'));
  timestring+=tmp_string;
  tmp_string=ushortConv.itostr(calendarTime.sec);
  while(tmp_string.length() < 2)
    tmp_string.insert((AIDA_STD::string::size_type)0,1,(char)('0'));
  timestring+=tmp_string;
  tmp_string=ulongConv.itostr(calendarTime.ns);
  while (tmp_string.length() < 9)
    tmp_string.insert((AIDA_STD::string::size_type)0,1,(char)('0'));
  timestring+= "." + tmp_string;
  return timestring;
}

TimeT SimpleTime::toTimeT() const {
  return timeval;
}

time_t SimpleTime::toUnixTime() const {
  if (isPlusInf() || isMinusInf())
    return -1;
  if (timeval < SIMPLETIME_UNIX_ORIGIN || // Outside unix_time's bounds...
      timeval > SIMPLETIME_UNIX_MAX)
    return -1;
  else
    return (timeval-SIMPLETIME_UNIX_ORIGIN)/SIMPLETIME_BILLION;
}

void SimpleTime::fromUnixTime(const time_t &unix_time) {
  timeval=SIMPLETIME_UNIX_ORIGIN + unix_time*SIMPLETIME_BILLION;
}

void SimpleTime::
fromCalendarTime(const unsigned short in_year,
		 const unsigned short in_month,
		 const unsigned short in_day,
		 const unsigned short in_hour,
		 const unsigned short in_min,
		 const unsigned short in_sec,
		 const unsigned long in_ns,
		 const bool plusInf,
		 const bool minusInf) {
  timeAndDate_t timestruct;
  timestruct.year =in_year;
  timestruct.month=in_month;
  timestruct.day  =in_day;
  timestruct.hour =in_hour;
  timestruct.min  =in_min;
  timestruct.sec  =in_sec;
  timestruct.ns   =in_ns;
  timestruct.plusInf=plusInf;
  timestruct.minusInf=minusInf;
  fromCalendarTime(timestruct);
}

void SimpleTime:: fromCalendarTime(const timeAndDate_t & in) {
  // fromCalendarTime converts an instance of timeAndDate_t into the 
  // internal representation.

  // Infinity is simple... We don't care about
  // the rest of the struct's contents.
  if (in.plusInf) {
    setPlusInf();
    return;
  }
  if (in.minusInf) {
    setMinusInf();
    return;
  }

  // Simple error check on the years. TODO: Throw an exception.
  if ((in.year < 1900) || (in.year > 2484)) {
    return;
  }
  unsigned short year=in.year-1900;
  TimeT days=0;

  // Calculate number of days elapsed since the origin (1900)
  days = year*365 + year/4
    - year/100 + (year+300)/400;

  // Add the proper number of days according to which month it is
  // The first month (January) is 1.
  switch(in.month) {
  case 1: days+=0;
    break;
  case 2: days+=31; // Jan
    break;
  case 3: days+=31+28; // Feb (Leap day treatment below!)
    break;
  case 4: days+=31+28+31; // Mar
    break;
  case 5: days+=31+28+31+30; // Apr
    break;
  case 6: days+=31+28+31+30+31; // May
    break;
  case 7: days+=31+28+31+30+31+30; // Jun
    break;
  case 8: days+=31+28+31+30+31+30+31; // Jul
    break;
  case 9: days+=31+28+31+30+31+30+31+31; // Aug
    break;
  case 10: days+=31+28+31+30+31+30+31+31+30; // Sep
    break;
  case 11: days+=31+28+31+30+31+30+31+31+30+31; // Oct
    break;
  case 12: days+=31+28+31+30+31+30+31+31+30+31+30; // Nov
    break;
  default:
    return; // Invalid number of months. Exit and leave the time untouched.
    // TODO: Throw an exception.
    break;
  }

  // The leap day (added above, in the years section) should be subtracted
  // for Jan..Feb (before Feb 29), if the year is
  // - every 4th year (leap year),
  // - although not each 100th year (1900, 2100, 2200, ...)
  // - but still every 400th year (2000, 2400, ...)
  if ((  year %  4 == 0) &&
      (!(year %100 == 0) || ((year+300)% 400 == 0)) &&
      (  in.month  < 3)) {
    days--;
  }

  // Add day of month. The first day in the month is 1 (subtract 1).
  days += in.day-1;

  timeval=(TimeT)
    ((((days*24) +
       in.hour)*60 +
      in.min)*60 +
     in.sec)*SIMPLETIME_BILLION +
    in.ns;
}

timeAndDate_t SimpleTime::toCalendarTime() const {
  // toCalendarTime converts the internal representation
  // to an instance of timeAndDate_t.

  timeAndDate_t retval;
  retval.plusInf=retval.minusInf=0;

  if (isPlusInf()) {
    retval.plusInf=1;
    return retval;
  }
  if (isMinusInf()) {
    retval.minusInf=1;
    return retval;
  }

  TimeT t1, t2;
  SimpleTime subtractor;
  int leap=0;

  // Year section
  // t1 = Approx. number of years
  t1=(TimeT)(timeval/(365.25637*24*60*60*SIMPLETIME_BILLION));
  // Count number of leap days
  t2=t1/4 - t1/100 + (t1+300)/400;
  t1=(timeval-t2*24*60*60*SIMPLETIME_BILLION)/
    (365*24*60*60*SIMPLETIME_BILLION); // Num. of years (more acc.)
  retval.year=t1+1900;

  // Month section
  subtractor.fromCalendarTime(retval.year, 1, 1, 0, 0, 0, 0);
  t1=timeval-subtractor.toTimeT(); // Number of ns in the month part
  // Is it leap year?
  if ((  (retval.year-1900) %  4 == 0) &&
      (!((retval.year-1900) %100 == 0) || (((retval.year-1900)+300)% 400 == 0)))
    leap=1;
  else
    leap=0;

  // Determine the month
  if (t1 < (31+28+leap+31+30+31+30+31+31+30+31+30+31)*
      24*60*60*SIMPLETIME_BILLION)
    t2=12;
  if (t1 < (31+28+leap+31+30+31+30+31+31+30+31+30)*
      24*60*60*SIMPLETIME_BILLION)
    t2=11;
  if (t1 < (31+28+leap+31+30+31+30+31+31+30+31)*
      24*60*60*SIMPLETIME_BILLION)
    t2=10;
  if (t1 < (31+28+leap+31+30+31+30+31+31+30)*
      24*60*60*SIMPLETIME_BILLION)
    t2=9;
  if (t1 < (31+28+leap+31+30+31+30+31+31)*
      24*60*60*SIMPLETIME_BILLION)
    t2=8;
  if (t1 < (31+28+leap+31+30+31+30+31)*
      24*60*60*SIMPLETIME_BILLION)
    t2=7;
  if (t1 < (31+28+leap+31+30+31+30)*
      24*60*60*SIMPLETIME_BILLION)
    t2=6;
  if (t1 < (31+28+leap+31+30+31)*
      24*60*60*SIMPLETIME_BILLION)
    t2=5;
  if (t1 < (31+28+leap+31+30)*
      24*60*60*SIMPLETIME_BILLION)
    t2=4;
  if (t1 < (31+28+leap+31)*
      24*60*60*SIMPLETIME_BILLION)
    t2=3;
  if (t1 < (31+28+leap)*
      24*60*60*SIMPLETIME_BILLION)
    t2=2;
  if (t1 < (31)*
      24*60*60*SIMPLETIME_BILLION)
    t2=1;
  retval.month=t2;

  // Days, hours, minutes, seconds and subseconds are left
  subtractor.fromCalendarTime(retval.year, retval.month, 1, 0, 0, 0, 0);
  t1=timeval-subtractor.toTimeT();
  retval.day=t1/(24*60*60*SIMPLETIME_BILLION) +1;
  t1%=24*60*60*SIMPLETIME_BILLION;
  retval.hour=t1/(60*60*SIMPLETIME_BILLION);
  t1%=60*60*SIMPLETIME_BILLION;
  retval.min=t1/(60*SIMPLETIME_BILLION);
  t1%=60*SIMPLETIME_BILLION;
  retval.sec=t1/SIMPLETIME_BILLION;
  t1%=SIMPLETIME_BILLION;
  retval.ns=t1;

  return retval;
}

unsigned short SimpleTime::year() const {
  return toCalendarTime().year;
}

unsigned short SimpleTime::month() const {
  return toCalendarTime().month;
}

unsigned short SimpleTime::day() const {
  return toCalendarTime().day;
}

unsigned short SimpleTime::hour() const {
  return toCalendarTime().hour;
}

unsigned short SimpleTime::minute() const {
  return toCalendarTime().min;
}

unsigned short SimpleTime::second() const {
  return toCalendarTime().sec;
}

unsigned long SimpleTime::nanosec() const {
  return (unsigned long) (toTimeT()%SIMPLETIME_BILLION);
}

SimpleTime operator + (const SimpleTime & arg1,
		       const SimpleTimeDuration & arg2) {
  SimpleTime retval(arg1);
  retval += arg2;
  return retval;
}

SimpleTime operator - (const SimpleTime & arg1,
		       const SimpleTimeDuration & arg2) {
  SimpleTime retval(arg1);
  retval -= arg2;
  return retval;
}
#endif // ifndef CondDBMySQL_SCRAM
