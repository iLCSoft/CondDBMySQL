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
/* $Id: MySqlFolderMgr.cxx,v 1.2 2007-03-08 17:01:05 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlFolderMgr.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


// C++ include files
#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"
#include "CondDBObject.h"

#include "MySqlFolderMgr.h"
#include "MySqlTypes.h"
#include "boot-sql.h"

/**
 * MySqlFolderMgr constructor that will reuse an already opened db handle
 */

MySqlFolderMgr::MySqlFolderMgr(MySqlDBMgr *dbMgr,
			       MySqlHandle *mysqlH)
    : MySqlConnection(mysqlH)
{
    databaseMgr = dbMgr;
}


/**
 * MySqlFolderMgr constructor that will build its own db connection handle
 */

MySqlFolderMgr::MySqlFolderMgr(MySqlDBMgr *dbMgr)
{
    databaseMgr = dbMgr;
}

/**
 * MySqlFolder manager destructor is a nop
 */

MySqlFolderMgr::~MySqlFolderMgr()
{
}

/*
 * Folder functions
 */

/**
 * search for a single folder or folderset that exacly matches the name 
 * in 'folder'.
 */

MySqlResult *MySqlFolderMgr::find(const string& pathname, folder_type type)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT * FROM folders_tbl WHERE fpath=\'" << pathname<< "\'";
    if (type!=Folder && type!=Any)
	query << " && is_set=" << type;
    return select(query);
}

/**
 * The same as above but for a given ID instead of a pathname.
 */

MySqlResult *MySqlFolderMgr::find(int folderId, folder_type type)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT * FROM " FOLDERS_TBL " WHERE fld_id=" << folderId;
    if (type!=Folder && type != Any)
	query << " && is_set=" << type;
    return select(query);
}

// search for a single folder (or folderset) that exacly matches the name
// in 'folder'.

/**
 * Search for a set of folders (or folderset) matching something like 
 * 'basename%'.
 */

MySqlResult *MySqlFolderMgr::browse(const string& basename, folder_type type)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT * FROM folders_tbl ";
    if ( basename != "" ) {
	query << " WHERE fname LIKE \'" << basename << "%\'";
	if ( type != Any ) {
	    query << " && is_set=" << type;
	}
    }
    else if ( type != Any ) {
	query << " WHERE is_set=" << type;
    }

    return select(query);
}


/**
 * Search for a set of child folders (or child folderset) from the parent
 * whose id is 'parentId'.
 */

MySqlResult *MySqlFolderMgr::browseChilds(int parentId, folder_type type)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT * FROM folders_tbl WHERE fparent=" << parentId; 
    
    if ( type != Any ) { 
	query << " && is_set=" << type;
    }
    return select(query);
}


/**
 * Check if the named folder or folderSet exists.
 */

bool MySqlFolderMgr::exist(const string& fullPath, folder_type type)
    throw(CondDBException)
{
    bool exist = false;
    MySqlResult *res = find(fullPath, type);

    if ( res->countRows() )
	exist = true;

    delete res;
    return exist;
}

/**
 * Check if the folder or folderSet with th given id exists.
 */

bool MySqlFolderMgr::exist(int id, folder_type type)
    throw(CondDBException)
{
    bool exist = false;
    MySqlResult *res = find(id, type);

    if ( res->countRows() )
	exist = true;

    delete res;
    return exist;
}
  

/**
 * Store a folder (or folderset)
 */

int MySqlFolderMgr::store(const string& pathName,
			  const string& description,
			  const string& attributes,
			  int parentId,
			  int dbId,
			  folder_type type,
			  int ddtype)
    throw(CondDBException)
{ 
    Assert ( type != MySqlFolderMgr::Any );
    // check if parent exists as a folderSet
    if ( pathName == "/" ) {
	Assert ( type == MySqlFolderMgr::FolderSet );
    }
    MYSQLSTREAM query;
    query << "INSERT INTO folders_tbl\n" 
	" (fparent,fpath,fdesc,fattr,is_set,db_id,ddtype) VALUES(" 
	  << parentId << ",\'" << pathName << "\',\'" 
	  << description << "\',\'" << attributes << "\'," 
	  << static_cast<int>(type) << "," << dbId << "," << ddtype << ")";

    execute(query);
    query << "SELECT LAST_INSERT_ID()";
    MySqlResult *res = select(query);
    int fldId = res->getIntField(0);
    delete res;
    return fldId;
}

