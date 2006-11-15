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
/* $Id: CondDBLog.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// File: "CondDBLog.h"
//

#ifndef CONDDBLOG_H
#define CONDDBLOG_H

#define CONDLOGDB "conddblog"

enum type{USER,DB};

// FOR COMPATIBILITY
#ifndef WIN32
//# include <ansidecl.h>
#ifdef __GNUC__
# if __GNUC__ < 3
#  include <strstream>
#  define MYSQLSTREAM strstream
#  define USE_OLD_STREAMS
# else
#  include <sstream>
#  define MYSQLSTREAM std::ostringstream 
# endif
#else
#  include <sstream>
#  define MYSQLSTREAM std::ostringstream 
#endif
#endif

#ifdef WIN32
#  include <sstream>
#  define MYSQLSTREAM std::ostringstream
#endif

// C include files

extern "C" {
#include <mysql.h>
#include <stdio.h>
}

// NOTES: At creation of the object variable user takes a value. This means that at 
//        assigent of the value of database variable, we can verify the existence of
//        the user and database, and then insert them in case of inexistence.


class CondDBLog {
/*
 This class will be used to create a connection to a database server and 
 store the exceptions in a separate database.
 There will be a table with users, a table with databases and a table with the log...
*/
 public:
    
  CondDBLog();

    CondDBLog (const char* server, const char* user, const char* password, unsigned int port);

    ~CondDBLog();

    void StoreLog(const char* msg);
    void StoreLog(const char* msg, const char* file, long line);

    void StoreMySqlLog(MYSQL *mysqlApp, const char* file, long line);

    void setUser(const char* user){username = user;}
    void setDbName(const char* db){database = db; verifyValues();}
    
    bool isUsable(){return m_isOperational;}

// private members
 private:
    const char *numberToString(long i)
	{
	    static char buf[32];
	    sprintf(buf,"%ld",i);
	    return buf;
	}

    void verifyValues();

    void exist(std::string name, type tp);

    long getID(const char* name, type tp);
    void add(const char* name, type tp);

 private:

    bool m_isOperational;
    bool m_isUser;
    bool m_isDB;

    MYSQL mysqlConn;
    MYSQL_RES *sqlResult;

    std::string logdb;
    std::string database;
    std::string username;

    // For future implementation;
    
    CondDBLog (const CondDBLog&);
    CondDBLog & operator = (const CondDBLog &);

};


/////////////////////////////////////
//  CondDBLog Member functions     //
/////////////////////////////////////

inline CondDBLog::CondDBLog() {
  m_isOperational = false;
  m_isUser = false;
  m_isDB = false;
}

inline CondDBLog::CondDBLog(const char* server,const char* user,const char* password, unsigned int port = 0)
{
  //cout<<"CondDBLog::CondDBLog ('"<<server<<"','"<<user<<"','"<<password<<"')"<<endl;
  logdb = CONDLOGDB;
  if ( user ) username = user;
  else username = "";
  m_isOperational = true;
  m_isUser = false;
  m_isDB = false;
  
  if (mysql_init(&mysqlConn) == NULL)
    {
      m_isOperational = false;
    }
    std::string query;
    
    // tries to connect to the dbase. If it doesn't exist create the whole structure!
    if ( (mysql_real_connect(&mysqlConn,server,user,password,logdb.c_str(),port,NULL,0)) == NULL )
      {
	if ((mysql_real_connect(&mysqlConn,server,user,password,0,0,NULL,0)) == NULL)
	{
	  m_isOperational = false;
	}
	
	query = std::string("CREATE DATABASE ") + logdb;
	
	if(mysql_real_query(&mysqlConn, query.c_str() , query.size()))
	{
	    m_isOperational = false;  
	    return;
	}
	
	if (mysql_select_db(&mysqlConn, logdb.c_str()))
	{
	    m_isOperational = false;
	    
	    return;
	}   
	query = std::string("CREATE TABLE users ( id int(11) NOT NULL auto_increment, name varchar(255) NOT NULL,")
	    + std::string(" PRIMARY KEY (id)) TYPE=MyISAM");

	if (mysql_real_query(&mysqlConn, query.c_str() , query.size()))
	{
	    m_isOperational = false;
	}
	
	query = std::string("CREATE TABLE dbases (id int(11) NOT NULL auto_increment, name varchar(255) NOT NULL,")
	    + std::string(" PRIMARY KEY (id)) TYPE=MyISAM");
	if (mysql_real_query(&mysqlConn, query.c_str() , query.size()))
	{
	    m_isOperational = false;
	}
	
	query = std::string("CREATE TABLE dblog (user int(11) NOT NULL, db int(11) NOT NULL,")
	    + " exception varchar(255) NOT NULL, time timestamp default NULL ) TYPE=MyISAM";
	if (mysql_real_query(&mysqlConn, query.c_str() , query.size()))
	{
	    m_isOperational = false;
	}
	
	
    }
    
// Now it's time to add this user 
    
    verifyValues();
    
}

