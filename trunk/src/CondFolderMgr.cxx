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
/* $Id: CondFolderMgr.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondFolderMgr.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#include "CondDBInterface.h"
#include "CondFolder.h"
#include "CondFolderSet.h"
#include "CondDBDataAccess.h"
#include "CondFolderMgr.h"
#include "CondDBTable.h"

#include "Debug.h"

#include <string>

#include "MySqlObjectMgr.h"
#include "MySqlOnlineMgr.h"
#include "MySqlTagMgr.h"
#include "MySqlTypes.h"

// let's try
#include "CondDBLog.h"


/**
 * Constructor
 */
CondFolderMgr::CondFolderMgr(MySqlDBMgr *dbMgr)
{
    relDBMgr    = dbMgr;
}


/**
 * Destructor
 */
CondFolderMgr::~CondFolderMgr()
{
}

/**
 * Initialization method: must be called before using the Folder Manager
 */

void CondFolderMgr::init()
    throw(CondDBException)
{
    relFolderMgr = relDBMgr->getFolderMgr();
    relTagMgr    = relDBMgr->getTagMgr();
}


/**
 * Create the ROOT folderSet
 */

void CondFolderMgr::createRootFolderSet( string attributes  /* = "" */,
					 string description /* = "" */)
    throw(CondDBException)
{
    DebugMesg(CondDB, devl, "Defining the root FolderSet");

    // The root FolderSet shall be unique.Check that there is no one already
    // The method MySqlFolderMgr::store() doesn't allow for duplicate
    // storage, so this test isn't realy necessary. Consider removing!
    if ( exist("/") ) 
    {
	if (relDBMgr->cdbLog->isUsable())
	    relDBMgr->cdbLog->StoreLog("Root FolderSet already existing.The root FolderSet is unique.", __FILE__, __LINE__);

	THROWEX("Root FolderSet already existing. "
		"The root FolderSet is unique.", 0);
    }

    // else there is no one already; create it.
        
    DebugMesg(CondDB, devl, "Adding folderSet to folderTree");

    //Let's retrieve the db_Id...    
    int db_id = getDB_id(relDBMgr->getDBName(), relDBMgr->getSrvName());
    //...done
    
    string rootFolderName("/");
    relFolderMgr->store(rootFolderName, description, attributes,
			0, db_id, MySqlFolderMgr::FolderSet);
    string msg("Addition of folderSet to folderTree succeded.");
    DebugMesg(CondDB, devl, msg);
}

/**
 * Get the database ID
 * @param dbname Database name
 * @param srvname Server name
 * @return Database Id
 */

int CondFolderMgr::getDB_id(string dbname, string srvname)
    throw(CondDBException)
{

    MYSQLSTREAM query;
    query << "SELECT db_id FROM databases_tbl WHERE dbname=\'" << dbname << "\' AND srvname=\'" << srvname << "\'";
    MySqlResult *res = relFolderMgr->select(query);
    Assert(res->countRows()==1);
    int db_Id = res->getIntField(0);
    return db_Id;
}

/**
 * Create a CondDBFolderSet
 */

