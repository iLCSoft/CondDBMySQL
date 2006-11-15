/* $Id: CondDBException.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef CondDBException_h
#define CondDBException_h

#include <stdexcept>

/**
 * Exception used for error handling.
 */

// WARN: ErrorCode values to be defined, will set 0 in the
//       intermediate time

class CondDBException : public std::runtime_error {

public:
  
  CondDBException(const std::string& theMessage,
		  unsigned long theErrorCode) : 
    runtime_error(theMessage), errorCode(theErrorCode) { }

  /// Returns the error message.
  virtual const char*     getMessage() const { return what(); }

  /// Returns the error code.
  virtual unsigned long   getErrorCode() const { return errorCode; }

private:

  unsigned long errorCode;
};


#endif   // CondDBException_h



