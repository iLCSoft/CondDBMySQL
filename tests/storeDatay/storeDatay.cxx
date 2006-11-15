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
/* $Id: storeDatay.cxx,v 1.1 2006-11-15 14:11:49 poeschl Exp $ */

#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/CondDBMySQLMgrFactory.h"
#include "ConditionsDB/CondDBMySQLObjectFactory.h"
#include "exampleObject.h"

using namespace std;

#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


int main ( int argc, char* argv[] )
{
  string folder = "/cal/temp";
  
  int num_obj;

  int i=1;

  try {

  ExampleVector ex;
    
    cout << "Testing -  DB initialization" << endl;
    
    ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
  
    //LIGACAO A BASE DE DADOS
    condDBmgr->init();
    
    condDBmgr->startRead();
    condDBmgr->openDatabase();
    condDBmgr->commit();

    ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
    ICondDBFolderMgr*  condFolderMgr  = condDBmgr->getCondDBFolderMgr(); 
    
    cout << "Example -  CondFolder verification" << endl;

    condDBmgr->startUpdate();
    cout << "Verification of existence of folder :" << folder << endl;    
    if (!condFolderMgr->exist(folder))
    	{
	  cout << "CondDB folder does not exist \nProceding to cretion of CondDBFolder " << folder << endl;
	  condFolderMgr->createCondDBFolder( folder, "", "", true); 
    	// add the system name of an existing DB if you wish
    	condDBmgr->commit();
    	}
    else
      cout << "CondDBFolder already exists...proceding!" << endl;
    /* PARTE DISPENSAVEL: LISTA TODOS OS FOLDERS EXISTENTES NA BD
    // Dump the list of stored CondFolder
    cout << "Example - all stored CondFolder:" << endl;    
    condDBmgr->startRead();
    // cout << "Ainda por implementar! (describe)" << endl;
    condFolderMgr->describe("/");
    condDBmgr->commit();
    */    
    cout << "Example -  storage of CondObject in folder " << folder << endl;
    
    condDBmgr->startUpdate();
    
    cout << "How many objects do you want to create?" << endl;
    if ( argc >=2 ) 
      num_obj = atoi( argv[1] );
    else
      cin >> num_obj;
    
    /*
    char tmp[255];
    sprintf(tmp,"%d",n_obj);
    setenv("COND_DB_NUM_OBJ", tmp, 1);
    */
    
    do  
    {
	ICondDBObject* condObject = CondDBMySQLObjectFactory::createCondDBObject(i, i+10,"sample data value \n \n object", "sample description" );

	condDataAccess->storeCondDBObject( folder,condObject );
	CondDBMySQLObjectFactory::destroyCondDBObject(condObject);
	i++;
    }
    while(i < num_obj+1);

    cout << "Testing - Sucessfully created - Object " << endl; 
    condDBmgr->commit();
    CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );
    return 0; // return success
  }

  catch (CondDBException &e)
  {
    cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
         << "***   error code: " << e.getErrorCode() << endl;
    
    return 1;  // return failure
  }
}










