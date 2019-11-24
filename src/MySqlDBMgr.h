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
/* $Id: MySqlDBMgr.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlDBMgr.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef MYSQL_DBMGR_H
#define MYSQL_DBMGR_H

// C++ include files
#include <string>
#include <map>
#include "Debug.h"

#include "ConditionsDB/CondDBKey.h"
#include "CondDBObject.h"

#include "MySqlConnection.h"

class MySqlTagMgr;
class MySqlFolderMgr;
class MySqlObjectMgr;
class MySqlDataMgr;
class MySqlOnlineMgr;

class MySqlDBMgr : public MySqlConnection {

 public:
    MySqlDBMgr();

    ~MySqlDBMgr();

    // Transaction handling
    void commit();
    void startUpdate();
    void startRead();

    // Get the time maintained by the DBMS, which should be used for timestamps
    string getDBName() { return databaseName; }

    string getSrvName() { return serverName; }

    // The connect() method which establishes the DBMS connection
    // Defined in the superclass
    void init(string& initstring)
	;

    // Open and create a new database
    bool openDatabase(const string& dbname="");

    void createDatabase(const string& dbname="")
	;

    // Get data path information
    void getFolderId(string folder, int& folder_id, int& db_path_id)
	;

    void getDBId(int fldId, int& dbId)
	;

    void getFolderType(string folderName, int& ftype)
	;

    void getTagId(const string& tagName, int& tagId)
	;

    int createDBPath(const string& srvName, const string& dbName)
	;

    void getDBPath(int db_id, string& db_name, string& srv_name)
	;

    // return the specific managers
    MySqlDataMgr*   getDataMgr(int db_path)
	;

    MySqlObjectMgr* getObjectMgr(int db_path)
	;

    MySqlOnlineMgr* getOnlineMgr(int db_path)
	;

    MySqlFolderMgr* getFolderMgr();

    MySqlTagMgr*    getTagMgr();

private:

    // methods

    // data
    string serverName;
    string databaseName;
    string userName;
    string password;
    unsigned int port;

    // Transient foldernames and tagnames
//    vector< map<string,int> > folderMap;
//    vector< map<string,int> > tagMap;

    // Pointers for lists of or simple child managers
    MySqlObjectMgr *relObjectMgr;
    MySqlOnlineMgr *relOnlineMgr;
    MySqlDataMgr   *relDataMgr;
    MySqlFolderMgr *relFolderMgr;
    MySqlTagMgr    *relTagMgr;
};


#endif /* MYSQL_DBMGR_H */
