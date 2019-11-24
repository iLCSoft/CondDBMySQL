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
/* $Id: MySqlOnlineMgr.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlObjectMgr.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef MYSQL_ONLINEMGR_H
#define MYSQL_ONLINEMGR_H

// C++ include files
#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"
#include "CondDBTable.h"
#include "CondDBObject.h"
#include "CondDBTimePath.h"

#include "MySqlConnection.h"

class MySqlOnlineMgr : public MySqlConnection {

public:
    MySqlOnlineMgr(MySqlDBMgr *dbMgr,
		   MySqlHandle *mysqlH);

    MySqlOnlineMgr(MySqlDBMgr *dbMgr);

    ~MySqlOnlineMgr();

    void findOnl(CondDBKey time,
		 int fldId,
		 int partId,
		 CondDBTable *table,
		 std::string Id,
		 std::string selection,
		 const std::vector <std::string> *nullValues)
	;

    void findTag(CondDBKey time,
		 int fldId,
		 int partId,
		 int tagId, CondDBTable *table,
		 std::string selection,
		 const std::vector <std::string> *nullValues)
	;

    void browseOnl(CondDBKey since, CondDBKey till,
		   int fld_id, int part_id,
		   CondDBTable *table,
		   std::string Id,
		   std::string selection,
		   const std::vector <std::string> *nullValues)
	;

    void browseHist(CondDBKey since, CondDBKey till,
		    int fld_id, int part_id, int tag_id,
		    CondDBTable *table,
		    std::string selection,
		    const vector <string> *nullValues)
	;

    void browsePoint(CondDBKey time,
		     int fldId,
		     int partId,
		     CondDBTable *table,
		     std::string selection,
		     const std::vector <std::string> *nullValues)
	;

    void browseTag(int fldId,
		   int tagId,
		   CondDBTable *table,
		   std::string selection,
		   const std::vector <std::string> *nullValues)
	;

    void browseAll(int fldId,
		   CondDBTable *table,
		   std::string selection,
		   const std::vector <std::string> *nullValues)
	;

    void createTablesOnl(int folderId, vector <string>& names, vector <CondDBTable::cdb_types>& types, bool Id)
	;

    void createTablesTag(int folderId, vector <string>& names, vector <CondDBTable::cdb_types>& types, bool Id)
	;

    void deleteTables(int folderId)
	;

    int createPartition(CondDBKey since, CondDBKey till,
			int folderId, int partId)
	;

    int getDatabaseId();

    int getPartitionId(CondDBKey point, int folderId)
	;

    void storeOnl(int fldId, int partId, CondDBTable *table)
	;

    void storeOnlId (int fldId, int partId, CondDBTable *table)
	;

    void storeVer(int fldId, int partId, CondDBTable *table)
	;

    void store(CondDBKey since, CondDBKey till, int folderId, int dbId, int datId)
	;

    void changeDatabase(const string& srvname,
			const string& username,
			const string& password,
			const string& dbname,
			int dbpath)
	;

    void getSchema(int fldId, int partId, CondDBTable *table, bool versions = false);

    void browseId (int folderId, int tagId, ICondDBTimePath& path)
	;

    void tagId (int folderId, int tagId, const ICondDBTimePath& path)
	;

private:

    void insert(CondDBKey since, CondDBKey till, int folderId, int layer, int tblId, int datId)
	;

    void insertOnl(CondDBKey since, CondDBKey till,
		    int fldId, int tblId, string& data, string& name)
	;

    void insertData(int fldId, int partId, int datId, string& data, string& name)
	;

    void makeTable(MySqlResult *res, CondDBTable *table, int fldId, int partId, const std::vector <std::string> *nullValues, int version = 0)
	;

    void escapeBinaryString(string& target, const string& source)
	;

    void breakUp (string fname, string& name, string &type);

    void typeNameToEnum(string type, CondDBTable::cdb_types& colType);

    bool compareNull(CondDBTable::cdb_types&type, string& value, string& null);

    MySqlDBMgr *databaseMgr;
};


#endif
