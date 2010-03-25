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
/* $Id: MySqlDataMgr.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlDataMgr.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef MYSQL_DATAMGR_H
#define MYSQL_DATAMGR_H

// C++ include files
#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"
#include "CondDBObject.h"

#include "MySqlConnection.h"

#define DATA_ID        0
#define DATA_DESC      1
#define DATA_BLOCK     2


class MySqlDataMgr : public MySqlConnection {

public:
    MySqlDataMgr(MySqlDBMgr *dbMgr,
		 MySqlHandle *mysqlH);
    
    MySqlDataMgr(MySqlDBMgr *dbMgr);
    
    ~MySqlDataMgr();

    MySqlResult *fetch(int folderId, int partId, int oKey)
	throw(CondDBException);

    int store(int folderId, int partId,
	      string& description, string& blob)
	throw(CondDBException);

    void changeDatabase(const string& srvname, 
			const string& username,
			const string& password,
			const string& dbname,
			int dbpath)
	throw(CondDBException);

private:

    void escapeBinaryString(string& target,
			    const string& source)
	throw(CondDBException);

    MySqlDBMgr *databaseMgr;
    string tableName;
};


#endif /* MYSQL_DATAMGR_H */













