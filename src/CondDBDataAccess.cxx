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
/* $Id: CondDBDataAccess.cxx,v 1.6 2009-11-30 08:57:09 meyern Exp $ */

// $HEAD 10
//
// File: "CondDBDataAccess.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#include "CondDBDataAccess.h"
#include "CondDBDataIterator.h"
#include "CondDBInterface.h" 
#include "CondDBObject.h"
#include "CondDBTable.h"
#include "CondDBTimePath.h"
#include "CondFolderMgr.h"
#include "CondFolder.h"
#include "CondDBTagMgr.h"
#include "MySqlTypes.h"

#include "MySqlOnlineMgr.h"
#include "MySqlObjectMgr.h"
#include "MySqlDataMgr.h"

#include "ConditionsDB/ICondDBTable.h"
#include "ConditionsDB/CondDBMySQLTableFactory.h"

#include <iostream>


// Constructor
CondDBDataAccess::CondDBDataAccess(MySqlDBMgr *dbMgr)
{
    relDBMgr = dbMgr;
}


// Destructor
CondDBDataAccess::~CondDBDataAccess()
{

}

 
void
CondDBDataAccess::init()
    throw(CondDBException)
{
   //Assert(condTagMgr != 0);

   // create tagNameMap
   //   condTagMgr->getTagNameMap(tagNameMap);
}

/**
 * Store object in the given folder
 * @param folderName Folder in which the object is to be stored in
 * @param CondObject The object to be stored
 */

void 
CondDBDataAccess::storeCondDBObject(const string& folderName, ICondDBObject* CondObject )
    throw(CondDBException)
{
    string theData;
    string theDesc;

    int data_id, folder_id, part_id, okey_path, ftype;
    CondDBObject* obj = static_cast<CondDBObject*>(CondObject);
 
// nbarros: does this methods create connections to fetch the data? NO!!  
    obj->data(theData);
    obj->description(theDesc);

// nbarros: relDBMgr -> MySqlDBMgr
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype == CondFolder::BLOBTAG)
    {
	relDBMgr->getFolderId(folderName, folder_id, okey_path);
	
	MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(okey_path);
	
	part_id = objectMgr->getPartitionId(obj->validSince(), folder_id);
	
	MySqlDataMgr *dataMgr = relDBMgr->getDataMgr(part_id);
	data_id = dataMgr->store(folder_id, part_id , theDesc, theData);
	objectMgr->store(obj->validSince(), obj->validTill(), 
			 folder_id, part_id, data_id);
//e aqui............................................................
	DebugMesg(CondDB, user, "Object stored");
    }
    else
	cerr << "Wrong folder type" << endl;
}

/**
 * Store table in the given folder
 * @param folderName Folder in which the table is to be stored in
 * @param table The table to be stored
 */

void CondDBDataAccess::storeCondDBObject( const string& folderName, ICondDBTable *table)
    throw(CondDBException)
{
    int part_id, dbId, fld_id, ftype;
    
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype != CondFolder::BLOBTAG)
    {
	CondDBTable* tableOnl = static_cast<CondDBTable*>(table); 
	
	relDBMgr->getFolderId(folderName, fld_id, dbId);
	MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(dbId);
	SimpleTime since;
	table->getSinceTime(0, since);
	part_id = onlineMgr->getPartitionId(since, fld_id);
	switch (ftype)
	{
	    case CondFolder::STRUCTID:
	    {
		onlineMgr->storeOnlId(fld_id, part_id, tableOnl);
		break;
	    }
	    case CondFolder::STRUCT:
	    {
		onlineMgr->storeOnl(fld_id, part_id, tableOnl);
		break;
	    }
	    case CondFolder::STRUCTTAG:
	    {
		onlineMgr->storeVer(fld_id, part_id, tableOnl);
		break;
	    }
	    default:
		cerr << "Folder type not supported by this function" << endl;
	}
	DebugMesg(CondDB, user, "Data stored");
    }
    else
	cerr << "Wrong folder type" << endl;
}

/**
 * Find data which next becomes valid after given point in time 
 *   (latest version or tag)
 * @param oblock The found data will be stored in this object
 * @param folderName The folder in which we are searching
 * @param point The point in time to start the search
 * @param tagName Optional. Find for tags
 */

