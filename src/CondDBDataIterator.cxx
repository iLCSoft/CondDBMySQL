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
/* $Id: CondDBDataIterator.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBDataIterator.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#include "CondDBDataIterator.h"
#include "Debug.h"
#include "CondDBObject.h"
#include "MySqlTypes.h"

#ifdef _WIN32
  typedef __int64 int64;
#else
  typedef long long int64;
#endif

// Constructors

CondDBDataIterator::CondDBDataIterator(MySqlDBMgr *dbMgr, MySqlResult *res, int fldId)
{
    int64 rows = res->countRows();
    Assert( rows > 0 );
    resultSet = res;
    folderId = fldId;
    currpos = 0;
    lastpos = rows - 1;
    relDBMgr = dbMgr;
}

// Destructor
CondDBDataIterator::~CondDBDataIterator()
{
    // have to delete the mysql resultSet
    delete resultSet;
}


bool CondDBDataIterator::hasNext() const
{
    if ( currpos < lastpos )
	return true;
    else
	return false;
}

bool CondDBDataIterator::hasPrevious() const
{
    if ( currpos > 0 )
	return true;
    else
	return false;
}


ICondDBObject* CondDBDataIterator::current()

{
    CondDBObject* CondObject = 0;
    resultSet->seek(currpos);
    CondObject = new CondDBObject(relDBMgr, resultSet, folderId);
    // There must exist an object
    Assert(CondObject != 0);
    return CondObject;
}

ICondDBObject* CondDBDataIterator::previous()

{
    if ( hasPrevious() ) {
	currpos--;
	return current();
    }
    else
	return 0;
}

ICondDBObject* CondDBDataIterator::next()

{
    if ( hasNext() ) {
	currpos++;
	resultSet->nextRow();
	CondDBObject* CondObject = 0;
	CondObject=new CondDBObject(relDBMgr, resultSet, folderId);
	Assert(CondObject!=0);
	return CondObject;
    }
    else
	return 0;
}


void CondDBDataIterator::goToLast()
{
    // goto the last entry
    currpos=lastpos;
    resultSet->seek(currpos);
}


void CondDBDataIterator::goToFirst()
{
    // goto the first entry
    currpos=0;
    resultSet->seek(currpos);
}


// THE END
