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
/* $Id: CondDBMySQLMgrFactory.h,v 1.1 2006-11-15 14:03:15 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBMySQLMgrFactory.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#ifndef CONDDBMYSQLMGR_HH
#define CONDDBMYSQLMGR_HH


#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/ICondDBDataAccess.h"
#include "ConditionsDB/ICondDBFolderMgr.h"


/**
 * Factory method of ICondDBMgr for implementation based on Objectivity.
 */

class CondDBMySQLMgrFactory {

public:

  /// Creates an instance of ICondDBMgr.
  static ICondDBMgr* createCondDBMgr()
    ;

  /// Destroys the instance of ICondDBMgr.
  static void destroyCondDBMgr(ICondDBMgr* CondDBMgr)
    ;

};

#endif
