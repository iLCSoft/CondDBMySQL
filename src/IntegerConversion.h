#ifndef CondDBMySQL_SCRAM
///////////////////////////////////////////////// 
// IntegerConversion.h 
// 
// Purpose: IntegerConversion provides a set of 
// functions capable of converting between 
// strings and arbitrary integer types. 
// 
// Author: Erik Zeitler Date: 2000-09-06 
// 
/////////////////////////////////////////////////

#include <iostream> 
#include <string> 

// Prevents the compiler from generating several copies of 
// methods during the precompiling stage, which leads to longer 
// compile time. 

#if defined(__GNUC__) 
#pragma interface 
#endif

template <class Integer> class IntegerConversion {

public:

  // From integer to string 
  AIDA_STD::string itostr(Integer n); 
  // From string to integer 
  Integer strtoi(const AIDA_STD::string &s); 

private:
  // Recursive function, used internally 
  // for conversion from string to integer. 
  Integer rec_strtoi(const AIDA_STD::string &s); 
};

template <class Integer> class UnsignedIntegerConversion {

public:

  // From integer to string 
  AIDA_STD::string itostr(Integer n); 
  // From string to integer 
  Integer strtoi(const AIDA_STD::string &s); 

private:
  // Recursive function, used internally 
  // for conversion from string to integer. 
  Integer rec_strtoi(const AIDA_STD::string &s); 
};

#if defined(__GNUC__) 
#pragma implementation 
#endif

template <class Integer> AIDA_STD::string IntegerConversion<Integer>:: itostr(Integer n) {

  AIDA_STD::string retval; 
  bool minus=0; 
  if (n < 0) {
    n=-n; 
    minus=1; 
  }

  do {
    retval.insert((AIDA_STD::string::size_type)0,1,(char)('0'+n%10)); 
  } while ((n/=10)>0); 

  if (minus)
    retval.insert((AIDA_STD::string::size_type)0,1,'-'); 

  return retval; 
}

template <class Integer> Integer IntegerConversion<Integer>:: strtoi(const AIDA_STD::string & s) {

  if (s.substr(0,1) == "-") {

    return -rec_strtoi(s.substr(1)); // Remove minus sign from string 
  } else {

    return rec_strtoi(s); 
  } 
}

template <class Integer> Integer IntegerConversion<Integer>:: rec_strtoi(const AIDA_STD::string & s) {

  if (s.length()) {

    return 10*rec_strtoi(s.substr(0,s.length()-1)) + s.c_str()[s.length()-1]-'0'; 
  } else {

    return 0; 
  } 
}

// --------------------------------------------------------------------------------

template <class Integer> AIDA_STD::string UnsignedIntegerConversion<Integer>:: itostr(Integer n) {

  AIDA_STD::string retval; 
  do {
    retval.insert((AIDA_STD::string::size_type)0,1,(char)('0'+n%10)); 
  } while ((n/=10)>0); 

  return retval;
}

template <class Integer> Integer UnsignedIntegerConversion<Integer>:: strtoi(const AIDA_STD::string & s) {

  if (s.substr(0,1) == "-") {

    return -rec_strtoi(s.substr(1)); // Remove minus sign from string 
  } else {

    return rec_strtoi(s); 
  } 
}

template <class Integer> Integer UnsignedIntegerConversion<Integer>:: rec_strtoi(const AIDA_STD::string & s) {

  if (s.length()) {

    return 10*rec_strtoi(s.substr(0,s.length()-1)) + s.c_str()[s.length()-1]-'0'; 
  } else {

    return 0; 
  } 
}

#else
#error This version of IntegerConversion.h should not be used by SCRAM
#endif // ifndef CondDBMySQL_SCRAM
