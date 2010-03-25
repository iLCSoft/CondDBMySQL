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
/* $Id: CondFolderSet.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondFolderSet.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#include "CondFolderSet.h"
#include "CondFolderMgr.h"
#include "Debug.h"
#include "MySqlFolderMgr.h"
#include "MySqlTypes.h"

CondFolderSet::CondFolderSet(const CondFolderMgr *inFolderMgr, MySqlResult *res)
{
    Assert(res->countRows()!=0);
  
    folderMgr       = inFolderMgr;
    theId           = res->getIntField(FOLDER_ID);
    theParent       = res->getIntField(FOLDER_PARENT);
    theAttributes   = res->getField(FOLDER_ATTR);
    theFullPathName = res->getField(FOLDER_PATHNAME);
    theDescription  = res->getField(FOLDER_DESC);
    theName         = theFullPathName.substr(theFullPathName.find_last_of("/") + 1, 
					     theFullPathName.size() - 1);
}

CondFolderSet::~CondFolderSet()
{
}


void CondFolderSet::getContainedFolderSet(std::vector<std::string>& allFolderSet) const
{
    MySqlResult *res = folderMgr->getContainedFolderRef(theId, MySqlFolderMgr::FolderSet);

	if (res->countRows())
		do {
			allFolderSet.push_back( res->getField(FOLDER_PATHNAME) );
		} while ( res->nextRow() );
    delete res;
}


void CondFolderSet::getContainedFolder(std::vector<std::string>& allFolder) const
{
    MySqlResult *res = folderMgr->getContainedFolderRef(theId, MySqlFolderMgr::Folder);
    
	if (res->countRows())
		do {
			allFolder.push_back( res->getField(FOLDER_PATHNAME) );
		} while ( res->nextRow() );
    delete res;
}









