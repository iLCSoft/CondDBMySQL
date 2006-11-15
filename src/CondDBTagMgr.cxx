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
/* $Id: CondDBTagMgr.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBTagMgr.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//



#include "ConditionsDB/SimpleTime.h"

#include "CondDBTagMgr.h"
#include "Debug.h"
#include "CondDBTag.h"
#include "CondFolderMgr.h"

#include "MySqlTagMgr.h"
#include "MySqlFolderMgr.h"
#include "MySqlTypes.h"
#include "MySqlObjectMgr.h"


// Constructor
CondDBTagMgr::CondDBTagMgr(MySqlDBMgr *dbMgr)
{
    relDBMgr = dbMgr;
}

// Destructor
CondDBTagMgr::~CondDBTagMgr()
{
}


void
CondDBTagMgr::init()
    throw(CondDBException)
{
    relTagMgr = relDBMgr->getTagMgr();
    relFolderMgr = relDBMgr->getFolderMgr();
    DebugMesg(CondDB, devl, "CondDBTagMgr::init - Completed");
}


void
CondDBTagMgr::createCondDBTag( const string& name,
			       const string  description) 
    throw(CondDBException)
{
    string msg("Creating tag "); msg += name;
    DebugMesg(CondDB, devl, msg);
	
    // check whether "name" already exists
	
    if ( relTagMgr->exist(name) ) {
	msg = "A tag already exists with name "; msg += name;
	THROWEX( msg, 0);
    }
    
    // otherwise create it
    relTagMgr->storeTag(name, description);
    msg = "Tag "; msg += name; msg += " successfully created";
    DebugMesg(CondDB, user, msg);
}


void
CondDBTagMgr::deleteCondDBTag( const string& tagName ) 
    throw(CondDBException)
{
    relTagMgr->deleteTag(tagName);
    string msg = "Tag "; msg += tagName; msg += " successfully removed";;
    DebugMesg(CondDB, user, msg);
}
  

void
CondDBTagMgr::getCondDBTag( const string& tagName,
			    ICondDBTag*& tag) const
    throw(CondDBException)
{
    string msg;

    MySqlResult *res = relTagMgr->find(tagName);

    if ( res->countRows() == 0 ) {
	msg = "No tag exists with name "; msg += tagName;
	THROWEX( msg, 0 );
    }
    
    msg = "Found tag "; msg += tagName;
    DebugMesg(CondDB, devl, msg);
    CondDBTag *newTag = new CondDBTag(this, res);
    tag = newTag;
    Assert ( tag != 0 );
}

bool CondDBTagMgr::getCondDBTag (const string& folder, vector <string>& tagNames)
    throw (CondDBException)
{
    string msg;
    
    int folderId, dbase;

    relDBMgr->getFolderId(folder, folderId, dbase);
        
    MySqlResult *res = relTagMgr->findFolder(folderId);
    
    if ( res->countRows() == 0 ) {
	msg = "No tag exists for folder "; msg += folder;
	return false;
    }
    
    msg = "Found tag(s) for folder "; msg += folder;
    
    do {
	tagNames.push_back( res->getField(0) );
    } while ( res->nextRow() );

    delete res;
    
    DebugMesg(CondDB, devl, msg);

    return true;
}

void CondDBTagMgr::getAllCondDBTag( vector<string>& allTag ) const
    throw(CondDBException)
{
    MySqlResult *res = relTagMgr->browseAll();
    Assert( res->countRows() != 0 );

    do {
	allTag.push_back( res->getField(TAG_NAME) );
    } while ( res->nextRow() );
    
    DebugMesg(CondDB, user, "getAllCondDBTag completed");
}


void
CondDBTagMgr::tag( const string& folderName,
		   const string& tagName,
		   string usingTagName)
    throw(CondDBException)
{
    int folderId, tblPath;
    int tagId = relTagMgr->getId(tagName);
    relDBMgr->getFolderId(folderName, folderId, tblPath);
    MySqlResult *res =relFolderMgr->find(folderId, MySqlFolderMgr::FolderSet);
    if (res->countRows())
    {
	res = relFolderMgr->browseChilds(folderId, MySqlFolderMgr::Any);
	if ( res->countRows()!=0 )
	    do {
		tag(res->getField(FOLDER_PATHNAME), tagName, usingTagName);
	    } while ( res->nextRow() );
    }
    else
    {
	MySqlObjectMgr *objM = relDBMgr->getObjectMgr(tblPath);
	if (usingTagName=="") {
	    int oldId = relTagMgr->getId(usingTagName);
	    objM->addTag(folderId, tagId, oldId);
	}
	else {
	    objM->addTag(folderId, tagId);
	}
	relFolderMgr->addTag(folderId, tagId);
    }
}


void
CondDBTagMgr::untag( const string& folderName,
		     const string& tagName)
    throw(CondDBException)
{
    int tagId, folderId, dbId;
    tagId = relTagMgr->getId(tagName);
    relDBMgr->getFolderId(folderName, folderId, dbId);
    MySqlObjectMgr *objM = relDBMgr->getObjectMgr(dbId);
    objM->removeTag(folderId, tagId);
    relFolderMgr->removeTag(folderId, tagId);
}


void
CondDBTagMgr::changeTagName( const string& oldName,
			     const string& newName) const
    throw(CondDBException)
{
    if ( relTagMgr->exist(newName) ) {
	string msg = "A tag already exists with name "; msg += newName;
	THROWEX( msg, 0);
    }
    relTagMgr->renameTag(oldName, newName);
}

/**
 * Gets a vector of all folders tagged with given tag
 * and coresponding vector of creation times
 */
void
CondDBTagMgr::getAllTaggedFolder( const string& tagName,
			          vector<string>& allFolder,
			          vector<SimpleTime>& allApplicationTime)  const
    throw(CondDBException)
{
    MySqlResult *res = relFolderMgr->browseTaggedFolders(tagName);
    if ( res->countRows() == 0 ) {
	// std::string msg = "No folder tagged with "+tagName+" tag";
	// THROWEX(msg, 0);
	return;
    }
    do {
	allFolder.push_back ( res->getField(FTAG_PATH) );
	allApplicationTime.push_back ( res->getTimeStampField(FTAG_INSERT_T) );
    } while ( res->nextRow() );
}


// THE END