inline CondDBLog::~CondDBLog()
{
  if (isUsable())
    mysql_close(&mysqlConn);
}

inline void CondDBLog::verifyValues()
{
// At this moment the variables user and db have already values, what means that we can 
// verify the existence of these values on the database and eventually add them.

    if (!m_isOperational)
    {
	return;
    }

    exist(username, USER);
    exist(database, DB);

    if ((!m_isUser) && (username.size()))
    {
	add(username.c_str(), USER);
	m_isUser = true;
    }

   if ((!m_isDB) && (database.size()))
    {
	add(database.c_str(), DB);
	m_isDB = true;
    }

}

// Methods relative to the user

inline long CondDBLog::getID(const char* name, type tp)
{
    std::string query;

    if (tp == USER)
	query = std::string("SELECT id from users WHERE name='") + name + "'";
    else
	query = std::string("SELECT id from dbases WHERE name='") + name + "'";
	
    mysql_real_query(&mysqlConn, query.c_str(), query.size()); 

//    MYSQL_RES *sqlResult = mysql_store_result(&mysqlConn);
    sqlResult = mysql_store_result(&mysqlConn);
    MYSQL_ROW sqlRow = mysql_fetch_row(sqlResult);

     if (!mysql_fetch_lengths(sqlResult))
     {
	 mysql_free_result(sqlResult);
	 return 0;
     }
    
    long result = atol(sqlRow[0]);
    mysql_free_result(sqlResult);
    return result;

}

inline void CondDBLog::exist(std::string name, type tp)
{
    if (getID(name.c_str(), tp))
	if (tp == USER)
	    m_isUser = true;
        else
	    m_isDB = true;
    else
 	if (tp == USER)
	    m_isUser = false;
        else
	    m_isDB = false;
    
}

inline void CondDBLog::add(const char* name, type tp)
{
// if the user already exists is no need to add it again!
    if (tp == USER)
	{
	     if (!m_isUser)
	     {
		 std::string query(std::string("INSERT INTO users (name) VALUES ('") + name + "')");
		 
		 if (mysql_real_query(&mysqlConn, query.c_str(), query.size()))
		     m_isOperational = false;
	     }
	}
    else
	if (!m_isDB)
	{
	    std::string query(std::string("INSERT INTO dbases (name) VALUES ('") + name + "')");
	    
	    // if the execution fails is better to do not continue!!!
	    if (mysql_real_query(&mysqlConn, query.c_str(), query.size()))
		m_isOperational = false;
	}
}

