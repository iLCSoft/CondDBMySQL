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
/* $Id: CondFolderMgr.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondFolderMgr.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef CONDFOLDERMGR_HH
#define CONDFOLDERMGR_HH

#include "ConditionsDB/ICondDBFolderMgr.h"
#include "MySqlFolderMgr.h"
//#include <CondDBLog.h>

//class CondDBTable;
//class CondDBInterface;
//class CondDBDataAccess;
class MySqlTagMgr;
class MySqlResult;
//class CondDBLog;

const char separator = '/';

/**
 * MySQL based implementation of ICondDBFolderMgr
 */

class CondFolderMgr : public ICondDBFolderMgr {

public:

    // Constructors
    CondFolderMgr(MySqlDBMgr *dbMgr);

    // Destructor
    virtual ~CondFolderMgr();
    
    virtual void init()
	throw(CondDBException);
    
    virtual void createRootFolderSet( string attributes  /* = "" */,
				      string description /* = "" */)
	throw(CondDBException);
    
    virtual void createCondDBFolderSet( const string& fullPathName,
					string  attributes,
					string  description,
					bool    parents,
					int dd_type) 
	throw(CondDBException);
    
    virtual void createCondDBFolderSet( const string& fullPathName,
                                        string  attributes  = "",
                                        string  description = "",
					bool    parents     = false)
        throw(CondDBException) {
	createCondDBFolderSet
	    ( fullPathName, attributes, description, parents, 0 );
    };

    virtual void createCondDBFolder( const string& fullPathName,
				     string  attributes  = "",
				     string  description = "",
				     bool    parents     = false) 
	throw(CondDBException);

    virtual void createCondDBFolder( const string& fullPathName,
				     ICondDBTable* table,
				     string  attributes = "" ,
				     string  description = "" ,
				     bool    parents = false,
				     ICondDBFolder::folder_types ftype = ICondDBFolder::STRUCT)
	throw(CondDBException);
    
    virtual bool exist( const string& fullName ) const
	throw(CondDBException);
    
    
    virtual void getCondDBFolderSet( const string&      fullPathName,
				     ICondDBFolderSet*& folderSet) const
	throw(CondDBException);
    
    virtual void getCondDBFolder( const string&   fullPathName,
				  ICondDBFolder*& folder) const
	throw(CondDBException);
    
    
    void describe(const string& fullName, string options="") const
	throw(CondDBException);
    
    void getAllCondDBFolder( vector<string>& allCondFolder ) const
	throw(CondDBException);
    
    void getAllCondDBFolderSet( vector<string>& allCondFolderSet ) const
	throw(CondDBException);
    
    void getAllCondDBFolderBeneath( const string& fullPathName,
				    vector<string>& folderList ) const
	throw(CondDBException);

    int getDB_id(std::string dbname, std::string srvname)
	throw(CondDBException);

    virtual void deleteFolder( const string& fullPathName )
	throw(CondDBException);

    int getFolderType(const string& fullPathName)
	throw(CondDBException);

    // Not in the interface
    MySqlResult *getContainedFolderRef(int parent_id, MySqlFolderMgr::folder_type type) const
	throw(CondDBException);

private:

    // Interface to the ralational dbms
    MySqlDBMgr*      relDBMgr;
    MySqlFolderMgr*  relFolderMgr;
    MySqlTagMgr*     relTagMgr;
    
    // methods
    MySqlResult *findFolderSet( const string& folderSetFullName ) const
	throw(CondDBException);
    
    MySqlResult *findFolder( const string& folderFullName ) const
	throw(CondDBException);
    
    void splitPath( const string& fullPathName,
		    string&       parent,
		    string&       name) const
	throw(CondDBException);
    
    void normalizePath( const string& fullPathName,
			string&       normName ) const
	throw(CondDBException);
};

#endif









