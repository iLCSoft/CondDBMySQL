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
/* $Id: CondDBTagMgr.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBTagMgr.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#ifndef CONDDBTAGMGR_HH
#define CONDDBTAGMGR_HH

#include "ConditionsDB/ICondDBTagMgr.h"
#include "CondDBInterface.h"

//#include <HepODBMS/odbms/HepODBMS.h>

class CondFolderMgr;
class MySqlDBMgr;
class MySqlTagMgr;

class CondDBTagMgr : public ICondDBTagMgr {

 public:

    // Constructors
    CondDBTagMgr(MySqlDBMgr *dbMgr);

    // Destructor
    virtual ~CondDBTagMgr();

    void init()
	throw(CondDBException);

    virtual void createCondDBTag( const std::string& name,
				  const std::string  description = "") 
	throw(CondDBException);

    virtual void deleteCondDBTag( const std::string& name ) 
	throw(CondDBException);
  
    virtual void getCondDBTag( const std::string& tagName,
			       ICondDBTag*& tag) const
	throw(CondDBException);

    virtual bool getCondDBTag (const std::string& folder, vector <string>& tagNames)
	throw (CondDBException);

    virtual void getAllCondDBTag( std::vector<std::string>& allTag ) const
	throw(CondDBException);

    virtual void tag( const std::string& folderName,
		      const std::string& tagName,
		      std::string usingTagName = "")
	throw(CondDBException);

    virtual void untag( const std::string& folderName,
			const std::string& tagName)
	throw(CondDBException);

    // other methods not from the interface
    virtual void changeTagName( const std::string& oldName,
				const std::string& newName) const
	throw(CondDBException);

    virtual void getAllTaggedFolder( const string& tagName,
				     vector<string>& allFolder,
				     vector<SimpleTime>& allApplTime) const
	throw(CondDBException);

 private:

    // Interface to the ralational dbms
    MySqlDBMgr*           relDBMgr;
    MySqlTagMgr*          relTagMgr;
    MySqlFolderMgr*       relFolderMgr;
};

#endif






