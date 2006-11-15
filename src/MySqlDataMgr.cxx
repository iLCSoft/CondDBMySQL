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
/* $Id: MySqlDataMgr.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlDataMgr.cxx"
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
#include "MySqlDataMgr.h"
#include "MySqlTypes.h"
#include "boot-sql.h"


MySqlDataMgr::MySqlDataMgr(MySqlDBMgr *dbMgr,
			  MySqlHandle *mysqlH)
    : MySqlConnection(mysqlH)
{
    databaseMgr = dbMgr;
}

MySqlDataMgr::MySqlDataMgr(MySqlDBMgr *dbMgr)
    : MySqlConnection()
{
    databaseMgr = dbMgr;
}

MySqlDataMgr::~MySqlDataMgr()
{
    if (next != 0) delete next;
}


// Object management functions


/**
 * Fetch the data associated with the triplet 'folderId','partId','oKey'
 */

MySqlResult *MySqlDataMgr::fetch(int folderId, int partId, int oKey)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    query << "SELECT * FROM " DATA_TBL_NN << folderId 
	<< "_" << partId << " WHERE (dat_id=" << oKey << ")";
    
    return select(query);
}

/**
 * Store the data blob in the data table identified by 'folderId' and 'partId'
 * and associate it with the 'description'.
 */

int MySqlDataMgr::store(int folderId, int partId, 
			string& description, string& blob)
    throw(CondDBException)
{
    int id;
    MYSQLSTREAM query;
    string eblob;

    escapeBinaryString(eblob, blob);

    query << "INSERT INTO " DATA_TBL_NN << folderId << "_" << partId
	  << " (description, oblock) VALUES(\'" << description 
	  << "\', \'" << eblob << "\')";
    
    execute(query);
    query << "SELECT LAST_INSERT_ID()";
    MySqlResult *res = select(query);
    Assert(res->countRows()==1);
	
    id = res->getIntField(0);
    delete res;
    return id;
}


/**
 * 
 */

void MySqlDataMgr::changeDatabase(const string& srvname,
				  const string& username,
				  const string& password,
				  const string& dbname,
				  int dbId)
    throw(CondDBException)
{
    connect(srvname, username, password);
    if(changeDB(dbname)==false)
	THROWEX("MySqlDataMgr::changeDatabase(...) failed",0);
    databaseId = dbId;
}


/**
 * Process BLOBs so it can be passed to a SQL statement. The source BLOB is 
 * passed in the 'source' argument and the escaped string is returned in the
 * 'target' argument
 */

void MySqlDataMgr::escapeBinaryString(string& target, const string& source)
    throw(CondDBException)
{
    unsigned int len = source.size();
    if( len==0 ) { target=""; return; }

    char *_target = new char[len*2];
    Assert( _target != 0 );
    
    len = mysql_real_escape_string(mysqlHandle->handle(), _target,
				   source.c_str(), len);
    
    // the later is faster because it doesn't need to call strlen()
    //  target = _target;
    target.assign(_target, len);
    delete[] _target;
}


// END