void CondDBDataAccess::findNextValidCondDBObject( ICondDBObject*&  oblock,
						  const string&    folderName,
						  const CondDBKey& point,
						  string           tagName ) const
    throw(CondDBException)
{

  if ( oblock != NULL && oblock->validSince() <= point ) {
    std::cerr << "WARNING: does not make sense to call CondDBDataAccess::findNextValidCondDBObject with" << std::endl
	      << " a default object which is already valid. Will reset return object to NULL" << std::endl;
    delete oblock;
  }

  int tagId, folderId, dbPath, ftype;
  std::string vObject;
  CONDDBACCESSSTREAM m_Object;
  
  relDBMgr->getFolderType(folderName, ftype);
  if (ftype == CondFolder::BLOBTAG) {
    relDBMgr->getFolderId(folderName, folderId, dbPath);
    relDBMgr->getTagId(tagName, tagId);
    MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(dbPath);
    MySqlResult *res = objectMgr->browseTagged( folderId, tagId );
    if ( res!=NULL && res->countRows() ) {
      CondDBDataIterator condIterator( relDBMgr, res, folderId );
      condIterator.goToFirst();
      for ( ICondDBObject* obj = condIterator->current(); condIterator->hasNext(); obj = condIterator->next() ) {
//      while ( condIterator.hasNext() ) {
//	ICondDBObject* obj = condIterator.next();
	if ( obj->validSince() > point 
	     && ( oblock == NULL 
		  || obj->validSince() < oblock->validSince() ) 
	     ) {
	  if ( oblock != NULL ) delete oblock;
	  oblock = obj;
	} else {
	  delete obj;
	}
      }
    }
    
    /*
      MySqlResult *res = objectMgr->find(point, folderId, tagId);
      if (res->countRows()) 
      {	
      //	Assert ( res->countRows() == 1 );
      condObject = new CondDBObject(relDBMgr, res, folderId);
      oblock = static_cast<ICondDBObject*>(condObject);
      }
      delete res;
    */
  }
}


/**
 * Find data for the given point in time (latest version or tag)
 * @param oblock The found data will be stored in this object
 * @param folderName The folder in which we are searching
 * @param point The point in time to be found
 * @param tagName Optional. Find for tags
 */

