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
/* $Id: MySqlDBMgr.cxx,v 1.2 2009-11-30 16:34:22 meyern Exp $ */

// $HEAD 10
//
// File: "MySqlDBMgr.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//

#include <time.h>

// Error codes and messages for exceptions
#define EXP_BADTAGNAME_CODE 2000
#define EXP_BADTAGNAME_MESS "No such tag:"

// C++ include files
#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"
#include "CondDBObject.h"

#include "MySqlDBMgr.h"
#include "MySqlObjectMgr.h"
#include "MySqlDataMgr.h"
#include "MySqlFolderMgr.h"
#include "MySqlTagMgr.h"
// WHAT MATTERS

#include "MySqlOnlineMgr.h"

#include "MySqlTypes.h"

#include "boot-sql.h"

// definitions from the configure script

#ifdef HAVE_CONFIG_H
# include "config.h"
#ifdef HAVE_STDLIB_H
extern "C"
{
#include<stdlib.h>
}
#include<iostream>
#endif
#endif

/*
#ifdef HAVE_CONFIG_H
# include "config.h"

#ifdef HAVE_UNISTD_H
# ifdef HAVE_PWD_H
#  ifdef HAVE_SYS_TYPES_H
extern "C"
{
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
}
#  endif
# endif
#endif
#endif
*/

#define DEFAULT_DB_NAME "test_conddb02"


MySqlDBMgr::MySqlDBMgr()
{
    // This one would be called implicity
    // MySqlConnection();
    relObjectMgr = 0;
    relOnlineMgr = 0;
    relDataMgr = 0;
    relFolderMgr = 0;
    relTagMgr = 0;
}


MySqlDBMgr::~MySqlDBMgr()
{
    if ( relObjectMgr != 0 )
	delete relObjectMgr;
    if ( relOnlineMgr != 0)
	delete relOnlineMgr;
    if ( relDataMgr != 0 )
	delete relDataMgr;
    if ( relFolderMgr != 0 )
	delete relFolderMgr;
    if ( relTagMgr != 0 )
	delete relTagMgr;
}

/**
 * Commit the current transation
 * This is a dummy method in the MySQL implementation based in the MyISAM tables
 */

void MySqlDBMgr::commit() {}

/**
 * Start an update transation
 * This is a dummy method in the MySQL implementation based in the MyISAM tables
 */

void MySqlDBMgr::startUpdate() {}

/**
 * Start a read transation
 * This is a dummy method in the MySQL implementation based in the MyISAM tables
 */

void MySqlDBMgr::startRead() {}


/**
 * Initialize the connection
 */

void MySqlDBMgr::init(string& databaseInfo)

{

// This is needed and are local connections
//relFolderMgr => MySqlFolderMgr
    relFolderMgr = getFolderMgr();
// relTagMgr => MySqlTagMgr
    relTagMgr = getTagMgr();

    DebugMesg(CondDB, user, "databaseInfo = " << databaseInfo);

    unsigned int sep = databaseInfo.find_first_of(":");
    int end = databaseInfo.size();
    serverName = databaseInfo.substr(0, sep);
    databaseInfo = databaseInfo.substr(sep + 1, end);

    DebugMesg(CondDB, user, "  serverName = " << serverName);

    sep = databaseInfo.find_first_of(":");
    end = databaseInfo.size();
    databaseName = databaseInfo.substr(0, sep);
    databaseInfo = databaseInfo.substr(sep + 1, end);
    if ( databaseName == "" ) databaseName = DEFAULT_DB_NAME;

    DebugMesg(CondDB, user, "databaseName = " << databaseName);

    sep = databaseInfo.find_first_of(":");
    end = databaseInfo.size();
    userName = databaseInfo.substr(0, sep);
    //    password = databaseInfo.substr(sep + 1, end);
    //changes made to allow the specification of the port
    databaseInfo = databaseInfo.substr(sep + 1, end);
#ifdef HAVE_STDLIB_H
    if (userName.size() == 0)
      userName = getenv("USER");
#endif
    DebugMesg(CondDB, user, "userName = " << userName);

    sep = databaseInfo.find_first_of(":");
    // if there are no more separators
    //it means that there is no port specified...
    if (sep == std::string::npos){
      password = databaseInfo;
      DebugMesg(CondDB, user, "passwd = " << password);
      char *aux;
#ifdef HAVE_STDLIB_H
      aux= getenv("CONDDB_PORT");
      // if there is no port defined we must use the default
      if (aux !=  NULL)
	port = atoi(aux);
#endif

   }
    else {
      // there si a separator...let's get the port
      password = databaseInfo.substr(0, sep);
      port = atoi((databaseInfo.substr(sep + 1, end)).c_str());
    }


/*
#ifdef HAVE_UNISTD_H
# ifdef HAVE_PWD_H
#  ifdef HAVE_SYS_TYPES_H

    if (userName.size() == 0)
    {
	struct passwd * userInfo = getpwuid(geteuid());
	userName = userInfo->pw_name;
    }
#  endif
# endif
#endif
*/
//    DebugMesg(CondDB, user, "    userName = " << userName);
    DebugMesg(CondDB, user, "    password = " << password);
    if (port != 0){
      DebugMesg(CondDB, user, "    port = " << port);
      connect(serverName, userName, password,port);
    }
    else{
      DebugMesg(CondDB, user, "    port --> using default ");

      ///////////////////////////////////////////////////
      connect(serverName, userName, password);
    }
}


