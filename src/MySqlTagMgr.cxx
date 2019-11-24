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
/* $Id: MySqlTagMgr.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlTagMgr.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


//
// C++ include files
//

#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"
#include "CondDBObject.h"
#include "MySqlTagMgr.h"
#include "MySqlTypes.h"
#include "boot-sql.h"

MySqlTagMgr::MySqlTagMgr(MySqlDBMgr *dbMgr,
			 MySqlHandle *mysqlH)
    : MySqlConnection(mysqlH)
{
    databaseMgr = dbMgr;
}

MySqlTagMgr::MySqlTagMgr(MySqlDBMgr *dbMgr)
{
    databaseMgr = dbMgr;
}

MySqlTagMgr::~MySqlTagMgr()
{
}



// Tag management function

/**
 * Fetch tag with tag ID
 */
MySqlResult *MySqlTagMgr::fetch(int id)

{
    MYSQLSTREAM query;
    query << "SELECT * FROM " TAGS_TBL << " WHERE tag_id=" << id;
    return select(query);
}

/**
 * Find named tag
 */
MySqlResult *MySqlTagMgr::find(const string& name)

{
    MYSQLSTREAM query;
    query << "SELECT * FROM " TAGS_TBL << " WHERE(tname=\'" << name << "\')";
    return select(query);
}

int MySqlTagMgr::getId(const string& tagName)

{
    if (tagName=="") return 0;
    MySqlResult *res = find(tagName);
    int tag_id;
    if ( res->countRows() == 0 )
	tag_id = -1;
    else
	tag_id = res->getIntField(TAG_ID);
    delete res;
    return tag_id;
}

/**
 * Find tags for a given folder Id
 */

MySqlResult *MySqlTagMgr::findFolder (int folderId)

{
    MYSQLSTREAM query;
    query << "SELECT * FROM tag2folder_tbl WHERE fld_id=" << folderId;
    MySqlResult *res=select(query);
    if (res->countRows()==0)
	return res;
    else
    {
	query << "SELECT tname FROM tags_tbl WHERE tag_id=";
	query << res->getField(0);
	while (res->nextRow())
	    query << " or tag_id=" << res->getField(0);
    }
    delete res;
    return select(query);
}

/**
 * Check if exists the named tag
 */
bool MySqlTagMgr::exist(const string& tagName)

{
    bool exists = false;
    MySqlResult *res = find(tagName);
    if ( res->countRows() > 0 )
	exists = true;
    delete res;
    return exists;
}

/**
 * Browse all currently defined tags
 */
MySqlResult *MySqlTagMgr::browseAll()

{
    MYSQLSTREAM query;
    query << "SELECT * FROM " TAGS_TBL;
    return select(query);
}

/**
 * Store a tag (same as defining a tag)
 */
void MySqlTagMgr::storeTag(const string& name, const string& desc)

{
    MYSQLSTREAM query;
    query << "INSERT INTO " TAGS_TBL " (insert_t,tname,tdesc) VALUES(NOW(),\'"
	  << name << "\',\'" << desc << "\')";
    execute(query);
}


/**
 * Remove a tag from tags table (same as undefine a tag)
 */
void MySqlTagMgr::deleteTag(const string& name)

{
    // Should we unlink the tags from objects?
    // Leaving the objects links has no effect other than
    // making queries slow as tables will grow.
    // On the other hand removing all links can be a time
    // consuming task. That could also be done as a
    // administrative task when nobody is using the database.
    MYSQLSTREAM query;
    query << "DELETE FROM " TAGS_TBL " WHERE(tname=\'" << name << "\')";
    execute(query);
}

/*
 * Rename a tag
 */
void MySqlTagMgr::renameTag(const string& oldname,
			    const string& newname)

{
    MYSQLSTREAM query;
    query << "UPDATE " TAGS_TBL " SET tname=\'" << newname
	  << "\' WHERE(tname=\'" << oldname << "\')";
    execute(query);
}


// THE END