void CondDBDataAccess::findCondDBObject( ICondDBObject*&  oblock,
				    const string&    folderName,
				    const CondDBKey& point,
				    string           tagName ) const
    throw(CondDBException)
{

    int tagId, folderId, dbPath, ftype;
    CondDBObject *condObject;
    std::string vObject;
    CONDDBACCESSSTREAM m_Object;

    relDBMgr->getFolderType(folderName, ftype);
    relDBMgr->getFolderId(folderName, folderId, dbPath);
    if (ftype == CondFolder::BLOBTAG)
    {
//	relDBMgr->getFolderId(folderName, folderId, dbPath);
	relDBMgr->getTagId(tagName, tagId);
	MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(dbPath);
	MySqlResult *res = objectMgr->find(point, folderId, tagId);
	if (res->countRows()) 
	{	
//	Assert ( res->countRows() == 1 );
	  condObject = new CondDBObject(relDBMgr, res, folderId);
	  oblock = static_cast<ICondDBObject*>(condObject);
	}
	delete res;
    }
    else 
	// for use with IOV. We'll just create a virtual ICondDBObject
	// Since IOV is the reponsible to manage the time of validity of each Athena object
	// getting the time of validity of the object containing the address 
	// we need to fetch the table in order to get the times and thus generate a CondDBObject
	// conatining the address to the table with the valid interval of validity
	if (ftype == CondFolder::STRUCT || ftype == CondFolder::STRUCTID || ftype == CondFolder::STRUCTTAG)
	{
	    
	    std::string fDesc;
	    SimpleTime DPoint;
	    bool isTime = false;
	    // first we need to figure which time encoding we have in this folder
	    
	    //let's get the folder description

	    CondFolder *folder;
	    
	    MySqlFolderMgr *relFolderMgr = relDBMgr->getFolderMgr();
	    
	    // let's get the folder
	    folder = new CondFolder(relFolderMgr->find(folderId,MySqlFolderMgr::folder_type(0)));
	    
	    // finally let'd get the folder description
	    fDesc = folder->getDescription();
	    if (fDesc.find("<timeStamp>run-event</timeStamp>") != std::string::npos) {
		isTime = false;
		DPoint = point;
	    }
	    else // if the description don't have the xml flag it means that it is a "real" time folder
		if ((fDesc.find("<timeStamp>time</timeStamp>") != std::string::npos) || (fDesc == "") ) {
		    
		    time_t TPoint = point;
		    DPoint.fromUnixTime(TPoint); // this will convert the seconds to nanoseconds
		    isTime = true;
		}
		else
		{
		    DebugMesg(CondDB, user, 
			      "There is a problem in folder descriptions. The descriptions does not have any information about the time encoding");
		    cerr << "There is a problem in folder descriptions" << endl;
		}
/*
// first we need to convert the 64bit value to a 32 bit
time_t TPoint = point;
SimpleTime DPoint(TPoint); // this will convert the seconds to nanoseconds
*/	    
	    // defining the data of the *virtual* object
	    // since the time is to be hadled by the CondDBMySQLCnvSvc, the value must be in nanoseconds
	    m_Object << "<key>" << folderName << "</key>"
		     << "<folder>" << folderName << "</folder>"
		     << "<timekey>" << DPoint.toTimeT() << "</timekey>";
	    
	    if (tagName != "")
		m_Object << "<tag>" << tagName << "</tag>";
	    
            // debugging the object created
	    DebugMesg(CondDB, user," Returning the IOV object ->[" << m_Object.str() << "]");
	    
	    ICondDBTable* m_table = CondDBMySQLTableFactory::createCondDBTable();
	    // let's get the table to know the interval of validity
	    findCondDBObject(m_table,folderName,DPoint,"","",0,tagName);
	    
// if the table is a STRUCID we must find the smallest interval of vallidity
	    
	    CondDBKey since;
	    CondDBKey till;
	    SimpleTime sinceT, sinceTime;
	    SimpleTime tillT, tillTime;
	    
	    // getting the first time intervals 
	    m_table->getSinceTime(0,sinceTime);
	    m_table->getTillTime(0, tillTime);
	    
	    //getting the number of objects retrieved
	    int numRows = m_table->getNumRows();
	    for (int i=1; i< numRows; i++)
	    {
		m_table->getSinceTime(i,sinceT);
		m_table->getTillTime(i, tillT);
		
		if (sinceT > sinceTime)
		    sinceTime = sinceT;
		if (tillT < tillTime)
		    tillTime = tillT;
	    }

#ifdef _WIN32
	    const TimeT SIMPLETIME_UNIX_ORIGIN = 2208988800000000000;     // 1.1.1970
	    const TimeT SIMPLETIME_UNIX_MAX    = 4356476047000000000;     // 19.01.2038
#else
	    const TimeT SIMPLETIME_UNIX_ORIGIN = 2208988800000000000LL;     // 1.1.1970
	    const TimeT SIMPLETIME_UNIX_MAX    = 4356476047000000000LL;     // 19.01.2038
#endif

	    if (isTime) {
		// if since is before unix origin it'll be set to 0
		if (sinceTime < SIMPLETIME_UNIX_ORIGIN)
		    since = 0;
		else
		    since = sinceTime.toUnixTime();
		
		
		if (tillTime.toTimeT() > SIMPLETIME_UNIX_MAX)
		{
		    // max unix time
		    tillTime = SimpleTime(2038,1,19,3,14,7,800000000);

		    till = tillTime.toUnixTime();
		}
		else
		    till = tillTime.toUnixTime();
		
		DebugMesg(CondDB, user,"Returning the time intervals (UNIX TIME)  [SINCE;TILL]=[" << since << ";" << till << "]");
		DebugMesg(CondDB, user,"Returning the time intervals (SIMPLETIME) [SINCE;TILL]=[" << sinceTime.toTimeT() << ";" << tillTime.toTimeT() << "]");
	    }
	    else {
		since = sinceTime.toTimeT();
		till  = tillTime.toTimeT();
		DebugMesg(CondDB, user,"Returning the time intervals (NANOSECONDS) [SINCE;TILL]=[" << since << ";" << till << "]");
	    }
	    
	    
	    //now let's create the *virtual* object to return
	    condObject = new CondDBObject(since,till,0,0,m_Object.str(),"",false);
	    oblock = static_cast<ICondDBObject*>(condObject);
	}
	else
	    cerr << "Folder is of wrong type!" << endl;
}

