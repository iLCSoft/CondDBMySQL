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
/* $Id: CondDBTag.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBTag.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#ifndef CONDDBTAG_HH
#define CONDDBTAG_HH

#include "ConditionsDB/ICondDBTag.h"
//#include "HepUtilities/SimpleTime.h"
#include "ConditionsDB/SimpleTime.h"
#include "CondDBTagMgr.h"


/*
 * MySQL based implementation of ICondDBTag
 */

class CondDBTag : public ICondDBTag {

 public:

    // Constructors
    CondDBTag(const CondDBTagMgr* inCondTagMgr, MySqlResult* res)
	throw(CondDBException);

    // Destructor
    virtual ~CondDBTag();
    
    virtual std::string getName() const;
    
    virtual std::string getDescription() const;
    
    virtual void getCreationTime( SimpleTime& creationTime ) const;
    
    virtual void changeName( const std::string& newName )
	throw(CondDBException);
  
    virtual void
	getTaggedFolder( std::vector<std::string>& allFolder,
			 std::vector<SimpleTime>& allApplicationTime ) const;

 private:

    // attributes
    const CondDBTagMgr* condTagMgr;
    std::string tagName;
    std::string tagDescription;
    // TimeT is equivalent to long long which, in turn is the same as CondDBKey
    // TimeT is defined in SimpleTime.h
    //  TimeT tagCreationTime;
    CondDBKey tagCreationTime;
};

#endif






