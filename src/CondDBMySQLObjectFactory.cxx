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
/* $Id: CondDBMySQLObjectFactory.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondObjFactory.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#include "ConditionsDB/CondDBMySQLObjectFactory.h"
#include "CondDBObject.h"
#include "Debug.h"

ICondDBObject*
CondDBMySQLObjectFactory::createCondDBObject(const CondDBKey& since,
				     const CondDBKey& till,
				     const std::string& data,
				     const std::string& description)

{
  if (since >= till)
    THROWEX("invalid input in createCondObject: since > till", 0);

  bool      isNULLinterval = false;
  long      layer = 0;   // the right value will be set internally
  CondDBKey insTime = 0; // the right value will be set internally

  return new CondDBObject(since, till, insTime,
			  layer, data, description, isNULLinterval);
}

ICondDBObject*
CondDBMySQLObjectFactory::createCondDBObject(const CondDBKey& since,
				     const CondDBKey& till,
				     const ICondDBStreamable& obj,
				     const std::string& description)

{
  Assert(since < till);
  std::string data;
  obj.storeToDB(data);

  bool      isNULLinterval = false;
  long      layer = 0;   // the right value will be set internally
  CondDBKey insTime = 0; // the right value will be set internally

  return new CondDBObject(since, till, insTime,
			  layer, data, description, isNULLinterval);
}


// WARN: is this necessary? why not the user to call directly delete CondObj ??
void
CondDBMySQLObjectFactory::destroyCondDBObject(ICondDBObject* CondObj)

{
  delete CondObj;
}
