/* $Id: ICondDBLogMgr.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef ICondDBLogMgr_h
#define ICondDBLogMgr_h

#include "ConditionsDB/CondDBException.h"

/**
 * Interface to manage the log.
 */

class ICondDBLogMgr {

public:

  /// Destructor
  virtual ~ICondDBLogMgr() {}

  // Creators

  /// Activate the logging.
  virtual void activate( ) 
    throw(CondDBException) = 0;

  /// Deactivate the logging.
  virtual void deactivate( ) 
    throw(CondDBException) = 0;

  /// Dump the log content; actual signature to be defined.
  virtual void dump( ) 
    throw(CondDBException) = 0;

};

#endif // ICondDBLogMgr_h