void
CondFolderMgr::createCondDBFolderSet( const string& fullPathName,
				      string  attributes  /* = "" */,
				      string  description /* = "" */,
				      bool    parents     /* = false */,
				      int dd_type) 
    throw(CondDBException)
{
    DebugMesg(CondDB, user, "Creating FolderSet : " << fullPathName);
    DebugMesg(CondDB, user, "       description : " << description);
    DebugMesg(CondDB, user, "        attributes : " << attributes);
    
    // check if this folder name already exists
    
    if ( exist(fullPathName) == true )
    {
	if (relDBMgr->cdbLog->isUsable())
	    relDBMgr->cdbLog->StoreLog("This folder cannot be created as this folder name exists already", __FILE__, __LINE__);

	THROWEX("This folder cannot be created as this folder"
		" name exists already", 10);
    }

    string parentFolderSet;
    string folderSetName;
    splitPath(fullPathName, parentFolderSet, folderSetName);

    MySqlResult *res = relFolderMgr->find(parentFolderSet,
					  MySqlFolderMgr::FolderSet);
    if ( res->countRows()==0 ) {
	delete res;
	if ( folderSetName == "" ) { // defining the root
	    createRootFolderSet(attributes, description);
	    return;
	}
	string msg("FolderSet "); msg = msg + parentFolderSet +
				      " does not exist";
	if ( parents == false )
	{
	    if (relDBMgr->cdbLog->isUsable())
		relDBMgr->cdbLog->StoreLog(msg.c_str(), __FILE__, __LINE__);

	    THROWEX(msg, 0);
	}	
        // create parentFolderSet with same attributes and description
	DebugMesg(CondDB, user, "Creating parent FolderSet");
	createCondDBFolderSet(parentFolderSet, attributes, description, true, dd_type);
	res = relFolderMgr->find(parentFolderSet, MySqlFolderMgr::FolderSet);
    }

    // Get parent properties
    int parentId = res->getIntField(FOLDER_ID);
    if ( attributes == "" )
	attributes = res->getField(FOLDER_ATTR);
    delete res;

    //Let's retrieve the db_Id...    
    int db_id = getDB_id(relDBMgr->getDBName(), relDBMgr->getSrvName());
    //...done

    relFolderMgr->store(fullPathName, description, attributes,
			parentId, db_id, MySqlFolderMgr::FolderSet, dd_type);
    
    DebugMesg(CondDB, devl, "FolderSet creation completed.");
}

/**
 * Create a CondDBFolder
 */

void
CondFolderMgr::createCondDBFolder( const string& fullPathName,
				   string  attributes  /* = "" */,
				   string  description /* = "" */,
				   bool    parents     /* = false */) 
    throw(CondDBException)
{
    DebugMesg(CondDB, user, "Creating Folder : " << fullPathName);
    DebugMesg(CondDB, user, "    description : " << description);
    DebugMesg(CondDB, user, "     attributes : " << attributes);

    // check if this folder name already exists
    if ( exist(fullPathName) == true )
    {
	if (relDBMgr->cdbLog->isUsable())
	    relDBMgr->cdbLog->StoreLog("This folder cannot be created as this folder name exists already", __FILE__, __LINE__);

	THROWEX("This folder cannot be created as this folder "
		"name exists already", 10);
    }
    // It is really new; create new CondFolder
    string parentFolderSet;
    string folderName;
    splitPath( fullPathName, parentFolderSet, folderName);

    // look for the parentFolderSet
    MySqlResult *res = relFolderMgr->find(parentFolderSet, 
					  MySqlFolderMgr::FolderSet);
    if ( res->countRows()==0 )
    {
	delete res;
	string msg("FolderSet "); msg = msg + parentFolderSet +
				      " does not exist";
	if ( parents == false )	{
	    if (relDBMgr->cdbLog->isUsable())
		relDBMgr->cdbLog->StoreLog(msg.c_str(), __FILE__, __LINE__);
	    
	    THROWEX(msg, 0);
	}
	else {  // create parentFolderSet with same attributes and description
	    DebugMesg(CondDB, user, "Creating parent FolderSet");
	    createCondDBFolderSet(parentFolderSet, attributes, description, true);
	}
	res = relFolderMgr->find(parentFolderSet, MySqlFolderMgr::FolderSet);
    }

    // Get parent properties
    int parentId = res->getIntField(FOLDER_ID);
    if ( attributes == "" )
	attributes = res->getField(FOLDER_ATTR);
    delete res;
    DebugMesg(CondDB, devl, "test --- attributes: " << attributes);

    // Store the folder. 

    int folderId;

    //Let's retrieve the db_Id...    
    int db_id = getDB_id(relDBMgr->getDBName(), relDBMgr->getSrvName());
    //...done

    folderId = relFolderMgr->store(fullPathName, description, attributes,
				   parentId, db_id, MySqlFolderMgr::Folder);

    // Get the object manager (this eventualy create a new database)
    MySqlObjectMgr *objM = relDBMgr->getObjectMgr(db_id);

    // Create tables
    objM->createTables(folderId);

    // Find
    DebugMesg(CondDB, devl, "Folder creation completed.");
}

/**
 * Creates a folder for a structure
 * @param table This is a CondDBTable object
 * @param ftype The type of the folder
 */