/**
 * Just open an existing database
 */

bool MySqlDBMgr::openDatabase(const string& dbname)
{
    if ( dbname=="" ) {
	return changeDB(databaseName);
    }
    else {
	databaseName = dbname;
	return changeDB(dbname);
    }
}


/**
 * Build a new database
 */

void MySqlDBMgr::createDatabase(const string& dbname)

{
    if ( dbname!="" )
    {
	databaseName = dbname;
    }

    MYSQLSTREAM query;
    query << "CREATE DATABASE " << databaseName.c_str();
    execute(query);

    changeDB(databaseName);

    // Create the minimal set of tables
    // Tables from DBL1
    query << "CREATE TABLE " FOLDERS_TBL " " FOLDERS_TBL_SCHEMA;
    execute(query);

    query << "CREATE TABLE " DATABASES_TBL " " DATABASES_TBL_SCHEMA;
    execute(query);
    query << "INSERT INTO " DATABASES_TBL " (db_id,dbname,srvname) VALUES(0,\'"
	  << databaseName << "\', \'" << serverName << "\')";
    execute(query);

    // TAGS
    query << "CREATE TABLE " TAGS_TBL " " TAGS_TBL_SCHEMA;
    execute(query);
    query << "INSERT INTO tags_tbl (tname,tattr,tdesc) "
	"VALUES('HEAD','NO ATTR','Latest objects version')";
    execute(query);
    query << "CREATE TABLE " TAG2FOLDER_TBL " " TAG2FOLDER_TBL_SCHEMA;
    execute(query);

}



/**
 * Tags related stuff
 */

void MySqlDBMgr::getTagId(const string& tagName, int& tag_id)

{
    tag_id = relTagMgr->getId(tagName);
    if (tag_id<0) {
// CondDBLog
      std::string message = EXP_BADTAGNAME_MESS + tagName;
	if (cdbLog->isUsable())
	{
	    cdbLog->StoreLog(EXP_BADTAGNAME_MESS ,  __FILE__  , __LINE__ );
	}
	THROWEX( message, EXP_BADTAGNAME_CODE);
    }
}


/**
 * Create a database path entry in database table
 */

int MySqlDBMgr::createDBPath(const string& srvName, const string& dbName)

{
    MySqlResult *res;
    MYSQLSTREAM query;
    query << "SELECT db_id\n  FROM " DATABASES_TBL "\n"
	"  WHERE dbname=\'" << dbName << "\' AND srvname=\'" << srvName << "\'";
    res = select(query);

    if ( res->countRows() == 0 ) {
	delete res;

	// create a temporary db connection to create a decoupled database
	MySqlDBMgr auxMgr;
	auxMgr.connect(srvName, userName, password);
	query << "CREATE DATABASE IF NOT EXISTS " << dbName.c_str();
	auxMgr.execute(query);

	// create a new database coordinate entry
	query << "INSERT INTO " DATABASES_TBL " (dbname,srvname) VALUES(\'"
	      << dbName << "\',\'" << srvName << "\')";
	execute(query);

	// get its ID
	query << "SELECT LAST_INSERT_ID()";
	res = select(query);
    }
    int dbId = res->getIntField(0);
    delete res;
    return dbId;
}


/**
 * Get database path information from database table based in dbID
 */

void MySqlDBMgr::getDBPath(int db_id, string& db_name, string& srv_name)

{
    MYSQLSTREAM query;
    query << "SELECT db_id, dbname, srvname\n"
	"  FROM " DATABASES_TBL "\n"
	"  WHERE (db_id=" << db_id << ")";
    MySqlResult *res = select(query);
    Assert(res->countRows()==1);
    db_id = res->getIntField(0);
    db_name = res->getField(1);
    srv_name = res->getField(2);
    delete res;
}


/**
 * Get the datapath for the folder named 'folderName'.
 * The datapath is identified the pair of integers folderId and databaseId.
 */

void MySqlDBMgr::getFolderId(string folderName, int& folderId, int& databaseId)

{
    MYSQLSTREAM query;
    query << "SELECT fld_id, db_id FROM " FOLDERS_TBL " WHERE(fpath='" << folderName << "')";
    MySqlResult *res = select(query);
    Assert ( res->countRows() != 0 );
    folderId = res->getIntField(0);
    databaseId = res->getIntField(1);
    delete res;
}

