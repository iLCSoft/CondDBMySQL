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
/* $Id: MySqlConnection.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlConnection.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef MYSQL_CONNECTION_H
#define MYSQL_CONNECTION_H

// C++ include files
#include <string>
using namespace std;

// Decide which stream implementations to use
//
// Use 'strstream' if GCC version < 3.0.0
// and 'sstream' otherwise

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

class MySqlHandle;
class MySqlResult;
class CondDBLog;

class MySqlConnection {

public:

    MySqlConnection()
	throw(CondDBException);
    
    MySqlConnection(MySqlHandle *mysql)
	throw(CondDBException);
    
    ~MySqlConnection();
    
    void connect(const string& server,
		 const string& username,
		 const string& password,
		 unsigned int port = 0)//,
//		 const string& dbname)
	throw(CondDBException);

    void execute(MYSQLSTREAM& query)
	throw(CondDBException);

    MySqlResult *select(MYSQLSTREAM& query)
	throw(CondDBException);

    bool changeDB(const string& dbname);

    // data members
    MySqlHandle *mysqlHandle;    
    MySqlConnection *next;
    int databaseId;

    CondDBLog *cdbLog;
    // constants
    // static const int MAX_INIT_STR_LEN = 255;

private:

// void parseInitStr(char *initstr, char **host, char **user, char **passwd);
};


#endif /* MYSQL_CONNECTION_H */











