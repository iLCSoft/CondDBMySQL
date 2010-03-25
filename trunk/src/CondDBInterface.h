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
/* $Id: CondDBInterface.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBInterface.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#ifndef CONDDBINTERFACE_HH
#define CONDDBINTERFACE_HH

#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/ICondDBDataAccess.h"

#include "CondFolderMgr.h"
#include "CondDBDataAccess.h"

#include "MySqlDBMgr.h"

/*
 * MySQL based implementation of ICondDBMgr
 */

class CondDBInterface : public ICondDBMgr {

public:

    // Constructor
    CondDBInterface();

    // Destructor
    virtual ~CondDBInterface();

    virtual void init(string databaseInfo = "")
	throw(CondDBException);

    virtual void createCondDB(string condDBInfo = "")
	throw(CondDBException);

    virtual bool isCondDBcreated() const
	throw(CondDBException);
  
    virtual void openDatabase()
	throw(CondDBException);

    virtual void startUpdate()
	throw(CondDBException);

    virtual void startRead()
	throw(CondDBException);

    virtual void abort()
	throw(CondDBException);

    virtual void commit()
	throw(CondDBException);

    const char* getGeneralDBname() const 
	{ return relDBMgr->getDBName().c_str(); }

    ICondDBBasicDataAccess* getCondDBBasicDataAccess()
        throw(CondDBException) {
	return getCondDBDataAccess();
    };

    ICondDBDataAccess* getCondDBDataAccess()
	throw(CondDBException);

    ICondDBBasicFolderMgr* getCondDBBasicFolderMgr()
        throw(CondDBException) {
	return getCondDBFolderMgr();
    };
    
    ICondDBFolderMgr* getCondDBFolderMgr()
	throw(CondDBException);

    ICondDBTagMgr* getCondDBTagMgr()
	throw(CondDBException);
      
private:
 
    // interface to the ralational dbms
    MySqlDBMgr*       relDBMgr;

    // private attributes
    CondFolderMgr*    condFolderMgr;
    CondDBDataAccess* condDataMgr;
    CondDBTagMgr*     condTagMgr;

    bool	      initDone;
    string            defaultUserProfile;
};


#endif