/**
 * Find data for the given point in time (latest version or tag)
 * @param table The found data will be stored in this table
 * @param folderName The folder in which we are searching
 * @param point The point in time to be found
 * @param tagName Optional. Find for tags
 */

void CondDBDataAccess::findCondDBObject (ICondDBTable *table,
					 const string& folderName, 
					 const CondDBKey& point, 
					 const string& id,
					 const string& selection,
					 const vector <string> *nullValues,
					 string tagName) const
    throw(CondDBException)
{
   int part_id, db_path, fld_id, ftype;
 
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype != CondFolder::BLOBTAG)
    {
	CondDBTable *tableOnl;
	relDBMgr->getFolderId(folderName, fld_id, db_path);
	MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	part_id = onlineMgr->getPartitionId(point, fld_id);
	tableOnl = static_cast<CondDBTable*>(table);
	string col;
	switch (ftype)
	{
	    case CondFolder::STRUCT:
	    {
		onlineMgr->findOnl(point, fld_id, part_id, tableOnl, "", selection, nullValues);
		break;
	    }
	    case CondFolder::STRUCTID:
	    {
		onlineMgr->findOnl(point, fld_id, part_id, tableOnl, id, selection, nullValues);
		break;
	    }
	    case CondFolder::STRUCTTAG:
	    {
		int tagId;
		relDBMgr->getTagId(tagName, tagId);
		onlineMgr->findTag(point, fld_id, part_id, tagId, tableOnl, selection, nullValues);
		break;
	    }
	    default:
		cerr << "Folder type not handled by this function" << endl;
	}
    }
    else 
	cerr << "Folder is of wrong type!" << endl;
}

/**
 * Not implemented
 */

void
CondDBDataAccess::dump( string folder ) const
    throw(CondDBException)
{
    if (folder == "")
    {
	DebugMesg(CondDB, user, "Warning: dump() not yet implemented. Please pass a folder");
	return;
    }
    
    DebugMesg(CondDB, user, "Printing all CondObject contained in CondFolder " << folder);
}

/**
 * Browse for data at the given point in time (all versions)
 * @param iterObjects An object iterator to store the data found
 * @param folderName The folder in which we are searching
 * @param point The point in time for which we are searching
 */

void CondDBDataAccess::browseObjectsAtPoint( ICondDBDataIterator*& iterObjects,
					const string&         folderName,
					const CondDBKey&      point ) const
    throw(CondDBException)
{
    int folder_id, tbl_path, ftype;
    CondDBDataIterator *condIterator;
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype == CondFolder::BLOBTAG)
    {
	relDBMgr->getFolderId(folderName, folder_id, tbl_path);
	MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(tbl_path);
	MySqlResult *res = objectMgr->browseAtPoint(point, folder_id);
//	Assert(res->countRows() > 0);
	if (res->countRows())
	   condIterator = new CondDBDataIterator(relDBMgr, res, folder_id);
	else 
	   condIterator = 0;
	iterObjects = static_cast<ICondDBDataIterator*>(condIterator);
    }
    else 
	cerr << "Folder is of wrong type!" << endl;
}

void CondDBDataAccess::browseObjectsAtPoint( ICondDBTable         *table,
					     const string&    folderName,
					     const CondDBKey&      point,
					     const string&    selection,
					     const vector <string> *nullValues) const
    throw(CondDBException)
{
    int part_id, db_path, fld_id, ftype;
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype != CondFolder::BLOBTAG)
    {
	CondDBTable *tableOnl;
	relDBMgr->getFolderId(folderName, fld_id, db_path);
	MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	part_id = onlineMgr->getPartitionId(point, fld_id);
	tableOnl = static_cast<CondDBTable*>(table);
	switch (ftype)
	{
	    case CondFolder::STRUCTTAG:
	    {
		onlineMgr->browsePoint(point, fld_id, part_id, tableOnl, selection, nullValues);
		break;
	    }
	    default:
		cerr << "Folder type not handled by this function" << endl;
	}
    }
    else 
	cerr << "Folder is of wrong type!" << endl;
}

