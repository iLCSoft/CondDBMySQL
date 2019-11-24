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
/* $Id: CondDBTag.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondDBTag.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//



#include "CondDBTag.h"

#include "Debug.h"
#include "CondDBTagMgr.h"

#include "MySqlTagMgr.h"
#include "MySqlTypes.h"

CondDBTag::CondDBTag(const CondDBTagMgr* inCondTagMgr,
		     MySqlResult *res)

{
    Assert( res->countRows() > 0 );

    SimpleTime creationTime;
    tagName         = res->getField(TAG_NAME);
    //    tagDescription  = res->getField(TAG_DESC);
    creationTime    = res->getTimeStampField(TAG_INSERT_T);
    tagCreationTime = creationTime.toTimeT();
    condTagMgr      = inCondTagMgr;
}

CondDBTag::~CondDBTag()
{

}


string CondDBTag::getName() const
{
    return tagName;
}

string CondDBTag::getDescription() const
{
    return tagDescription;
}

void CondDBTag::getCreationTime(SimpleTime& creationTime) const
{
    creationTime=tagCreationTime;
}


void CondDBTag::changeName(const string& newName)

{
    condTagMgr->changeTagName(tagName, newName);
    string msg = "Tag name changed to "; msg += newName;
    DebugMesg(CondDB, user, msg);
}


void
CondDBTag::getTaggedFolder(vector<string>& allFolder,
			   vector<SimpleTime>& allApplicationTime) const
{
    condTagMgr->getAllTaggedFolder(tagName, allFolder, allApplicationTime);
}



// THE END
