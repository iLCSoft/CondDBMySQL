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
/* $Id: MySqlFolderMgr.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "MySqlFolderMgr.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//


#ifndef MYSQL_FOLDERMGR_H
#define MYSQL_FOLDERMGR_H

// C++ include files
#include <string>
#include "Debug.h"
//#include "ConditionsDB/CondDBKey.h"

#include "MySqlConnection.h"

class MySqlConnection;
class MySqlDBMgr;
class MySqlHandle;


// These must be in sync with the queries
#define FOLDER_ID        0
#define FOLDER_PARENT    1
#define FOLDER_INSERT_T  2
#define FOLDER_PATHNAME  3
#define FOLDER_DESC      4
#define FOLDER_ATTR      5
#define FOLDER_DDTYPE    6
#define FOLDER_TBL_ID    7
#define FOLDER_IS_SET    8



class MySqlFolderMgr : public MySqlConnection {

public:
    enum folder_type {Folder = 0, FolderSet = 1, Any = 2};

    MySqlFolderMgr(MySqlDBMgr *dbMgr,
		   MySqlHandle *mysqlH);

    MySqlFolderMgr(MySqlDBMgr *dbMgr);

    ~MySqlFolderMgr();

    MySqlResult *find(const string& pathName, folder_type type)
	throw(CondDBException);

    MySqlResult *find(int folderId, folder_type type)
	throw(CondDBException);

    int getId(const string& pathName, folder_type type)
	throw(CondDBException);

    MySqlResult *browse(const string& basename, folder_type type)
	throw(CondDBException);

    MySqlResult *browseChilds(int parentId, folder_type type)
	throw(CondDBException);

    int getFolderType(const string& fullPathName)
	throw(CondDBException);

    bool exist(const string& pathName, folder_type type)
	throw(CondDBException);

    bool exist(int folderId, folder_type type)
	throw(CondDBException);

    int store(const string& pathName,
	      const string& description,
	      const string& attributes,
	      int parentId, int tblId, folder_type type, int ddtype = 0)
	throw(CondDBException);

    void deleteFolder(int folderId)
	throw(CondDBException);

// Associate tags to folders
// This operation must be paired with the tag object association
    bool isTagged(int folderId, int tagId)
	throw(CondDBException);

    void addTag(int folderId, int tagId)
	throw(CondDBException);

    void removeTag(int folderId, int tagId)
	throw(CondDBException);

    MySqlResult *browseTaggedFolders(const std::string& tagName)
	throw(CondDBException);
 
private:
    MySqlDBMgr *databaseMgr;

};

#endif /* MYSQL_FOLDERMGR_H */