/**
 * Delete a given folder or folderSet
 */

void MySqlFolderMgr::deleteFolder(int folderId)
    throw(CondDBException)
{
    MySqlResult *res = browseChilds(folderId, MySqlFolderMgr::Any);
    if (res->countRows() == 0)
    {
	MYSQLSTREAM query;
	// Should instead move the record to a deleted_folder_table for 
	// later delete every related objects and data
	query << "DELETE FROM folders_tbl\n  WHERE fld_id=" << folderId;
	execute(query);
    }
    else
	cout << "# Folder not empty" << endl;
    delete res;
}

//
// Tag <-> folder association
//

/**
 * Check if the folder with id 'folderId' is tagged with the tag whose
 * id is 'tagId'
 */
bool MySqlFolderMgr::isTagged(int folderId, int tagId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT * FROM " TAG2FOLDER_TBL " WHERE fld_id=" 
	  << folderId << " AND tag_id=" << tagId;
    MySqlResult *res = select(query);
    if ( res->countRows() > 0 )
	return true;
    else
	return false;
}

/**
 * Remove the tag identified by 'tagId' from the folder given by 'folderId'.
 * This method must be used in conjunction with the method 
 * of the MySqlObjectMgr with the same name.
 */
void MySqlFolderMgr::removeTag(int folderId, int tagId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    if ( !isTagged( folderId, tagId) ) {
      query << "DELETE FROM " TAG2FOLDER_TBL " WHERE fld_id="
           << folderId << " AND tag_id=" << tagId;
      execute(query);
    }
    else {
      MYSQLSTREAM fquery ;
      fquery << "SELECT fpath FROM folders_tbl\n  WHERE fld_id=" << folderId;
      MySqlResult *fres = select(fquery);
      // check consistency
      if ( fres->countRows() == 0 ) {
       string msg = "Folder not found in folders_tbl with folder id : ";
       msg += folderId;
       THROWEX( msg, 0); // should not happen since checked before
      }
      MYSQLSTREAM tquery;
      tquery << "SELECT tname FROM tags_tbl\n  WHERE tag_id=" << tagId;
      MySqlResult *tres = select(tquery);
      if ( tres->countRows() == 0  ) {
       string msg = "Tag not found in tags_tbl with tag id : ";
       msg += tagId;
       THROWEX( msg, 0); // should not happen since checked before
      }
      // tag status should have been checked before,
      // throw an exception if an already tagged folder is found
      string msg = "CondDBMySQL: MySqlFolderMgr::addTag: folder ";
      msg += fres->getField(0);
      msg += " was already tagged with tag ";
      msg += tres->getField(0);
      delete fres;
      delete tres;
      THROWEX( msg, 0); // should not happen since it should have been checked before
    }
}


/**
 * Associate the tag given by 'tagId' with the folder given by 'folderId'.
 * This method must be used in conjunction with the method 
 * of the MySqlObjectMgr with the same name.
 */
void MySqlFolderMgr::addTag(int folderId, int tagId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "INSERT INTO " TAG2FOLDER_TBL " (tag_id,fld_id,insert_t) VALUES(" 
	  << tagId << "," << folderId << ",NOW())"; 
    execute(query);
}

MySqlResult *MySqlFolderMgr::browseTaggedFolders(const string& tagName)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT f.fpath, t.tname, l.insert_t\n"
	"  FROM " FOLDERS_TBL " AS f, " TAG2FOLDER_TBL " AS l, " TAGS_TBL " AS t\n"
	"  WHERE t.tname=\'" << tagName << "\' AND l.fld_id=f.fld_id AND l.tag_id=t.tag_id";
    return select(query);
}

int MySqlFolderMgr::getFolderType(const string& fullPathName)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT ddtype FROM folders_tbl WHERE fpath=\'" << fullPathName << "\'";
    MySqlResult *res = select(query);
    int ftype=0;
    if (res->countRows()!=0)
       ftype=res->getIntField(0);
    delete res;
    return ftype;
}


// END





