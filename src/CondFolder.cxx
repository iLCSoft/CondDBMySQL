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
/* $Id: CondFolder.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondFolder.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#include "CondFolder.h"
#include "Debug.h"
#include "MySqlFolderMgr.h"
#include "MySqlTypes.h"

CondFolder::CondFolder(MySqlResult *res)
{
    Assert(res->countRows()!=0);

// Administrative fields used internaly only
// By now are not used at all

    theId = res->getIntField(FOLDER_ID);
    theParent = res->getIntField(FOLDER_PARENT);
    insertTime = res->getTimeStampField(FOLDER_INSERT_T);
/*
    tblId = res->getIntField(FOLDER_TBL_ID);
    ddType = res->getIntField(FOLDER_DDTYPE);
*/

// Demanded by the interface
    theAttributes   = res->getField(FOLDER_ATTR);
    theFullPathName = res->getField(FOLDER_PATHNAME);
    theDescription  = res->getField(FOLDER_DESC);
    theName         = theFullPathName.substr(theFullPathName.find_last_of("/") + 1, 
					     theFullPathName.size() - 1);
}

CondFolder::CondFolder(MySqlResult *res, std::string &addrHeader)
{
    Assert(res->countRows()!=0);

// Administrative fields used internaly only
// By now are not used at all

    theId = res->getIntField(FOLDER_ID);
    theParent = res->getIntField(FOLDER_PARENT);
    insertTime = res->getTimeStampField(FOLDER_INSERT_T);
/*
    tblId = res->getIntField(FOLDER_TBL_ID);
    ddType = res->getIntField(FOLDER_DDTYPE);
*/

// Demanded by the interface
    theAttributes   = res->getField(FOLDER_ATTR);
    theFullPathName = res->getField(FOLDER_PATHNAME);

    // let's check if the header already has data.
    // if it's empty we must prepend "our" header
    std::string tmpHeader = res->getField(FOLDER_DESC);
    if (tmpHeader == "") {
	theDescription  = addrHeader + tmpHeader;
    }
    else
	theDescription = tmpHeader;
    
    //    theDescription  = addrHeader + res->getField(FOLDER_DESC);
    theName         = theFullPathName.substr(theFullPathName.find_last_of("/") + 1, 
					     theFullPathName.size() - 1);

}

CondFolder::~CondFolder()
{
}















