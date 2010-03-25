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
/* $Id: CondDBInterface.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBInterface.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#include "CondDBInterface.h"
#include "CondFolderMgr.h"
#include "CondDBObject.h"
#include "CondDBTable.h"
#include "CondDBTagMgr.h"


#include <string>

//-------------
// C Headers --
//-------------


#ifdef WIN32
extern "C" {
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}
#include <sys/types.h>
#include <io.h>
#else

#endif

#include "Debug.h"
#include "CondDBDataAccess.h"
#include "CondDBLog.h"
//#include "MySqlTypes.h"

// nbarros: Since the configure script defines ALWAYS the initialization string 
//          MYSQL_INIT_STR, the following lines are not necessary

#ifndef MYSQL_INIT_STR
// This is the default initialization string: using localhost server and
// the 'test_conddb01' database. The user is the current logged user and 
// no password is provided.
//
// This initialization string can be overridded at compile time by Makefile 
// defines. Se the topLevel Makefile for details.
//
// This can also be overridded at runtime by the environment variable 
// 'COND_DB_INIT' or by passing explicity an argument to the init method,
// CondDBInterface::init(...)
//


// nbarros: Since the configure script defines ALWAYS the initialization string 
//          MYSQL_INIT_STR, the following lines are not necessary
// dklose: necessary for windows!!!
#define MYSQL_INIT_STR "localhost:test_conddb01::"
#endif



// Constructor
CondDBInterface::CondDBInterface()
{  
    condDataMgr = 0;
    condFolderMgr = 0;
    condTagMgr = 0;
  
    // Search the environment for an initialization string
    const char *setting = getenv("COND_DB_INIT");
    if (setting)
	defaultUserProfile = setting;
    else
	defaultUserProfile = MYSQL_INIT_STR;

    initDone = false;
}


// Destructor
CondDBInterface::~CondDBInterface()
{
    if (condFolderMgr) delete condFolderMgr;
    if (condDataMgr) delete condDataMgr;
    if (condTagMgr) delete condTagMgr;
      
    if (relDBMgr) delete relDBMgr;
}


void 
CondDBInterface::init(string databaseInfo)
    throw(CondDBException)
{
    // databaseInfo = "server:dbname:user:passwd"
    // e.g. "atlobk01.cern.ch:conditionsdb:anonymous:online"
    if (initDone == true)
    {
	DebugMesg(CondDB, user, "ICondDBMgr::init already invoked");
	return;
    }

    // Instatiate the low level DB manager
    relDBMgr = new MySqlDBMgr();
    Assert(relDBMgr != 0);
    
    if ( databaseInfo == "" )
	databaseInfo = defaultUserProfile;


//nbarros: relDBMgr -> MySqlDBMgr   
    relDBMgr->init(databaseInfo);
    initDone = true;
    
    DebugMesg(CondDB, user, "MySqlDBMgr::init() succeded." );
}


void
CondDBInterface::createCondDB(string condDBInfo)
    throw(CondDBException)
{
    if ( initDone == false )
    {
	if (relDBMgr->cdbLog->isUsable())
	    relDBMgr->cdbLog->StoreLog("ICondDBMgr->init has not been called yet",__FILE__,__LINE__);

	THROWEX( "ICondDBMgr->init has not been called yet" , 0);
    }
    // Check if the named DB already exists
    if (relDBMgr->openDatabase(condDBInfo)==true)
    {
// generalDB already exists, don't need to create it; just use it
	DebugMesg(CondDB, user, "Database " << condDBInfo.c_str() << " already existing");
    }
    else
    {
	// Otherwise create it
	DebugMesg(CondDB, user, "Creating database " << condDBInfo.c_str());
	relDBMgr->createDatabase(condDBInfo);
    }
    
    DebugMesg(CondDB, user, "createCondDB completed" );
}


bool CondDBInterface::isCondDBcreated() const
    throw(CondDBException)
{
    if ( initDone == false )
    {
	if (relDBMgr->cdbLog->isUsable())
	    relDBMgr->cdbLog->StoreLog("ICondDBMgr->init has not been called yet",__FILE__,__LINE__);

	THROWEX( "ICondDBMgr->init has not been called yet" , 0);
    }
    // check to see if condDBName exists
    // simply open it and return the return value
    return relDBMgr->openDatabase();
}


void 
CondDBInterface::openDatabase()
    throw(CondDBException)
{
    if ( initDone == false )
    {
	if (relDBMgr->cdbLog->isUsable())
	    relDBMgr->cdbLog->StoreLog("ICondDBMgr->init has not been called yet",__FILE__,__LINE__);

	THROWEX( "ICondDBMgr->init has not been called yet" , 0);
    }
    
    // Check if database exists
    if ( relDBMgr->openDatabase()==false ) 
    {
	// generalDB does not exist; createCondDB has not yet been invoked
	if (relDBMgr->cdbLog->isUsable())
	    relDBMgr->cdbLog->StoreLog("createCondDB has not yet been invoked",__FILE__,__LINE__);

	THROWEX( "createCondDB has not yet been invoked" , 0);
    }
    else {
	DebugMesg(CondDB, user, "CondDBInterface::openDatabase completed" );
    }
}


// By now we dont use trasactions, so those are dummy methods
void CondDBInterface::startUpdate() throw(CondDBException) {}

void CondDBInterface::startRead() throw(CondDBException) {}

void CondDBInterface::abort() throw(CondDBException) {}

void CondDBInterface::commit() throw(CondDBException) {}

ICondDBDataAccess* CondDBInterface::getCondDBDataAccess()
    throw(CondDBException)
{
    if (!condDataMgr) {
	condDataMgr = new CondDBDataAccess(relDBMgr);
	Assert(condDataMgr != 0);
	condDataMgr->init();
	
    }
    return condDataMgr;
}


ICondDBFolderMgr* CondDBInterface::getCondDBFolderMgr()
    throw(CondDBException)
{
    if (!condFolderMgr) {
	condFolderMgr = new CondFolderMgr(relDBMgr);
	Assert(condFolderMgr != 0);
	condFolderMgr->init();
    }
    return condFolderMgr;
}

ICondDBTagMgr* CondDBInterface::getCondDBTagMgr()
    throw(CondDBException)
{
    if (!condTagMgr) {
	condTagMgr = new CondDBTagMgr(relDBMgr);
	Assert(condTagMgr != 0);
	condTagMgr->init();	
    }
    return condTagMgr;
}

// THE END
