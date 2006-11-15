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
/* $Id: MySqlTagMgr.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlTagMgr.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef MYSQL_TAGMGR_H
#define MYSQL_TAGMGR_H

// C++ include files
#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"
#include "CondDBObject.h"

// These macros must be syncronized with the queries code
#define TAG_ID        0
#define TAG_INSERT_T  1
#define TAG_NAME      2
#define TAG_DESC      3

#define FTAG_PATH     0
#define FTAG_NAME     1
#define FTAG_INSERT_T 2

#include "MySqlConnection.h"
#include "MySqlDBMgr.h"


class MySqlTagMgr : public MySqlConnection {

public:
    MySqlTagMgr(MySqlDBMgr *dbMgr,
		MySqlHandle *mysqlH);

    MySqlTagMgr(MySqlDBMgr *dbMgr);

    ~MySqlTagMgr();

// Get a tag by ID
    MySqlResult *fetch(int id)
	throw(CondDBException);
// Find named tag
    MySqlResult *find(const string& name)
	throw(CondDBException);
    MySqlResult *browseAll()
	throw(CondDBException);
// Get ID given a name
    int getId(const string& tagName)
	throw(CondDBException);
// Check if exists the named tag
    bool exist(const string& name)
	throw(CondDBException);

// Get tag names for given folder Id    
    MySqlResult *findFolder (int folderId)
	throw(CondDBException);

// Stores, updates, renames and removes
    void storeTag(const string& name, const string& desc)
	throw(CondDBException);

    void updateTag(int id, const string& name, const string& desc)
	throw(CondDBException);

    void deleteTag(const string& name)
	throw(CondDBException);

    void renameTag(const string& oldname, const string& newname)
	throw(CondDBException);


private:
    MySqlDBMgr *databaseMgr;
};

#endif  /* MYSQL_TAGMGR_H */