/**
 * Browse for data at the given point in time (all versions)
 * @param iterObjects An object iterator to store the data found
 * @param folderName The folder in which we are searching
 * @param point The point in time for which we are searching
 */

void CondDBDataAccess::browseObjectsInInterval( ICondDBDataIterator*& iterObjects,
						const string&         folderName,
						const CondDBKey&      begin,
						const CondDBKey&      end ) const
  throw(CondDBException)
{
    int folder_id, tbl_path, ftype;
    CondDBDataIterator *condIterator;
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype == CondFolder::BLOBTAG)
    {
	relDBMgr->getFolderId(folderName, folder_id, tbl_path);
	MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(tbl_path);
	MySqlResult *res = objectMgr->browseInInterval(begin, end, folder_id);
//	Assert(res->countRows() > 0);
	if (res->countRows())
	   condIterator = new CondDBDataIterator(relDBMgr, res, folder_id);
	else 
	   condIterator = 0;
	iterObjects = static_cast<ICondDBDataIterator*>(condIterator);
    }
    else 
	cerr << "Folder is of wrong type!" << endl;
}


/**
 * Browse for data at the given point in time (for tag)
 * @param iterObjects An object iterator to store the data found
 * @param folderName The folder in which we are searching
 * @param tagName The tag for which we are searching, no tag means head
 */

void
CondDBDataAccess::browseObjectsInTag(ICondDBDataIterator*& iterObjects,
				     const string&         folderName,
				     string                tagName) const
    throw(CondDBException)
{
    int tag_id, folder_id, tbl_path, ftype;
    CondDBDataIterator *condIterator;
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype == CondFolder::BLOBTAG)
    {
	relDBMgr->getFolderId(folderName, folder_id, tbl_path);
	relDBMgr->getTagId(tagName, tag_id);
	MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(tbl_path);
	MySqlResult *res = objectMgr->browseTagged(folder_id, tag_id);
	
	// Use mysql_fetch_row just to check if the result set is empty 
	if (res!=NULL && res->countRows()){
//	    Assert (res->countRows() > 0);
	    condIterator = new CondDBDataIterator(relDBMgr, res, folder_id);
	    iterObjects = static_cast<ICondDBDataIterator*>(condIterator);
	}
	else {
	    THROWEX("tagName does not exist", 0);
	}
	
    }
    else 
	cerr << "Folder is of wrong type!" << endl;
}

void CondDBDataAccess::browseObjectsInTag( ICondDBTable     *table,
					   const string&    folderName,
					   const string&    tagName,
					   const string&    selection,
					   const vector <string> *nullValues) const
    throw(CondDBException)
{
    int db_path, fld_id, ftype;
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype != CondFolder::BLOBTAG)
    {
	CondDBTable *tableOnl;
	relDBMgr->getFolderId(folderName, fld_id, db_path);
	MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	tableOnl = static_cast<CondDBTable*>(table);
	switch (ftype)
	{
	    case CondFolder::STRUCTTAG:
	    {
		int tagId;
		relDBMgr->getTagId(tagName, tagId);
		onlineMgr->browseTag(fld_id, tagId, tableOnl, selection, nullValues);
		break;
	    }
	    default:
		cerr << "Folder type not handled by this function" << endl;
	}
    }
    else 
	cerr << "Folder is of wrong type!" << endl; 
}
    

/**
 * Browse for data at the given point in time (all objects)
 * @param iterObjects An object iterator to store the data found
 * @param folderName The folder in which we are searching
 */

void
CondDBDataAccess::browseAllObjects(ICondDBDataIterator*& iterObjects,
				   const string&  folderName ) const
    throw(CondDBException)
{
    int folder_id, tbl_path, ftype;
    CondDBDataIterator *condIterator;
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype == CondFolder::BLOBTAG)
    {
	relDBMgr->getFolderId(folderName, folder_id, tbl_path);
	MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(tbl_path);
	MySqlResult *res = objectMgr->browseAll(folder_id);
//	Assert (res->countRows() > 0);
	if (res->countRows())
	{
	  condIterator = new CondDBDataIterator(relDBMgr, res, folder_id);
	
          iterObjects = static_cast<ICondDBDataIterator*>(condIterator);
	}
    }
    else
	cerr << "Folder is of wrong type!" << endl;
}