void
CondFolderMgr::createCondDBFolder( const string& fullPathName,
				   ICondDBTable* table,
				   string  attributes ,
				   string  description ,
				   bool    parents,
				   CondFolder::folder_types ftype)
    throw(CondDBException)
{
    if (ftype==CondFolder::BLOBTAG)
    {
	THROWEX("Wrong folder type", 0);
    }
    DebugMesg(CondDB, user, "Creating Folder : " << fullPathName);
    DebugMesg(CondDB, user, "    description : " << description);
    DebugMesg(CondDB, user, "     attributes : " << attributes);

    // check if this folder name already exists
    if ( exist(fullPathName) == true )
    {
	if (relDBMgr->cdbLog->isUsable())
	    relDBMgr->cdbLog->StoreLog("This folder cannot be created as this folder name exists already", __FILE__, __LINE__);

	THROWEX("This folder cannot be created as this folder "
		"name exists already", 10);
    }
    // It is really new; create new CondFolder
    string parentFolderSet;
    string folderName;
    bool first=0;
    splitPath( fullPathName, parentFolderSet, folderName);

    // look for the parentFolderSet
    MySqlResult *res = relFolderMgr->find(parentFolderSet, 
					  MySqlFolderMgr::FolderSet);
    if ( res->countRows()==0 )
    {
	delete res;
	string msg("FolderSet "); msg = msg + parentFolderSet +
				      " does not exist";
	if ( parents == false )	{
	    if (relDBMgr->cdbLog->isUsable())
		relDBMgr->cdbLog->StoreLog(msg.c_str(), __FILE__, __LINE__);

	    THROWEX(msg, 0);
	}
	else {  // create parentFolderSet with same attributes and description
	    DebugMesg(CondDB, user, "Creating parent FolderSet");
	    createCondDBFolderSet(parentFolderSet, attributes, description, true, 1);
	    first=1;
	}
	res = relFolderMgr->find(parentFolderSet, MySqlFolderMgr::FolderSet);
    }

    // Get parent properties
    int parentId = res->getIntField(FOLDER_ID);
    if ( attributes == "" )
	attributes = res->getField(FOLDER_ATTR);
    delete res;
    DebugMesg(CondDB, devl, "test --- attributes: " << attributes);

    // Store the folder.

    int folderId = relFolderMgr->store(fullPathName, description, attributes,
				   parentId, 1, MySqlFolderMgr::Folder, ftype);
    
    int db_id = getDB_id(relDBMgr->getDBName(), relDBMgr->getSrvName());

    // Get the online manager (this eventually creates a new database)
    MySqlOnlineMgr *onlM = relDBMgr->getOnlineMgr(db_id);

    //Get the schema of the table
    vector <string> names;
    vector <CondDBTable::cdb_types> types;

    table->getNames(names);
    table->getTypes(types);

    // Create tables
      // Create tables
    switch (ftype)
    {
	case CondFolder::STRUCTID:
	{
	    onlM->createTablesOnl(folderId, names, types, true);
	    break;
	}
	case CondFolder::STRUCT:
	{
	    onlM->createTablesOnl(folderId, names, types, false);
	    break;
	}
	case CondFolder::STRUCTIDTAG:
	{
	    onlM->createTablesTag(folderId, names, types, true);
	    break;
	}
	case CondFolder::STRUCTTAG:
	{
	    onlM->createTablesTag(folderId, names, types, false);
	    break;
	}
	default: 
	{
	    cerr << "Wrong folder type" << endl;
	    break;
	}
	
    }
    DebugMesg(CondDB, devl, "Folder creation completed.");
}

/**
 * Returns the type of the given folder name
 * @param fullPathName This is the full name of the folder whose type we want to determine
 */

int CondFolderMgr::getFolderType(const string& fullPathName)
	throw(CondDBException)
{
    return relFolderMgr->getFolderType(fullPathName);
}


/**
 * Checks if the folder given by fullName already exists as CondFolder or CondFolderSet
 * @param fullName Complete name of the folder
 */

bool CondFolderMgr::exist( const string& fullName ) const
    throw(CondDBException)
{
    string normName;
    normalizePath( fullName, normName );
    return relFolderMgr->exist(normName, MySqlFolderMgr::Any);
}

