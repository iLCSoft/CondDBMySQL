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
/* $Id: CondDBObject.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBObject.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#ifndef CONDOBJECT_HH
#define CONDOBJECT_HH

#include "ConditionsDB/ICondDBObject.h"
#include "ConditionsDB/SimpleTime.h"
#include "CondDBInterface.h"

class MySqlResult;

/*
 * MySQL based implementation of ICondDBObject
 */

class CondDBObject : public ICondDBObject {

public:

    // Methods of ICondDBObject
    
    // Constructors

    CondDBObject();

    CondDBObject(const CondDBKey&    since,
		 const CondDBKey&    till,
		 const CondDBKey&    insertionTime,
		 const long&         layer,
		 const string&       data,
		 const string&       description,
		 const bool          isNULLint);

    // Specific to the MySQL implementation.
    CondDBObject(MySqlDBMgr *dbMgr, MySqlResult* res, int folderId)
	throw(CondDBException);
  
    // Destructor
    virtual ~CondDBObject();
    
    
    // Get methods

    virtual CondDBKey validSince() const;
    virtual CondDBKey validTill() const;
    virtual bool contains(const CondDBKey& point) const;
    virtual bool isNullInterval() const;
    virtual void data(string& dataValue) const;
    virtual void data(ICondDBStreamable &s) const
	throw(CondDBException);
    virtual void insertionTime( SimpleTime& insTime ) const;
    virtual long layer() const;
    virtual void description(string& descr) const;
    virtual void allTags(list<string>::iterator& assocTags) const;

    // not in the interface
    // Set methods

    void setSince(const CondDBKey& since);
    void setTill(const CondDBKey& till);
    void setLayer(const long& layer);
    void setData(const string& data);
    void setDescription(const string& description);
    void setNULLinterval(const bool NULLinterval);
    CondDBKey getInsertionTime();

private:

    CondDBKey      theSince;
    CondDBKey      theTill;
    CondDBKey      theInsertionTime;

    // The layer is a representation of version
    int            theLayer;

    string         theData;
    string         theDescription;
    bool           isNULLinterval;
};

#endif


