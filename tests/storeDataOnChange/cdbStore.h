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

#ifndef CDBSTORE_H
#define CDBSTORE_H
///////////////////////////////////////////////////////////////////////
///           cdbStore.h                                            ///
///                                                                 ///
///               Luis Pedro 2004                                   ///
///     Interface class to access the database methods              ///
///                                                                 ///
///            1st version: 8 March 2004                            ///
///           Last updated: 11 March 2003                           ///
///////////////////////////////////////////////////////////////////////

#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/CondDBMySQLMgrFactory.h"
#include "ConditionsDB/ICondDBTable.h"
#include "ConditionsDB/CondDBMySQLTableFactory.h"

extern "C" {
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
};
#include <iostream>


using namespace std;

class cdbStore{

 public:
  cdbStore                                  (int, int);
  ~cdbStore                                 ();

  void                  init               (string profile="");
  void                  createFolder       (std::string folder, ICondDBTable * table);
  void                  store              (std::string folder, ICondDBTable* table);
  void                  printTable         (ICondDBTable *table);

  int                   getNextUpdateTime  ();
  int                   getIdToUpdate      ();   
  double                generateNumber();

 private:
  ICondDBMgr            * condDBmgr;
  ICondDBDataAccess     * condDataAccess;
  ICondDBFolderMgr      * condFolderMgr;
  int                   m_timeToNextUpdate;
  int                   m_idToUpdate;
  int                   m_maxTimeToUpdate;
  int                   m_numIds;

  void                  setNextUpdateTime ();
  void                  setNextIdToUpdate ();




};
#endif //CDBSTORE_H
