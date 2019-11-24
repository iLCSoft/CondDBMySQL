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
/* $Id: MySqlObjectMgr.h,v 1.2 2009-02-02 14:52:44 meyern Exp $ */

// $HEAD 10
//
// File: "MySqlObjectMgr.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef MYSQL_OBJECTMGR_H
#define MYSQL_OBJECTMGR_H

// C++ include files
#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"
#include "CondDBObject.h"
#include "CondDBTimePath.h"

#include "MySqlConnection.h"

// These macros must agree with the queries code.

#define OBJKEY_ID        0
#define OBJKEY_INSERT_T  1
#define OBJKEY_SINCE_T   2
#define OBJKEY_TILL_T    3
#define OBJKEY_LAYER     4
#define OBJKEY_DB_ID     5
#define OBJKEY_DAT_ID    6
#define OBJKEY_IS_HEAD   7

class MySqlObjectMgr : public MySqlConnection {

public:
    MySqlObjectMgr(MySqlDBMgr *dbMgr,
		   MySqlHandle *mysqlH);

    MySqlObjectMgr(MySqlDBMgr *dbMgr);

    ~MySqlObjectMgr();

    MySqlResult *find(CondDBKey point, int folderId, int tagId)
	;

    MySqlResult *browseAll(int folderId)
	;

    MySqlResult *browseAtPoint(CondDBKey point, int folderId)
	;

    MySqlResult *browseInInterval(CondDBKey begin, CondDBKey end, int folderId)
	;

    MySqlResult *browseTagged(int folderId, int tagId)
	;

    MySqlResult *browseHistory(int folderId, CondDBKey since, CondDBKey till, int tagId)
	;

    void browseId (int folderId, int tagId, ICondDBTimePath& path)
	;

    void tagId (int folderId, int tagId, const ICondDBTimePath& path)
	;

    void addTag(int folderId, int newTagId, int oldTagId=0)
	;

    void removeTag(int folderId, int tagId)
	;

    void createTables(int folderId)
	;

    void deleteTables(int folderId, int folderType)
	;

    int createPartition(CondDBKey since, CondDBKey till,
			int folderId, int partId)
	;

    int getDatabaseId();

    int getDBId(CondDBKey point, int folderId)
        ;

    int getPartitionId(CondDBKey point, int folderId)
	;

    void store(CondDBKey since, CondDBKey till, int folderId, int tblId, int datId)
	;

    void changeDatabase(const string& srvname,
			const string& username,
			const string& password,
			const string& dbname,
			int dbpath)
	;

private:

    void insert(CondDBKey since, CondDBKey till,
	       int folderId, int layer, int tblId, int datId)
	;

    MySqlDBMgr *databaseMgr;
};


#endif /* MYSQL_OBJECTMGR_H */