void CondDBDataAccess::browseAllObjects( ICondDBTable         *table,
					 const string&    folderName,
					 const string&    selection,
					 const vector <string> *nullValues) const
	throw(CondDBException)
{
    int db_path, fld_id, ftype;
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype != CondFolder::BLOBTAG)
    {
	CondDBTable *tableOnl;
	relDBMgr->getFolderId(folderName, fld_id, db_path);
	MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	tableOnl = static_cast<CondDBTable*>(table);
	switch (ftype)
	{
	    case CondFolder::STRUCTTAG:
	    {
		onlineMgr->browseAll(fld_id, tableOnl, selection, nullValues);
		break;
	    }
	    default:
		cerr << "Folder type not handled by this function" << endl;
	}
    }
    else 
	cerr << "Folder is of wrong type!" << endl; 
}

/**
 * Browse folder for a given time interval
 * @param folder The folder to be browsed
 * @param iterObjects An iterator to the objects in which the data found will be stored in this table
 * @param since The lower limit of the time interval
 * @param till The upper limit of the time interval
 */

void CondDBDataAccess::browseHistory (ICondDBDataIterator*& iterObjects,
				      const std::string&    folderName,
				      const CondDBKey since,
				      const CondDBKey till,
				      std::string tagName) const
    throw(CondDBException)
{
    int tag_id, folder_id, tbl_path, ftype;
    CondDBDataIterator *condIterator;
    relDBMgr->getFolderType(folderName, ftype);
    if (ftype == CondFolder::BLOBTAG)
    {
	relDBMgr->getFolderId(folderName, folder_id, tbl_path);
	relDBMgr->getTagId(tagName, tag_id);
	MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(tbl_path);
	MySqlResult *res = objectMgr->browseHistory(folder_id, since, till, tag_id);
	
	// Use mysql_fetch_row just to check if the result set is empty 
	if (res!=NULL && res->countRows()){
//	    Assert (res->countRows() > 0);
	    condIterator = new CondDBDataIterator(relDBMgr, res, folder_id);
	    iterObjects = static_cast<ICondDBDataIterator*>(condIterator);
	}
	else
	{
	    DebugMesg(CondDB, user, "No data found");
	}
    }
    else 
	cerr << "Folder is of wrong type!" << endl;
}



/**
 * Browse folder for a given time interval
 * @param folder The folder to be browsed
 * @param table Data found will be stored in this table
 * @param since The lower limit of the time interval
 * @param till The upper limit of the tim e interval
 */

void CondDBDataAccess::browseHistory(const string& folder,
				     ICondDBTable *table,
				     const CondDBKey& since,
				     const CondDBKey& till,
				     const string& id,
				     const string& selection,
				     const vector <string> *nullValues,
				     string tagName) const
    throw(CondDBException)
{
    int part_id, db_path, fld_id, ftype;
    relDBMgr->getFolderType(folder, ftype);
    if (ftype != CondFolder::BLOBTAG)
    {
	CondDBTable *tableOnl;
	relDBMgr->getFolderId(folder, fld_id, db_path);
	MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	part_id = onlineMgr->getPartitionId(since, fld_id);
	tableOnl = static_cast<CondDBTable*>(table);
	switch (ftype)
	{
	    case CondFolder::STRUCT:
	    {
		onlineMgr->browseOnl(since, till, fld_id, part_id, tableOnl, "", selection, nullValues);
		break;
	    }
	    case CondFolder::STRUCTID:
	    {
		onlineMgr->browseOnl(since, till, fld_id, part_id, tableOnl, id, selection, nullValues);
		break;
	    }
	    case CondFolder::STRUCTTAG:
	    {
		int tagId;
		relDBMgr->getTagId(tagName, tagId);
		onlineMgr->browseHist(since, till, fld_id, part_id, tagId, tableOnl, selection, nullValues);
		break;
	    }
	    default:
		cerr << "Folder type not handled by this function" << endl;
	}
    }
    else 
	cerr << "Folder is of wrong type!" << endl;
}