void MySqlDBMgr::getDBId(int fldId, int& dbId)

{
		MYSQLSTREAM query;
		query << "SELECT db_id FROM folders_tbl WHERE fld_id=" << fldId;
		MySqlResult *res = select(query);
		Assert (res->countRows() == 1);
		dbId = res->getIntField(0);
		delete res;
}

void MySqlDBMgr::getFolderType(string folderName, int& ftype)

{
    MYSQLSTREAM query;
    query << "SELECT ddtype FROM " FOLDERS_TBL " WHERE(fpath='" << folderName << "')";
    MySqlResult *res = select(query);
    if ( res->countRows() == 0 )
      std::cerr << "Non-existing (or empty?) folder " << folderName << std::endl;
    Assert ( res->countRows() != 0 );
    ftype = res->getIntField(0);
    delete res;
}

/**
 * Tries to find a 'MySqlObjectMgr' object that matches the database
 * identified by 'dbId'. It will generate an exception if it fails.
 */

MySqlObjectMgr* MySqlDBMgr::getObjectMgr(int dbId)

{
    Assert(dbId != 0);

    MySqlObjectMgr *oMgr = relObjectMgr;
    while ( oMgr != 0 ) {
	if ( oMgr->databaseId == dbId )
	    return oMgr;
	oMgr = static_cast<MySqlObjectMgr*>(oMgr->next);
    }
//nbarros:ate aqui tem 1 ligacao apenas!!!!
    string srvName, dbName;
    getDBPath(dbId, dbName, srvName);

//nbarros: this will create a new connection. How far is this necessary?
// If we want to store an object why don't we use the handle already created?
//    oMgr = new MySqlObjectMgr(this);

// this will use the existing connection....

    oMgr = new MySqlObjectMgr(this, mysqlHandle);
    oMgr->changeDatabase(srvName, userName, password, dbName, dbId);
    oMgr->next = relObjectMgr;
    relObjectMgr = oMgr;
    return relObjectMgr;
}

/**
 * Tries to find a 'MySqlOnlineMgr' object that matches the database
 * identified by 'dbId'. It will generate an exception if it fails.
 */

MySqlOnlineMgr* MySqlDBMgr::getOnlineMgr(int dbId)

{
    Assert(dbId != 0);
    MySqlOnlineMgr *oMgr = relOnlineMgr;
    while ( oMgr != 0 ) {
	if ( oMgr->databaseId == dbId )
	    return oMgr;
	oMgr = static_cast<MySqlOnlineMgr*>(oMgr->next);
    }

//nbarros:ate aqui tem 1 ligacao apenas!!!!
    string srvName, dbName;
    getDBPath(dbId, dbName, srvName);

//nbarros: this will create a new connection. How far is this necessary?
// If we want to store an object why don't we use the handle already created?
//    oMgr = new MySqlOnlineMgr(this);

// this will use the existing connection....
    oMgr = new MySqlOnlineMgr(this, mysqlHandle);
    oMgr->changeDatabase(srvName, userName, password, dbName, dbId);
    oMgr->next = relOnlineMgr;
    relOnlineMgr = oMgr;
    return relOnlineMgr;
}

/**
 * Tries to find a 'MySqlDataMgr' object that matches the database
 * identified by 'dbId'. It will generate an exception if it fails.
 */

MySqlDataMgr* MySqlDBMgr::getDataMgr(int dbId)

{
    Assert( dbId!=0 );

    MySqlDataMgr *dMgr = relDataMgr;
    while ( dMgr != 0 ) {
	if ( dMgr->databaseId == dbId )
	    return dMgr;
	dMgr = static_cast<MySqlDataMgr*>(dMgr->next);
    }

    string srvName, dbName;
    getDBPath(dbId, dbName, srvName);

 //nbarros: this will create a new connection. How far is this necessary?
// If we want to store an object why don't we use the handle already created?
//    oMgr = new MySqlDataMgr(this);

// this will use the existing connection....
    dMgr = new MySqlDataMgr(this, mysqlHandle);
    dMgr->changeDatabase(srvName, userName, password, dbName, dbId);
    dMgr->next = relDataMgr;
    relDataMgr = dMgr;
    return relDataMgr;
}


/**
 * Returns a pointer to the 'MySqlFolderMgr' object tied to the current connection
 */

MySqlFolderMgr* MySqlDBMgr::getFolderMgr()
{
    if ( relFolderMgr == 0 )
	relFolderMgr = new MySqlFolderMgr(this,mysqlHandle);
    return relFolderMgr;
}


/**
 * Returns a pointer to the 'MySqlTagMgr' object tied to the current connection
 */

MySqlTagMgr* MySqlDBMgr::getTagMgr()
{
    if ( relTagMgr == 0 )
	relTagMgr = new MySqlTagMgr(this,mysqlHandle);
    return relTagMgr;
}

// THE END