// Methods relative to the database
/*
inline long CondDBLog::getDbId(const char* db)
{

    std::string query(std::string("SELECT id from dbases WHERE name='") + db + "'");
    mysql_real_query(&mysqlConn, query.c_str(), query.size());  
    
    MYSQL_RES* sqlResult =  mysql_store_result(&mysqlConn);
    
    MYSQL_ROW sqlRow = mysql_fetch_row(sqlResult);
    
    if (!mysql_fetch_lengths(sqlResult))
    {
	mysql_free_result(sqlResult);
	return 0;
    }
    
    mysql_free_result(sqlResult);
    return atol(sqlRow[0]);
    
}

inline void CondDBLog::add(const char* name, type )
{
// if the database exists on the table is no need to add it
    if (!m_isDB)
    {
	std::string query(std::string("INSERT INTO dbases (name) VALUES ('") + db + "')");
	
	// if the execution fails is better to do not continue!!!
	if (mysql_real_query(&mysqlConn, query.c_str(), query.size()))
	    m_isOperational = false;
    }
    
}

inline void CondDBLog::existDb(std::string db)
{
   
    if (getDbId(db.c_str()))
	m_isDB = true;
    else
	m_isDB = false;
}
*/

// Storage methods

inline void CondDBLog::StoreLog(const char* msg)
{

// If there's some problem with the user or the database exits without doing nothing
    if (!(m_isUser && m_isDB))
    {
	m_isOperational = false;
	return;
    }

    MYSQLSTREAM tmp;
    
    tmp << "INSERT INTO dblog(user, db, exception) VALUES (" << getID(username.c_str(), USER) << ","
	<< getID(database.c_str(), DB) << ",'" << msg <<"')";
    
#ifdef USE_OLD_STREAMS
    tmp.rdbuf()->freeze(true); 
    mysql_real_query(&mysqlConn, tmp.str(), tmp.pcount()); 
    tmp.rdbuf()->freeze(false); 
    tmp.seekp(0); 
    tmp.width(0); 
#else
    mysql_real_query(&mysqlConn,tmp.str().c_str(), tmp.str().size()); 
#endif
 
}

inline void CondDBLog::StoreLog(const char* msg, const char* file, long line)
{
// If there's some problem with the user or the database exits without doing nothing
    
    if (!m_isOperational)
	return;
    
    if (!(m_isUser && m_isDB))
    {
	m_isOperational = false;
	return;
    }

    std::string tmp1(std::string(msg) + "\n*** in file: " + 
			        file + " line: " + 
			       numberToString( line )); 
    
    
    MYSQLSTREAM tmp;
    tmp << "INSERT INTO dblog(user, db, exception) VALUES (" << getID(username.c_str(), USER) <<  ","
	<< getID(database.c_str(), DB) << ",'" << tmp1.c_str() <<"')";
    
#ifdef USE_OLD_STREAMS
    tmp.rdbuf()->freeze(true); 
    mysql_real_query(&mysqlConn, tmp.str(), tmp.pcount()); 
    tmp.rdbuf()->freeze(false); 
    tmp.seekp(0); 
    tmp.width(0); 
#else
    mysql_real_query(&mysqlConn,tmp.str().c_str(), tmp.str().size()); 
#endif
 
}

inline void CondDBLog::StoreMySqlLog(MYSQL *mysqlApp, const char* file, long line)
{

  // if the handler does not exist we won't store anything
  if (!m_isOperational) {
    return;
  }

// If there's some problem with the user or the database exits without doing nothing
    if (!(m_isUser && m_isDB))
    {
	m_isOperational = false;
	return;
    }

   MYSQLSTREAM tmp1;
   tmp1 << "MySQL error [" << numberToString(mysql_errno(mysqlApp)) << "]:\n" 
	<< mysql_error(mysqlApp) << "\n*** catched in file: " << file  << "line: " 
	<< numberToString( line ); 

    MYSQLSTREAM tmp;
    tmp << "INSERT INTO dblog(user, db, exception) VALUES (" << getID(username.c_str(), USER) <<  ","
	<< getID(database.c_str(), DB) << ",'" << tmp1.str() <<"')";
     
#ifdef USE_OLD_STREAMS
    tmp.rdbuf()->freeze(true); 
    mysql_real_query(&mysqlConn, tmp.str(), tmp.pcount()); 
    tmp.rdbuf()->freeze(false); 
    tmp.seekp(0); 
    tmp.width(0); 
//    delete tmp;
#else
    mysql_real_query(&mysqlConn, tmp.str().c_str(), tmp.str().size()); 
//    delete tmp;
#endif
  
    
}

#endif /* CONDDBLOG_H */