void CondDBDataAccess::browseId (ICondDBTimePath& path, 
				 std::string folder, 
				 std::string tagName) const
    throw(CondDBException)
{
    int tag_id, db_path, fld_id, ftype;
    relDBMgr->getFolderType(folder, ftype);
    relDBMgr->getFolderId(folder, fld_id, db_path);
    relDBMgr->getTagId(tagName, tag_id);
    switch (ftype)
    {
	case CondFolder::BLOBTAG:
	{
	    MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(db_path);
	    objectMgr->browseId(fld_id, tag_id, path);
	    break;
	}
	case CondFolder::STRUCTTAG:
	{
	    MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	    onlineMgr->browseId(fld_id, tag_id, path);
	    break;
	}
	default:
	{
	    cerr << "Wrong folder type" << endl;
	    break;
	}
    }
}


    
void CondDBDataAccess::tagFromTimePath (std::string folder, std::string tagName, ICondDBTimePath& path) const
	throw(CondDBException)
{
    int tag_id, db_path, fld_id, ftype;
    relDBMgr->getFolderType(folder, ftype);
    relDBMgr->getFolderId(folder, fld_id, db_path);
    relDBMgr->getTagId(tagName, tag_id);
    MySqlFolderMgr *relFolderMgr = relDBMgr->getFolderMgr();
    switch (ftype)
    {
	case CondFolder::BLOBTAG:
	{
	    MySqlObjectMgr *objectMgr = relDBMgr->getObjectMgr(db_path);
	    objectMgr->tagId(fld_id, tag_id, path);
	    relFolderMgr->addTag(fld_id, tag_id);
	    break;
	}
	case CondFolder::STRUCTTAG:
	{
	    MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	    onlineMgr->tagId(fld_id, tag_id, path);
	    relFolderMgr->addTag(fld_id, tag_id);
	    break;
	}
	default:
	{
	    cerr << "Wrong folder type" << endl;
	    break;
	}
    }

}

void CondDBDataAccess::getTableSchema (string folder, ICondDBTable *table)
	throw(CondDBException)
{
    int db_path, fld_id, ftype;
    relDBMgr->getFolderType(folder, ftype);
    switch (ftype)
    {
	case CondFolder::STRUCT:
	case CondFolder::STRUCTID:
	{
	    CondDBTable *tableOnl = static_cast<CondDBTable*>(table);
	    relDBMgr->getFolderId(folder, fld_id, db_path);
	    MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	    int part_id = onlineMgr->getPartitionId(0, fld_id);
	    onlineMgr->getSchema(fld_id, part_id, tableOnl);
	    break;
	}
	case CondFolder::STRUCTTAG:
	{
	    CondDBTable *tableOnl = static_cast<CondDBTable*>(table);
	    relDBMgr->getFolderId(folder, fld_id, db_path);
	    MySqlOnlineMgr *onlineMgr = relDBMgr->getOnlineMgr(db_path);
	    int part_id = onlineMgr->getPartitionId(0, fld_id);
	    onlineMgr->getSchema(fld_id, part_id, tableOnl, true);
	    break;
	}
	default:
	    break;
    }
}

/*
d_ULong
CondDBDataAccess::getTagId(const std::string& tagName) const
  throw(CondDBException)
{
  d_ULong tagId = 0;

  if ( tagName == "" )
    return 0; // to identify the HEAD
  
  if ( tagNameMap.count( tagName ) )
    tagId = tagNameMap[ tagName ];
  else
  {
    // tagName not found
    DebugMesg(CondDB, user, "tagName not found; alligning the map with"
	      " TagMgr (in case it changed in the meantime)");

    // try to update tagNameMap with TagMgr; maybe it was change

    tagNameMap.clear();
    condTagMgr->getTagNameMap(tagNameMap);

    // now see again if tagName is there
    if ( tagNameMap.count( tagName ) )
      tagId = tagNameMap[ tagName ];
    else
    { // in this case it is really wrong
      THROWEX("tagName not existing", 0);
    }
  }
  
  return tagId;
}
*/

 
// THE END

