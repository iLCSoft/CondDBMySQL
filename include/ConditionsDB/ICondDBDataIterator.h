// ICondDBDataIterator.h -- simple iterator for browsing through versions or time sequences
// created on Thu Apr 26 11:25:10 2001
//
// $Id: ICondDBDataIterator.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $
//
#ifndef ICondDBDataIterator_h
#define ICondDBDataIterator_h

#include "ConditionsDB/ICondDBObject.h"


/**
 * Iterator interface wich allows to iterate over the conditions
 * objects in a folder.
 */

class ICondDBDataIterator {

public:

  /// Virtual destructor
  virtual ~ICondDBDataIterator(){};

  /// Testing

  /**
   * Tests whether this iterator has any elements after its current location.
   *
   * Returns true is the iteration set has at least one element after the current location;
   * otherwise false.
   */
  virtual bool hasNext() const = 0;

  /**
   * Tests whether this iterator has any elements before its current location.
   *
   * Returns true is the iteration set has at least one element before the current location;
   * otherwise false.
   */
  virtual bool hasPrevious() const = 0;

  /// Finding elements

  /**
   * Finds the current object in this iterator's iteration set.
   *
   * Returns a pointer to this iterator's current element, or a null pointer
   * if there is no current element (because the iterator is positioned before
   * the first element or after the last element).
   *
   * Note: the object pointed by ICondDBObject* has to be deleted by the user
   * at the end of its usage.
   */
  virtual ICondDBObject* current()
     = 0;

  /**
   * Finds the previous object in this iterator's iteration set.
   *
   * Returns a pointer to the previous element in this iteration set, or a null pointer
   * if this iterator is positioned before the first element in the iteration set.
   *
   * Note: the object pointed by ICondDBObject* has to be deleted by the user
   * at the end of its usage.
   */
  virtual ICondDBObject* previous()
     = 0;

  /**
   * Finds the next object in this iterator's iteration set.
   *
   * Returns a pointer to the previous element in this iteration set, or a null pointer
   * if this iterator is positioned after the last element in the iteration set.
   *
   * Note: the object pointed by ICondDBObject* has to be deleted by the user
   * at the end of its usage.
   */
  virtual ICondDBObject* next()
     = 0;

  /**
   * Positions the iterator on the last object in this iterator's iteration set.
   */
  virtual void goToLast() = 0;

  /**
   * Positions the iterator on the first object in this iterator's iteration set.
   */
  virtual void goToFirst() = 0;

};


#endif /* ICondDBDataIterator_h */
