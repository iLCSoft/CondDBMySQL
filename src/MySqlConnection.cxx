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
/* $Id: MySqlConnection.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlConnection.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#include <string>
#include "Debug.h"
#include "MySqlConnection.h"
#include "MySqlTypes.h"
//////
#ifndef _WIN32
  #include "sys/time.h"
#else
  #include <time.h>
#endif

MySqlConnection::MySqlConnection()
    throw(CondDBException)
{
    mysqlHandle = new MySqlHandle();
    Assert(mysqlHandle != 0);
    next = 0;
    databaseId = 0;
    cdbLog = 0;

}

MySqlConnection::MySqlConnection(MySqlHandle *mysqlH)
    throw(CondDBException)
{
    mysqlHandle = 0;
    Assert(mysqlH != 0);
    
    mysqlH->link(mysqlHandle);
    cdbLog = mysqlH->theLog;
    next = 0;
    databaseId = 0;

}

MySqlConnection::~MySqlConnection()
{
    // Unlink itself. This will eventualy delete the handle
    if (mysqlHandle->isShared())
	mysqlHandle->unlink(mysqlHandle);
    else
    {
	mysqlHandle->unlink(mysqlHandle);
/***** CondDBLog ***/    
	if (cdbLog) delete cdbLog;
    }
}


/**
 * Connect to the database server. This method is called by the derived
 * classes objects, for example as 'MySqlObjectMgr::connect(...)'
 */
void MySqlConnection::connect(const string& server,
			      const string& username,
			      const string& password,
			      unsigned int port)
  throw(CondDBException)
{
  const char *host = "localhost"; // Default server is in localhost
  const char *user = NULL;        // Default user is no user
  const char *passwd = NULL;      // Default password is nopassword
  unsigned int p_port = 0;

  if  (server!="") host = server.c_str(); 
  if  (username!="") user = username.c_str(); 
  if  (password!="") passwd = password.c_str(); 
  if (port){
    p_port = port;
  }
  
  /************* CondDBLog *************/
  // Let's create the logging object!!!
  
  const char* isDebug = getenv("CONDDB_LOG");
  
  if (isDebug) {
    
    if (atoi(isDebug) == 1)
      //	     if(!cdbLog)
      {
	cdbLog = new CondDBLog(host, user, passwd,p_port);
	mysqlHandle->theLog = cdbLog;
      }
    
    else {
      cdbLog = new CondDBLog();
      mysqlHandle->theLog = cdbLog;
    }
  } else {
    cdbLog = new CondDBLog();
    mysqlHandle->theLog = cdbLog;
  }
  
  DebugMesg(CondDB, user, "MySqlConnection::connect():" << "\n\t" << host <<
	    "\n\t" << user << "\n\t" << passwd << endl);
  
//nbarros
//    mysqlHandle->open(host, user, passwd, dbname);
    mysqlHandle->open(host, user, passwd,p_port);//, 0);

}


/**
 * Changes the current database. The database must already exist.
 */
bool MySqlConnection::changeDB(const string& dbname)
{
  if (cdbLog->isUsable())
    cdbLog->setDbName(dbname.c_str());

  if( mysqlHandle->changeDB(dbname.c_str()) ) {
	DebugMesg(CondDB, user, 
		  "MySqlConnection::changeDB(\"" << dbname << "\") failed");
	/***** CondDBLog *******/
	if (cdbLog->isUsable())
	    cdbLog->StoreMySqlLog(mysqlHandle->handle(), __FILE__, __LINE__);
	return false;
    } 
    if (cdbLog->isUsable())
	cdbLog->StoreLog("MySqlConnection:: Connection successfully opened.");

    DebugMesg(CondDB, user,
	      "MySqlConnection::changeDB(\"" << dbname << "\") succeded ");

    return true;
}


/**
 * SQL Query. This function will flush the stream contents so that it can be
 * reused by the caller
 */
void MySqlConnection::execute(MYSQLSTREAM& query)
    throw(CondDBException)
{

#ifdef USE_OLD_STREAMS
    // Is this really necessary ? Apparently calling strstream::str() 
    // already freezes it.
    query.rdbuf()->freeze(true);
    // Because strstream::str() doesn't return a null terminated
    // C like string, one use mysql_real_query() instead of mysql_query()
    // Besides mysql_real_query() is more eficient because it doesn't
    // need to call strlen()
    string querystr(query.str(),query.pcount());
    DebugMesg(CondDB, devl, "Issuing query:\n\"" << querystr.c_str() << "\"");
    
    //Time for querying
    #ifdef TIMETESTS
       struct timeval t_start, t_end;
       long t;
       gettimeofday(&t_start, 0); 
    #endif

    mysqlHandle->exec(query.str(), query.pcount());

    #ifdef TIMETESTS
       gettimeofday(&t_end, 0);
       t = ((t_end.tv_sec*1000000 + t_end.tv_usec) - (t_start.tv_sec*1000000 + t_start.tv_usec));
       DebugMesg(CondDB, devl, "  Query succeded! Time to execute:\t " << t);
    #else
       DebugMesg(CondDB, devl, "  Query succeded!");
    #endif

    // now unfreeze it so that the storage can be managed
    query.rdbuf()->freeze(false);
    query.seekp(0);
    query.width(0);
#else
    //ostringstream doesen't nedd to be freezed
    //query.str() returns a c++ string 
    //using query.str().c_str() to convert into a char * 
    DebugMesg(CondDB, devl, "Issuing query:\n\"" << query.str().c_str() << "\"");
    //query.str().size() returns the size of the string

    //Time for querying
    #ifdef TIMETESTS
       struct timeval t_start, t_end;
       long t;
       gettimeofday(&t_start, 0);
    #endif
    mysqlHandle->exec(query.str().c_str(), query.str().size());
    #ifdef TIMETESTS
       gettimeofday(&t_end, 0);
       t = ((t_end.tv_sec*1000000 + t_end.tv_usec) - (t_start.tv_sec*1000000 + t_start.tv_usec));
       DebugMesg(CondDB, devl, "  Query succeded! Time to execute the query:\t" << t);
    #else
       DebugMesg(CondDB, devl, "  Query succeded!");
    #endif
   
    //clears the buffer
    query.str("");
#endif
}

/**
 * SQL Select Query. This method in turn calls the 'MySqlConnection::execute'
 * method but expects the query to be a select query and thus prepares the code
 * to return the corresponding 'MySqlResult' object.
 */
MySqlResult *MySqlConnection::select(MYSQLSTREAM& query)
    throw(CondDBException)
{
    execute(query);
    MySqlResult *res = new MySqlResult(mysqlHandle->getres(),mysqlHandle);
    Assert(res!=0);
    return res;
}

// THE END
