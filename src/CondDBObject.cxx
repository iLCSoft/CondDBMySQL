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
/* $Id: CondDBObject.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBObject.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#include "CondDBObject.h"
#include "MySqlTypes.h"
#include "Debug.h"
#include "MySqlDataMgr.h"
#include "MySqlObjectMgr.h"

// Default Constructor

CondDBObject::CondDBObject()
{
    theSince = 0;
    theTill  = 0;
    theInsertionTime = 0;
    theLayer = 0;
    theData  = "";
    theDescription = "";
    isNULLinterval = true;
}

/**
 * Constructor to be used explicitly from clients
 */
CondDBObject::CondDBObject(const CondDBKey&    since,
			   const CondDBKey&    till,
			   const CondDBKey&    insertionTime,
			   const long&         layer,
			   const string&       data,
			   const string&       description,
			   const bool          isNULLint)
{
    theSince         = since;
    theTill          = till;
    theInsertionTime = insertionTime;
    theLayer         = layer;
    theData          = data;
    theDescription   = description;
    isNULLinterval   = isNULLint;
    theInsertionTime = insertionTime;
}

/**
 * MySQL Specific Constructor
 */
CondDBObject::CondDBObject(MySqlDBMgr *dbMgr, MySqlResult *res, int folderId)

{
    int db_id;
    int data_id;
    int part_id;
    SimpleTime insertTime;
    // standard object fields

    theSince         = res->getCondDBKeyField(OBJKEY_SINCE_T);
    theTill          = res->getCondDBKeyField(OBJKEY_TILL_T);
    insertTime       = res->getTimeStampField(OBJKEY_INSERT_T);
    theInsertionTime = insertTime.toTimeT();
    theLayer         = res->getIntField(OBJKEY_LAYER);
    // extra fields
    db_id           = res->getIntField(OBJKEY_DB_ID);
    data_id         = res->getIntField(OBJKEY_DAT_ID);
    /*
      DebugMesg(CondDB, devl, "CondDBObject: DB_ID="<<db_id);
      DebugMesg(CondDB, devl, "CondDBObject: DATA_ID="<<data_id);
      DebugMesg(CondDB, devl, "CondDBObject: LAYER="<<theLayer);
    */
    if ( db_id != 0 ) {
        MySqlObjectMgr *objectMgr = dbMgr->getObjectMgr(db_id);
        part_id = objectMgr->getPartitionId(theSince, folderId);
        MySqlDataMgr *dataMgr = dbMgr->getDataMgr(db_id);
        // DebugMesg(CondDB, devl, "CondDBObject: *dataMgr=" << (void*)(dataMgr));
        MySqlResult *datarec = dataMgr->fetch(folderId, part_id, data_id);
	if ( datarec->countRows() == 0 ) {
	    DebugMesg(CondDB,devl,"CondDBObject: NO DATA ROWS");
	    theDescription = "";
	    theData = "";
	}
	else {
	    theDescription = datarec->getField(DATA_DESC);
	    // This has to work with binary data
	    unsigned long len = datarec->getLength(DATA_BLOCK);
	    theData.assign( datarec->getField(DATA_BLOCK), len );
	    isNULLinterval = false;
	}
	delete datarec;
    }
    else {
	theDescription = "Empty object";
	theData = "";
	isNULLinterval = true;
    }
}


/**
 * Destructor
 */
CondDBObject::~CondDBObject()
{
}

/**
 * Return the time since when the object is valid as a CondDBKey
 */
CondDBKey CondDBObject::validSince() const
{
    return theSince;
}

/**
 * Return the time till when the object is valid as a CondDBKey
 */
CondDBKey CondDBObject::validTill() const
{
    return theTill;
}

/**
 * Returns true or false whether the 'point' belongs to the
 * interval of validity
 */
bool CondDBObject::contains(const CondDBKey& point) const
{
    if ( point>=theSince && point<theTill )
	return true;
    else
	return false;
}

/**
 * Checks whether the interval is Null, i.e, if the object represents
 * an interval where no Conditions Object is defined.
 */
bool CondDBObject::isNullInterval() const
{
    return isNULLinterval;
}

/**
 * Gets the data as a string
 */
void CondDBObject::data(string& dataValue) const
{
    dataValue = theData;
}

/**
 * Gets the data as a ICondDBStreamable object
 */
void CondDBObject::data(ICondDBStreamable& obj) const

{
    // the conversion could generate an exception (in principle)
    obj.retrieveFromDB(theData);
}


void CondDBObject::insertionTime( SimpleTime& insTime ) const
{
    insTime = theInsertionTime;
}

CondDBKey CondDBObject::getInsertionTime()
{
    return theInsertionTime;
}

long CondDBObject::layer() const
{
    return theLayer;
}

void CondDBObject::description(string& descr) const
{
    descr = theDescription;
}


void CondDBObject::allTags(list<string>::iterator& /* assTags */) const
{
    // WARN: to be implemented
    DebugMesg(CondDB, user, "CondDBObject::allTags yet to be implemented");
}
