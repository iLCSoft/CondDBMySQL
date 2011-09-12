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
/* $Id: MySqlTypes.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlTypes.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef MYSQL_TYPES_H
#define MYSQL_TYPES_H

// C include files

extern "C" {
#include <mysql.h>
}

#include "ConditionsDB/CondDBKey.h"
#include "ConditionsDB/SimpleTime.h"


// let's use the exceptions in our way...
#include <stdexcept>

#ifdef _WIN32
typedef __int64 int64;
#define atoi64(x) _atoi64(x)
#else
#define atoi64(x) atoll(x)
typedef long long int64;
#endif

#include "CondDBLog.h"
class CondDBLog;


/*
* Exception used for error handling.
*/
// WARN: ErrorCode values to be defined, will set 0 in the
//       intermediate time


class MySqlHandle {
public:
    MySqlHandle();
   
    ~MySqlHandle();

    void link(MySqlHandle*& mysqlH) 
	throw(CondDBException);
    
    void unlink(MySqlHandle*& mysqlH) 
	throw(CondDBException);

    bool isShared();

    MYSQL *handle();
    
    bool isOpened();

    void open(const char *host, const char *user, 
	      const char *passwd,unsigned int port)//, const char *dbname)
	throw(CondDBException);

    int changeDB(const char *dbname);

    void exec(const char *buf, unsigned int size)
	throw(CondDBException);

    MYSQL_RES *getres()
	throw(CondDBException);

public:

    CondDBLog *theLog;
    
private:
    MYSQL mysqlApp;
    int usage_count;
    bool is_opened;

    
// disallow copy constructor and = operator
    MySqlHandle (const MySqlHandle &);
    MySqlHandle & operator = (const MySqlHandle &);
};


class MySqlResult {
  
public:
    MySqlResult(MYSQL_RES *_res)
	throw(CondDBException);

    MySqlResult(MYSQL_RES *_res, MySqlHandle *& theHandle)
	throw(CondDBException);
   
    ~MySqlResult();
    
    void seek(int64 pos);
    
    int64 countRows();
    
    int64 countFields();
    
    bool nextRow();
    
    char *getField(unsigned int field)
	throw(CondDBException);
    
    int getIntField(unsigned int field)
	throw(CondDBException);

    CondDBKey getCondDBKeyField(unsigned int field)
	throw(CondDBException);

    SimpleTime getTimeStampField(unsigned int field)
	throw(CondDBException);

    char **getFields();
    
    unsigned long getLength(unsigned int field) 
	throw(CondDBException);

    string getFieldName(unsigned int field)
        throw(CondDBException);

    string getFieldType(unsigned int field)
	throw(CondDBException);

    
private:
    int64 rpos;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MySqlHandle *handle;
    CondDBLog *theLog;
};


////////////////////////////////////
//  MySqlHandle member functions  //
////////////////////////////////////

inline MySqlHandle::MySqlHandle()
{

    mysql_init(&mysqlApp);
    usage_count = 1;
    is_opened = false;
    theLog= 0; // avoid uninitialized pointer here
}


inline MySqlHandle::~MySqlHandle()
{
    if ( is_opened ) 
    {
// CondDBLog
      if (theLog->isUsable())
	theLog->StoreLog("MySqlHandle:: Connection Successfully closed!");
	
	mysql_close(&mysqlApp);
    }
}

inline void MySqlHandle::link(MySqlHandle*& mysqlH)
    throw(CondDBException)
{
    Assert(mysqlH == 0);
    mysqlH = this;
    usage_count++;
}
    
inline void MySqlHandle::unlink(MySqlHandle*& mysqlH)
    throw(CondDBException)
{
    Assert(mysqlH == this);
    usage_count--;
    mysqlH = 0;
    if ( !usage_count )	
	delete this;
}

inline bool MySqlHandle::isShared()
{
    if (usage_count > 1)
	return true;
    else
	return false;
}

inline void MySqlHandle::open(const char *host, const char *user,
			      const char *passwd, unsigned int port = 0)
    throw(CondDBException)
{
	
    

    if ( is_opened )
    {
//NBARROS
	if (theLog->isUsable())
	    theLog->StoreLog("MySqlHandle::open: Trying to reopen connection.\nCOnnection already opened.");

	return;
    }
    if ( mysql_real_connect(&mysqlApp,host,user,passwd,/*dbname*/ 0,port,NULL,0) )
    {
	is_opened = true;
    }    
    else
    {
//NBARROS
	if (theLog->isUsable())
	    theLog->StoreMySqlLog(&mysqlApp, __FILE__, __LINE__);

	MyTHROWEX(&mysqlApp);

    }
}
#include<iostream>

inline void MySqlHandle::exec(const char *buf, unsigned int size)
    throw(CondDBException)
{

    if (mysql_real_query(&mysqlApp, buf, size))
    {
//NBARROS
	if (theLog->isUsable())
	    theLog->StoreMySqlLog(&mysqlApp, __FILE__, __LINE__);

	MyTHROWEX(&mysqlApp);
    }
}

inline MYSQL_RES *MySqlHandle::getres()
    throw(CondDBException)
{
    MYSQL_RES *res = mysql_store_result(&mysqlApp);
    if ( !res )
    {
	//NBARROS
	if (theLog->isUsable())
	    theLog->StoreMySqlLog(&mysqlApp,__FILE__, __LINE__);

	MyTHROWEX(&mysqlApp);
    }
    return res;
}
    
