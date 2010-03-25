/* $Id: ICondDBBasicDataAccess.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef ICondDBBasicDataAccess_h
#define ICondDBBasicDataAccess_h

#include "ConditionsDB/CondDBException.h"
#include "ConditionsDB/ICondDBObject.h"
#include "ConditionsDB/ICondDBDataIterator.h"

// Forward declaration
class ICondDBDataAccess;

/**
 * Basic interface to the conditions database.
 * Used to store, retrieve and tag the conditions data.
 * All methods throw exception CondDBException.
 */

class ICondDBBasicDataAccess {

public:

  /// Destructor.
  virtual ~ICondDBBasicDataAccess() {}

  // Creators

  /**
   * Stores the object in the specified CondFolder.
   * folder: the full path and name of a CondFolder
   *         e.g.: "/compass/calo/aaaa/temp"
   *
   * NOTES:
   * - the fullPathName folder must be precise, without repeated and unnecessary "/"
   *   otherwise the passed name will not be recognized and an exception will be thrown.
   *   A possible check and correction of the passed name is avoided for
   *   performance reasons.
   */
  virtual void storeCondDBObject( const std::string& folder,
				  ICondDBObject*     CondObject )
    throw(CondDBException) = 0;

  
  // Queries

  /**
   * Retrieves the CondObject valid for the given point
   * and associated to the given tag; otherwise the last
   * version (HEAD) will be retrieved.
   *
   * NOTES:
   * - the object pointed by ICondDBObject* has to be deleted by the
   *   user at the end of its usage.
   * - the fullPathName folder must be precise, without repeated and unnecessary "/"
   *   otherwise the passed name will not be recognized and an exception will be thrown.
   *   A possible check and correction of the passed name is avoided for
   *   performance reasons.
   * - tagName can identify either a local or a global tag
   */
  virtual void findCondDBObject( ICondDBObject*&    oblock,
				 const std::string& folder,
				 const CondDBKey&   point,
				 std::string        tagName = "" ) const
    throw(CondDBException) = 0;


  // Browsing

  /**
   * Returns an iterator over all CondObject valid for the given point in the given folder.
   * The iterator is positioned on the first object (layer=0).
   *
   * NOTES:
   * - the object pointed by ICondDBDataIterator* has to be deleted by the
   *   user at the end of its usage
   * - the fullPathName folder must be precise
   */
  virtual void
    browseObjectsAtPoint( ICondDBDataIterator*& iterObjects,
			  const std::string&    folder,
			  const CondDBKey&      point ) const
    throw(CondDBException) = 0;

  /**
   * Returns an iterator over all CondObject valid for the given folder and tag.
   * The iterator is positioned on the first object (since=-infinity).
   *
   * If tag is not specified, the latest version (HEAD) will be considered
   * (this is currently the only possibility, as the tag is not yet available)
   *
   * NOTES:
   * - the object pointed by ICondDBDataIterator* has to be deleted by the
   *   user at the end of its usage.
   * - the fullPathName folder must be precise
   * - tagName can identify either a local or a global tag
   */
  virtual void
  browseObjectsInTag( ICondDBDataIterator*& iterObjects,
		      const std::string&    folder,
		      std::string           tagName = "" ) const
    throw(CondDBException) = 0;

  /**
   * Returns an iterator over all CondObject stored in the given folder.
   * The iterator is positioned on the first object (since=-infinity).
   *
   * NOTES:
   * - the object pointed by ICondDBDataIterator* has to be deleted by the
   *   user at the end of its usage.
   * - the fullPathName folder must be precise
   */
  virtual void
  browseAllObjects( ICondDBDataIterator*& iterObjects,
		    const std::string&    folder ) const
    throw(CondDBException) = 0;

  
  /**
   * Dump the list of CondObject contained in the given folder.
   * If folder is not specified all CondFolder will be dumped.
   *
   * NOTES:
   * - the fullPathName folder must be precise
   */
  virtual void dump( std::string folder = "" ) const
    throw(CondDBException) = 0;

};

#endif // ICondDBBasicDataAccess_h





