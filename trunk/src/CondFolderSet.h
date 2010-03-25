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
/* $Id: CondFolderSet.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "CondFolderSet.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#ifndef CONDFOLDERSET_HH
#define CONDFOLDERSET_HH

#include "ConditionsDB/ICondDBFolder.h"
#include "CondFolderMgr.h"

/*
 * MySQL based implementation of ICondDBFolderSet
 */

class CondFolderSet : public ICondDBFolderSet {

public:

  // Constructors

  CondFolderSet(const CondFolderMgr *inFolderMgr, MySqlResult *res);

  // Destructor
  virtual ~CondFolderSet();

  // methods of ICondDBFolderGeneral
  virtual std::string getName() const
    { return theName; }
  
  virtual std::string getFullPathName() const
    { return theFullPathName; }

  virtual std::string getAttributes() const
    { return theAttributes; }

  virtual std::string getDescription() const 
    { return theDescription; }
  
  virtual void
  getContainedFolderSet(std::vector<std::string>& allFolderSet) const;

  virtual void
  getContainedFolder(std::vector<std::string>& allFolder) const;

  
private:

  // attributes
  const CondFolderMgr* folderMgr;
  int theId;
  int theParent;
  std::string theName;
  std::string theAttributes;
  std::string theFullPathName;
  std::string theDescription;

};

#endif  // CONDFOLDERSET_HH