inline int MySqlHandle::changeDB(const char *dbname)
{
    return mysql_select_db(&mysqlApp, dbname);
}

    

inline MYSQL *MySqlHandle::handle()
{
    return &mysqlApp;
}

inline bool MySqlHandle::isOpened()
{
    return is_opened;
}


//
// MySqlResult member functions
//
inline MySqlResult::MySqlResult(MYSQL_RES *_res)
    throw(CondDBException)
{
// This test is not necessary because this contructor is only
// used in a place where the argument passed has already been
// checked to be != 0. Remove it along with the throw clause 
// in the declaration 
//    Assert(_res!=0);
    res = _res;
    row = 0;
    rpos = 0;
}

inline MySqlResult::MySqlResult(MYSQL_RES *_res, MySqlHandle *& theHandle)
	throw(CondDBException)
{
    res = _res;
    row = 0;
    rpos = 0;
    handle = theHandle;
//    theLog = theHandle->mysqlLog;
}
    
inline MySqlResult::~MySqlResult()
{
    mysql_free_result(res);
}
    
inline void MySqlResult::seek(int64 pos)
{
    if (pos < countRows())
    {	
	rpos = pos;
	mysql_data_seek(res, pos);
	row = 0;
    }
}
    
inline int64 MySqlResult::countRows()
{
    return mysql_num_rows(res);
}
    
inline int64 MySqlResult::countFields()
{
    if ( !row )
	row = mysql_fetch_row(res);
    
    return mysql_num_fields(res);
}
    
inline bool MySqlResult::nextRow()
{
    if ( rpos < countRows()-1 )
    {
	rpos++;
	row = 0;
	return true;
    }
    return false;
}

inline char * MySqlResult::getField(unsigned int field)
    throw(CondDBException)
{
    if ( !row )
	row = mysql_fetch_row(res);
    if ( field < mysql_num_fields(res) )
	return row[field];
    else
	THROWEX("MySqlResult::getField: invalid field.", 0);
}
    
inline int MySqlResult::getIntField(unsigned int field)
    throw(CondDBException)
{
    char *ptr;
    if ( (ptr = getField(field))==0 )
	THROWEX("MySqlResult::getIntField: can't convert NULL field.", 0);
    // Must provide some verification if the field is realy integer
    // And throw an expection otherwise.
    return atoi(ptr);
}

inline CondDBKey MySqlResult::getCondDBKeyField(unsigned int field)
    throw(CondDBException)
{
    char *ptr;
    if ( (ptr = getField(field))==0 )
	THROWEX("MySqlResult::getCondDBKeyField: can't convert NULL field.", 0);
    // Must provide some verification if the field is realy big integer
    // And throw an expection otherwise.
    return atoi64(ptr);
}

inline SimpleTime MySqlResult::getTimeStampField(unsigned int field)
    throw(CondDBException)
{
    char *ptr;
    if ( (ptr = getField(field))==0 )
    {
	if (theLog->isUsable())
	    theLog->StoreLog("MySqlResult::getTimeStampField: can't convert NULL field.",
			     __FILE__,
			     __LINE__);
	
	THROWEX("MySqlResult::getTimeStampField: can't convert NULL field.", 0);
    }
    std::string stime(ptr);
    stime += "000000000"; // for nanoseconds. Always zero.
    //    DebugMesg(CondDB, user, "MySqlResult::getTimeStampField -> "<< stime);
    // This should be rewrote to support nanosecond resolution?
    // Remark: this method is only used to retrieve timeStamp fields used for 
    // insertion time, not for evolution axis.
    SimpleTime timeStamp(stime);
    return timeStamp;
}

inline char **MySqlResult::getFields()
{
    if ( !row )
	row = mysql_fetch_row(res);
    return row;
}
    
inline unsigned long MySqlResult::getLength(unsigned int field)
    throw(CondDBException)
{
    if ( !row )
	row = mysql_fetch_row(res);
    
    unsigned long *lengths = mysql_fetch_lengths(res);
    if ( field < mysql_num_fields(res) )
	return lengths[field];
    else 
	THROWEX("MySqlResult::getLength: invalid field.", 0);
}

inline string MySqlResult::getFieldName(unsigned int field)
    throw(CondDBException)
{
    MYSQL_FIELD *mfield;
    if (field < mysql_num_fields(res))
    {
        mfield = mysql_fetch_field_direct(res, field);
        string fieldName=mfield->name;
        return fieldName;
    }
    else
        THROWEX("MySqlResult::getFieldName: invalid field.", 0);
}

inline string MySqlResult::getFieldType(unsigned int field)
	throw(CondDBException)
{

    MYSQL_FIELD *mfield;
    if (field < mysql_num_fields(res))
    {
	string typeName="NONE";
        mfield = mysql_fetch_field_direct(res, field);
        int fieldType=mfield->type;
	if (fieldType==FIELD_TYPE_FLOAT)
	    typeName="FLOAT";
	if (fieldType==FIELD_TYPE_LONG)
	    typeName="INT";
	if (fieldType==FIELD_TYPE_BLOB)
	    typeName="STRING";
	return typeName;
    }
    else
        THROWEX("MySqlResult::getFieldName: invalid field.", 0);

}



#endif /* MYSQL_TYPES_H */







