/*
 Copyright (C) 1999-2003 FreeDevices.org  geral@freedevices.org

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $Id: CondDBDataAccess.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBDataAccess.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//

#ifndef CONDDBDATAACCESS_HH
#define CONDDBDATAACCESS_HH

#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/ICondDBDataAccess.h"

class CondDBInterface;
class CondFolderMgr;
class CondDBTagMgr;

//#include "MySqlOnlineMgr.h"
//#include "MySqlObjectMgr.h"
//#include "MySqlDataMgr.h"

// part to allow the use of streams
#ifndef WIN32
//# include <ansidecl.h>
#ifdef __GNUC__
# if __GNUC__ < 3
#  include <strstream>
#  define CONDDBACCESSSTREAM strstream 
#  define USE_OLD_STREAMS
# else
#  include <sstream>
#  define CONDDBACCESSSTREAM std::ostringstream 
# endif
#else
#  include <sstream>
#  define CONDDBACCESSSTREAM std::ostringstream 
#endif
#endif

#ifdef WIN32
#  include <sstream>
#  define CONDDBACCESSSTREAM std::ostringstream
#endif

class MySqlDBMgr;

/*
 * MySQL based implementation of ICondDBDataAccess
 */

class CondDBDataAccess : public ICondDBDataAccess {

public:

    // Constructors
    CondDBDataAccess(MySqlDBMgr *dbMgr);

    // Destructor
    virtual ~CondDBDataAccess();

    virtual void init()
	throw(CondDBException);
    
    // Storing

    /**
     * Store object in the given folder
     * @param folderName Folder in which the object is to be stored in
     * @param CondObject The object to be stored
     */

    virtual void storeCondDBObject( const std::string& folderName,
				    ICondDBObject* CondObject )
	throw(CondDBException);

    /**
     * Store table in the given folder
     * @param folderName Folder in which the object is to be stored in
     * @param table The table to be stored
     */

    virtual void storeCondDBObject( const std::string& folderName, ICondDBTable *table)
	throw(CondDBException);

    // Queries
    
    /**
     * Find object for the given point in time
     * @param oblock The data found will be stored in this object
     * @param folderName The folder in which we are searching
     * @param point The point in time to be found
     * @param tagName Optional. Find for tags
     */

    virtual void findCondDBObject( ICondDBObject*&    oblock,
				   const std::string& folderName,
				   const CondDBKey&   point,
				   std::string        tagName = "" ) const
	throw(CondDBException);

    /**
     * Find object for the given point in time (latest version or tag)
     * @param table The found data will be stored in this table
     * @param folderName The folder in which we are searching
     * @param point The point in time to be found
     * @param tagName Optional. Find for tags
     */

    virtual void findCondDBObject( ICondDBTable *table,
				   const std::string& folderName,
				   const CondDBKey&   point,
				   const std::string& id = "",
				   const std::string&  selection = "",
				   const std::vector <std::string> *nullValues = 0,
				   std::string tagName = "") const
	throw(CondDBException);

    // Browsing
    
    /**
     * Browse for data at the given point in time (all versions)
     * @param iterObjects An object iterator to store the data found
     * @param folderName The folder in which we are searching
     * @param point The point in time for which we are searching
     */
    
    virtual void browseObjectsAtPoint( ICondDBDataIterator*& iterObjects,
				       const std::string&    folderName,
				       const CondDBKey&      point ) const
	throw(CondDBException);
    
    virtual void browseObjectsAtPoint( ICondDBTable *table,
				       const std::string&    folderName,
				       const CondDBKey&      point,
				       const std::string& selection = "",
				       const std::vector <std::string> *nullValues = 0) const
	throw(CondDBException);

    /**
     * Browse for data at the given point in time (for tag)
     * @param iterObjects An object iterator to store the data found
     * @param folderName The folder in which we are searching
     * @param tagName The tag for which we are searching, no tag means head
     */
 
    virtual void browseObjectsInTag( ICondDBDataIterator*& iterObjects,
				     const std::string&    folderName,
				     std::string           tagName = "" ) const
	throw(CondDBException);

    virtual void browseObjectsInTag( ICondDBTable         *table,
				     const std::string&    folderName,
				     const std::string&    tagName = "",
				     const std::string&    selection = "",
				     const std::vector <std::string> *nullValues = 0 ) const
	throw(CondDBException);
    
    /**
     * Browse for data at the given point in time (all objects)
     * @param iterObjects An object iterator to store the data found
     * @param folderName The folder in which we are searching
     */

    virtual void browseAllObjects( ICondDBDataIterator*& iterObjects,
				   const std::string&    folderName ) const
	throw(CondDBException);

    virtual void browseAllObjects( ICondDBTable         *table,
				   const std::string&    folderName,
				   const std::string&    selection = "",
				   const std::vector <std::string> *nullValues = 0) const
	throw(CondDBException);
   
    /**
     * Browse folder for a given time interval
     * @param folder The folder to be browsed
     * @param iterObjects An iterator to the objects in which the data found will be stored in this table
     * @param since The lower limit of the time interval
     * @param till The upper limit of the time interval
     */
    
    virtual void browseHistory(ICondDBDataIterator*& iterObjects,
			       const std::string&    folderName,
			       const CondDBKey since,
			       const CondDBKey till,
			       std::string tagName = "") const
	throw(CondDBException);
    
    /**
     * Browse folder for a given time interval
     * @param folder The folder to be browsed
     * @param table Data found will be stored in this table
     * @param since The lower limit of the time interval
     * @param till The upper limit of the tim e interval
     */
    
    virtual void browseHistory(const std::string& folder,
			       ICondDBTable *table,
			       const CondDBKey& since,
			       const CondDBKey& till,
			       const std::string& id = "",
			       const std::string&  selection = "",
			       const std::vector <std::string> *nullValues = 0,
			       string tagName = "") const
	throw(CondDBException);

    /**
     * Browse folder for Ids with optional tag
     * @param path A container which holds the result of the search
     * @param folder The folder name in which to search
     * @param tagName Optional. Search for the given tag. If no tag is give HEAD is used
     */
    
    virtual void browseId (ICondDBTimePath& path, 
			   std::string folder, 
			   std::string tagName = "") const
	throw(CondDBException);
    
    /**
     * Build tag entries in database from the ICondDBTimePath container
     * @param folder The folder name to be associated to the tag
     * @param tagName The tagName to be used
     * @param path The container
     */
    
    virtual void tagFromTimePath (std::string folder, std::string tagName, ICondDBTimePath& path) const
	throw(CondDBException);


	
    /**
     * Get the table schema
     * @param folder The folder whose schema we want
     * @schema A vector containing the types of the columns
     */

    virtual void getTableSchema (std::string folder, ICondDBTable *table)
	throw(CondDBException);

    virtual void dump( std::string folderName = "" ) const
	throw(CondDBException);
    
 private:
    
    // Interface to the relational dbms
    MySqlDBMgr*  relDBMgr;
    
    //  mutable std::map<std::string, d_ULong>  tagNameMap;
    
    //  d_ULong  getTagId(const std::string& tagName) const
    //    throw(CondDBException);
};

#endif










