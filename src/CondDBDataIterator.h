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
/* $Id: CondDBDataIterator.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBDataIterator.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#ifndef CONDDB_DATAITERATOR_H
#define CONDDB_DATAITERATOR_H

#include "ConditionsDB/ICondDBDataIterator.h"


class MySqlResult;
class MySqlDBMgr;

/**
 * Objy based implementation of ICondDBDataIterator for vertical iteration
 */

class CondDBDataIterator : public ICondDBDataIterator {

public:

  // Constructors
    CondDBDataIterator(MySqlDBMgr *dbMgr, MySqlResult *res, int fldId);

    // Destructor
    virtual ~CondDBDataIterator();

    virtual bool hasNext() const;
    virtual bool hasPrevious() const;

    virtual ICondDBObject* current()
	;

    virtual ICondDBObject* previous()
	;

    virtual ICondDBObject* next()
	;

    virtual void goToFirst();
    virtual void goToLast();

 private:

    MySqlDBMgr *relDBMgr;
    MySqlResult *resultSet;
    int folderId;
    // attributes
    #ifdef _WIN32
	  __int64 lastpos;
  	  __int64 currpos;
    #else
      long long lastpos;
      long long currpos;
    #endif
    // methods

};

#endif  /* CondDBDataIteratorHorizontal_h */
