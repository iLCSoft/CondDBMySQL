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
/* $Id: CondFolder.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondFolder.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#ifndef CONDFOLDER_HH
#define CONDFOLDER_HH

#include "ConditionsDB/ICondDBFolder.h"
#include "ConditionsDB/CondDBKey.h"

/*
 * MySQL based implementation of ICondDBFolder
 */

class MySqlResult;

class CondFolder : public ICondDBFolder {

public:

  // Constructors
  CondFolder(MySqlResult *res);

  CondFolder(MySqlResult *res, std::string &addrHeader);
  
  // Destructor
  virtual ~CondFolder();

  // methods of ICondDBFolderGeneral
  virtual std::string getName() const
    { return theName; }
  
  virtual std::string getFullPathName() const
    { return theFullPathName; }

  virtual std::string getAttributes() const
    { return theAttributes; }

  virtual std::string getDescription() const 
    { return theDescription; }

private:

  // attributes
  int theId;
  int theParent;
  CondDBKey insertTime;
  std::string theName;
  std::string theAttributes;
  std::string theFullPathName;
  std::string theDescription;

};

#endif  // CONDFOLDER_HH