void CondFolderMgr::getCondDBFolder( const string& fullPathName,
				     ICondDBFolder*& folder) const
    throw(CondDBException)
{
    // look for the Folder
    CondFolder *condFolder;
    MySqlResult *res = relFolderMgr->find(fullPathName, MySqlFolderMgr::Folder);
    
    if (res->countRows() != 0)
    {
	// if the folderType is not of the BLOB type we must prepend the address description for use in Athena
	int fType = relFolderMgr->getFolderType(fullPathName);
	
	if ((fType !=ICondDBFolder::BLOB) && (fType != ICondDBFolder::BLOBTAG)) 
	{
	    std::string addrHeader("<addrHeader><address_header service_type=\"72\" clid=\"94049889\" /></addrHeader><typeName>GenericDbTable</typeName><timeStamp>time</timeStamp>");
	    condFolder = new CondFolder(res, addrHeader);
	}
	else {
	    condFolder = new CondFolder(res);
	}
	delete res;
    }
    else
    {
	condFolder = 0;
	string msg("Folder "); msg = msg + fullPathName + " does not exist";
	THROWEX(msg, 0);
    }
    
    
    folder = static_cast<ICondDBFolder*>(condFolder);
}
    

void
CondFolderMgr::getCondDBFolderSet( const string& fullPathName,
				   ICondDBFolderSet*& folderSet) const
    throw(CondDBException)
{
    // look for the FolderSet
    // look for the Folder
    CondFolderSet *condFolderSet;
    MySqlResult *res = relFolderMgr->find(fullPathName, MySqlFolderMgr::FolderSet);
    
    if (res->countRows() != 0)
    {
	condFolderSet = new CondFolderSet(this, res);
	delete res;
    }
    else
    {
	condFolderSet = 0;
	string msg("FolderSet "); msg = msg + fullPathName + " does not exist";
	THROWEX(msg, 0);
    }
    
    folderSet = static_cast<ICondDBFolderSet*>(condFolderSet);
}



void
CondFolderMgr::getAllCondDBFolder( vector<string>& allCondFolder ) const
    throw(CondDBException)
{
    string name("");
    MySqlResult *res = relFolderMgr->browse(name, MySqlFolderMgr::Folder);
    Assert( res->countRows() != 0 );
    do {
	allCondFolder.push_back(res->getField(FOLDER_PATHNAME));
    } while ( res->nextRow() );
    delete res;
}


void
CondFolderMgr::getAllCondDBFolderSet( vector<string>& allCondFolderSet ) const
    throw(CondDBException)
{
    string name("");
    MySqlResult *res = relFolderMgr->browse(name, MySqlFolderMgr::FolderSet);
    Assert( res->countRows() != 0 );
    
    do {
	allCondFolderSet.push_back(res->getField(FOLDER_PATHNAME));
    } while ( res->nextRow() );
    delete res;
}

// if fullPathName is a Folder, folderList will contain just fullPathName
// otherwise it will be a full list (including fullPathName)
void
CondFolderMgr::getAllCondDBFolderBeneath( const string& fullPathName,
					  vector<string>& folderList ) const
    throw(CondDBException)
{
    MySqlResult *res;

    // check if fullPathName exist
    string msg;

    if ( !exist(fullPathName) )
    {
	msg = "Name"; msg += fullPathName;
	msg += " does not exist neither as Folder nor as FolderSet";
	THROWEX(msg, 0);
    }
    
    if ( (res = findFolder(fullPathName)) != 0 )
    {
	// yes, it's a Folder
	delete res;
	if ( (res = findFolderSet(fullPathName)) == 0 ) // is it a folder set ?
	{
	    folderList.push_back(fullPathName); // only this folder has to be tagged
	    delete res;
	    return;
	}
	else
	{
	    // fullPathName is a FolderSet, must find all the Folder contained by it
	    // and all its contained FolderSet
	    // NOTE: fullPathName must not be added to folderList as FolderSet are not
	    // JMAL  tagged What is the meaning of the above line???
	    
	    // JMAL Question: we want just the folders and folderSets imediately beneath 
	    // JMAL           this folderSet or all its descendents?
	    
#if 0   // All the descendents
	    delete res;
	    res = relFolderMgr->browse(fullPathName, MySqlFolderMgr::Any);
#else   // Just imediately beneath
	    if (res->countRows()!=0)
	    {
		int parentId = atoi( res->getField(FOLDER_ID) );
		delete res;
		res = relFolderMgr->browseChilds(parentId, MySqlFolderMgr::Any);
	    }
#endif 
	    folderList.push_back(fullPathName);
	    if ( res->countRows()!=0 )
		do {
		    folderList.push_back( res->getField(FOLDER_PATHNAME) );
		} while ( res->nextRow() );
	    
	    delete res;
	}
    }
}

