/* $Id: ICondDBDataAccess.h,v 1.1 2006-11-15 14:03:15 poeschl Exp $ */
#ifndef ICondDBDataAccess_h
#define ICondDBDataAccess_h

#include "ConditionsDB/ICondDBBasicDataAccess.h"
#include "ConditionsDB/ICondDBTable.h"
#include "ConditionsDB/ICondDBTimePath.h"

/**
 * Extended interface to the conditions database.
 * Used to store, retrieve and tag the conditions data.
 * All methods throw exception CondDBException.
 */

class ICondDBDataAccess : virtual public ICondDBBasicDataAccess {

public:

  /// Destructor.
  virtual ~ICondDBDataAccess() {}

  // Creators

  /**
   * Store table in the given folder
   * @param folderName Folder in which the object is to be stored in
   * @param table The table to be stored
   */

  virtual void storeCondDBObject( const string& folderName,
                                  ICondDBTable *table)
     = 0;

  // Redeclare ICondDBBasicDataAccess method to prevent it from being "hidden"
  virtual void storeCondDBObject( const std::string& folder,
				  ICondDBObject*     CondObject )
     = 0;

  // Queries

  /**
   * Find object for the given point in time (latest version or tag)
   * @param table The found data will be stored in this table
   * @param folderName The folder in which we are searching
   * @param point The point in time to be found
   * @param id The Id to search for. Only for tables with ID
   * @param selection Optional. A selection clause which will be inserted in the query as WHERE clause
   * @param nullValues Optional. A pointer to a vector of string which contains the NULL values for the table
   * @param tagName Optional. Find for tags
   */

  virtual void findCondDBObject( ICondDBTable *table,
				 const std::string&  folderName,
				 const CondDBKey&    point,
				 const std::string&  id = "",
				 const std::string&  selection = "",
				 const std::vector <std::string> *nullValues = 0,
				 std::string      tagName = "") const
       = 0;

  // Redeclare ICondDBBasicDataAccess method to prevent it from being "hidden"
  virtual void findCondDBObject( ICondDBObject*&    oblock,
				 const std::string& folder,
				 const CondDBKey&   point,
				 std::string        tagName = "" ) const
     = 0;

  // Browsing

  /**
   * Browse at point for the given folder (all versions)
   * @param table A ICondDBTable in which the search result will be stored
   * @param folderName Name of the folder to be searched in
   * @param point Time to be searched for
   * @param selection Optional. A selection clause which will be inserted in the query as WHERE clause
   * @param nullValues Optional. A pointer to a vector of string which contains the NULL values for the table
   */

  virtual void browseObjectsAtPoint( ICondDBTable *table,
				     const std::string&    folderName,
				     const CondDBKey&      point,
				     const std::string& selection = "",
				     const std::vector <std::string> *nullValues = 0) const
       = 0;

  // Redeclare ICondDBBasicDataAccess method to prevent it from being "hidden"
  virtual void
    browseObjectsAtPoint( ICondDBDataIterator*& iterObjects,
			  const std::string&    folder,
			  const CondDBKey&      point ) const
     = 0;

  /**
   * Browse tags for the given folder
   * @param table A ICondDBTable in which the search result will be stored
   * @param folderName Name of the folder to be searched in
   * @param tagName Optional. Empty tag means searching in HEAD, otherwise the TAG to be searched for
   * @param selection Optional. A selection clause which will be inserted in the query as WHERE clause
   * @param nullValues Optional. A pointer to a vector of string which contains the NULL values for the table
   */

  virtual void browseObjectsInTag( ICondDBTable         *table,
				   const std::string&    folderName,
				   const std::string&    tagName = "",
				   const std::string&    selection = "",
				   const std::vector <std::string> *nullValues = 0) const
      = 0;

  // Redeclare ICondDBBasicDataAccess method to prevent it from being "hidden"
  virtual void
  browseObjectsInTag( ICondDBDataIterator*& iterObjects,
		      const std::string&    folder,
		      std::string           tagName = "" ) const
     = 0;

  /**
   * Browse all for the given folder
   * @param table A ICondDBTable in which the search result will be stored
   * @param folderName Name of the folder to be searched in
   * @param selection Optional. A selection clause which will be inserted in the query as WHERE clause
   * @param nullValues Optional. A pointer to a vector of string which contains the NULL values for the table
   */

  virtual void browseAllObjects( ICondDBTable         *table,
				 const std::string&    folderName,
				 const std::string&    selection = "",
				 const std::vector <std::string> *nullValues = 0) const
       = 0;

  // Redeclare ICondDBBasicDataAccess method to prevent it from being "hidden"
  virtual void
  browseAllObjects( ICondDBDataIterator*& iterObjects,
		    const std::string&    folder ) const
     = 0;

  /**
   * Browse folder for a given time interval
   * @param folder The folder to be browsed
   * @param iterObjects An iterator to the objects in which the data found will be stored in this table
   * @param since The lower limit of the time interval
   * @param till The upper limit of the time interval
   */

  virtual void browseHistory (ICondDBDataIterator*& iterObjects,
			      const std::string&    folderName,
			      const CondDBKey since,
			      const CondDBKey till,
			      std::string tagName = "") const
       = 0;

  /**
   * Browse folder for a given time interval
   * @param folder The folder to be browsed
   * @param table Data found will be stored in this table
   * @param since The lower limit of the time interval
   * @param till The upper limit of the time interval
   * @param id The ID to search for. Only for tables with ID
   * @param selection Optional. A selection clause which will be inserted in the query as WHERE clause
   * @param nullValues Optional. A pointer to a vector of string which contains the NULL values for the table
   * @param tagName Optional. Find for tags
   */

  virtual void browseHistory(const string& folder,
			     ICondDBTable *table,
			     const CondDBKey& since,
			     const CondDBKey& till,
			     const std::string& id = "",
			     const std::string& selection = "",
			     const std::vector <std::string> *nullValues = 0,
			     string tagName = "") const
       = 0;

  /**
   * Browse folder for Ids with optional tag
   * @param path A container which holds the result of the search
   * @param folder The folder name in which to search
   * @param tagName Optional. Search for the given tag. If no tag is give HEAD is used
   */

  virtual void browseId (ICondDBTimePath& path,
			 std::string folder,
			 std::string tagName = "") const
       = 0;

  /**
   * Build tag entries in database from the ICondDBTimePath container
   * @param folder The folder name to be associated to the tag
   * @param tagName The tagName to be used
   * @param path The container
   */

  virtual void tagFromTimePath (std::string folder, std::string tagName, ICondDBTimePath& path) const
       = 0;

  /**
   * Get the table schema
   * @param folder The folder whose schema we want
   * @param table An empty table with the same schema of the given folder
   */

  virtual void getTableSchema (std::string folder, ICondDBTable *table)
       = 0;

};

#endif // ICondDBDataAccess_h
