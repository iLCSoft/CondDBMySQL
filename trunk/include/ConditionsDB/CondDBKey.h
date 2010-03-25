/* $Id: CondDBKey.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef CondDBKey_h
#define CondDBKey_h

/// int64 type used for the evolution axis of the conditions data

/// Temporarly defined only for Linux; a portable solution will
/// be defined later on
/// Name space will be used

#ifdef __linux__
// tell gcc be silent about ANSI non-compliance of long long
# if defined (__GNUG__) 
__extension__ typedef long long CondDBKey;
__extension__ const CondDBKey CondDBminusInf = -9223372036854775807LL - 1; // - 2^63
__extension__ const CondDBKey CondDBplusInf  = 9223372036854775807LL;  //   2^63 -1

# else /* __GNU__ */
typedef long long CondDBKey;
const CondDBKey CondDBminusInf = -9223372036854775807LL - 1; // - 2^63
const CondDBKey CondDBplusInf  = 9223372036854775807LL;  //   2^63 -1
# endif /* __GNU__ */

#else /* __linux__ */

#ifdef __sun
typedef long long CondDBKey;
const CondDBKey CondDBminusInf = -9223372036854775807LL - 1; // - 2^63
const CondDBKey CondDBplusInf  = 9223372036854775807LL;  //   2^63 -1
#else /* __sun */

#ifdef __FreeBSD__
typedef long long CondDBKey;
const CondDBKey CondDBminusInf = -9223372036854775807LL - 1; // - 2^63
const CondDBKey CondDBplusInf  = 9223372036854775807LL;  //   2^63 -1
#define atoll(str)	strtoll(str, (char **)NULL, 10)
#else  /* __FreeBSD__ */

#ifdef __APPLE__
typedef long long CondDBKey;
const CondDBKey CondDBminusInf = -9223372036854775807LL - 1; // - 2^63
const CondDBKey CondDBplusInf  = 9223372036854775807LL;  //   2^63 -1
#else  /* __APPLE__ */

#ifdef WIN32

// This includes std::ostream and disables warning C4146
// ("unary minus operator applied to unsigned type, result still unsigned")
#include <string>

// The "long long" type is not defined on Windows: __int64 is the 
// equivalent type for a 64 bit signed integer.
typedef __int64 CondDBKey;

// The LL suffix to indicate a 64bit integer does not exist on Windows.
// In hex notation: -9223372036854775808=0x8000000000000000
// In hex notation:  9223372036854775807=0x7fffffffffffffff
const CondDBKey CondDBminusInf = -9223372036854775808; // - 2^63
const CondDBKey CondDBplusInf  =  9223372036854775807; //   2^63 -1

// Define standard output format of __int64 using useful info on printing 
// __int64 available at http://www.codeproject.com/useritems/precisetimer.asp
inline std::ostream& operator<<( std::ostream& os , const CondDBKey& key )
{ 
  static char buf[128];

  // Output in std::dec representation [could also use _i64toa(arg,buf,10)]
  //sprintf(buf,"%20.19I64d",key); // Fill with initial zeroes if needed
  sprintf(buf,"%-1.1I64d",key); // Remove initial zeroes and left-justify
  std::string darg(buf);

  // Output in std::hex representation [could also use _i64toa(arg,buf,16)] 
  //sprintf(buf,"%16.16I64x",key); // Fill with initial zeroes if needed
  sprintf(buf,"%1.1I64x",key); // Remove initial zeroes and left-justify
  std::string xarg(buf);

  // Return the desired representation
  int flg = os.flags();
  (flg & std::ios::hex) ? os<<xarg : os<<darg;
  return os;

}

#else /* WIN32 */

# error this platform is not supported yet

#endif /* WIN32 */

#endif /* __APPLE__ */

#endif /* __FreeBSD__ */

#endif /* __sun */

#endif /* __linux__ */

#endif // CondDBKey_h