// Not in the interface
// To be used by CondFolderSet

MySqlResult *
CondFolderMgr::getContainedFolderRef(int parentId, 
				     MySqlFolderMgr::folder_type type) const
    throw(CondDBException)
{
    return relFolderMgr->browseChilds(parentId, type); 
}

//
// Private methods
//

// returns 0 if folderSetFullName is not found 
MySqlResult *
CondFolderMgr::findFolderSet( const string& folderSetFullName ) const
    throw(CondDBException)
{
    return relFolderMgr->find(folderSetFullName, MySqlFolderMgr::FolderSet);
}


// returns 0 if folderFullName is not found 
MySqlResult *
CondFolderMgr::findFolder( const string& folderFullName ) const
    throw(CondDBException)
{
    return relFolderMgr->find(folderFullName, MySqlFolderMgr::Folder);
}

void 
CondFolderMgr::deleteFolder( const string& folderFullName )
    throw(CondDBException)
{
    int folderId, dbPath, folderType;
    folderType = getFolderType(folderFullName);
    relDBMgr->getFolderId(folderFullName, folderId, dbPath);

    if (folderType == ICondDBFolder::BLOB || folderType == ICondDBFolder::BLOBTAG || folderType == ICondDBFolder::STRUCTTAG )
    {
	MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(dbPath);
	if (!findFolderSet(folderFullName))
	    objectMgr->deleteTables(folderId, folderType);
	
	relFolderMgr->deleteFolder(folderId);
    }
    else
    {
	MySqlOnlineMgr *objectMgr = relDBMgr->getOnlineMgr(dbPath);
	if (!findFolderSet(folderFullName))
	    objectMgr->deleteTables(folderId);
	relFolderMgr->deleteFolder(folderId);
    }
    // delete object data
    // delete object keys
//    objectMgr->deleteTables(folderId);
    
    // delete folder
//    relFolderMgr->deleteFolder(folderId);
}


// extract parent and name
void
CondFolderMgr::splitPath(const string& fullPathName,
			 string&       parent,
			 string&       name) const
    throw(CondDBException)
{
    string normName;
    normalizePath(fullPathName, normName);
    
    string::size_type lastPos=0;  // position of last /
    
    // determine lastPos
    for (long i=normName.size()-1; i>-1; i--)
    {
	if ( normName[i] == separator )
	{
	    lastPos = i; 
	    break;
	}
    }
    
    if (lastPos==0)
	parent = separator;
    else
	parent = normName.substr(0, lastPos);
    
    name = normName.substr(lastPos+1, normName.size()-1);
    DebugMesg(CondDB, user, "   parentFolderSet : " << parent);
    DebugMesg(CondDB, user, "              name : " << name);
}


// remove useless "/"; normName=/aaa/bbb/ccc
void
CondFolderMgr::normalizePath( const string& fullPathName,
			      string&       normName ) const
    throw(CondDBException)
{
    if ((fullPathName[0] != separator) ||
	(fullPathName.size() == 0) )
	THROWEX("Invalid fullPathName", 0);
    
    normName += separator;  // ensure there is an initial /
    
    // remove duplicated /
    for (unsigned long i=0; i<fullPathName.size(); i++)
    {
	if ( (normName[normName.size()-1] == separator) &&
	     (fullPathName[i] == separator) )
	    continue; // skip duplicated /
	
	normName += fullPathName[i];
    }

    // remove trialing /
    if ( normName.size()>1 )  // skip case normName=/
    {
	while (1)
	{
	    if ( normName[normName.size()-1] == separator )
		normName.erase(normName.size()-1,1);
	    else
		break;
	}
    }
}


void CondFolderMgr::describe(const string& /* fullName */,
                             string /* options */) const
    throw(CondDBException)
{
    // Not yet implemented
}




