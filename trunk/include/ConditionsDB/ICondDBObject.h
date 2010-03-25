/* $Id: ICondDBObject.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef ICondDBObject_h
#define ICondDBObject_h

//#include "HepUtilities/SimpleTime.h"
#include "ConditionsDB/SimpleTime.h"


#include "ConditionsDB/CondDBKey.h"
#include "ConditionsDB/CondDBException.h"
#include "ConditionsDB/ICondDBStreamable.h"

#include <list>


/**
 * Interface to a CondObject.
 */

class ICondDBObject {

public:

  /// Destructors.
  virtual ~ICondDBObject() {}

  // Get methods

  /// Returns the starting validity of the interval.
  virtual CondDBKey validSince() const = 0;

  /// Returns the end of validity of the interval.
  virtual CondDBKey validTill() const = 0;

  /// Returns true if since <= point < till .
  virtual bool contains(const CondDBKey& point) const = 0;

  /**
   * Returns true if this object has no associated data.
   * If the value is true, this object does not represent
   * an actual condition value inserted by the user;
   * it does simply represent a hole, where no actual data are present.
   * The values Since and Till can be used to get the width of
   * this hole.
   */
  virtual bool isNullInterval() const = 0;
  
  /**
   * Returns the actual data associated to this CondObject.
   * This method has to be used when simple string are stored as
   * condition value.
   */
  virtual void data(std::string& dataValue) const = 0;

  /**
   * Returns the actual data associated to this CondObject.
   * This method has to be used when generic types are stored
   * as condition value.
   *
   * The conversion could generate an exception (in principle)
   */
  virtual void data(ICondDBStreamable &s) const
    throw(CondDBException) = 0;

  /**
   * Returns the insertion time of this object.
   */  
  virtual void insertionTime( SimpleTime& insTime ) const = 0;

  /**
   * Returns the layer number of this object.
   * layerNum=0 for the initial empty interval.
   */
  virtual long layer() const = 0;

  /// Returns the description provided for this object.
  virtual void description(std::string& descr) const = 0;

  /* Not yet implemented
  /// Returns the list of all associated tags.
  virtual void allTags(std::list<std::string>::iterator& assocTags) const = 0;
  */
  
};

#endif // ICondDBObject_h
