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
/* $Id: storeDatax.cxx,v 1.1 2006-11-15 14:11:47 poeschl Exp $ */

// NB
// File: "storeData.cpp"
//

#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/CondDBMySQLMgrFactory.h"
#include "ConditionsDB/CondDBObjFactory.h"
#include "exampleObject.h"

using namespace std;

#include <string>
#include <iostream>


int main ( int argc, char* argv[] )
{
    string folder = "/cal/temp";
    int n_obj, obj_ref=5;

    try {
	
	ExampleVector ex;
	
	cout << "Testing -  DB initialization" << endl;
	
	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
	
	//Database initialization
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
	
	cout << "Example -  storage of CondObject in folder " << folder << endl;
	
	condDBmgr->startUpdate();
	
	//How many objects will be stored...	 
  cout << "How many objects do you want to create?" << endl;
  if ( argc >=2 ) 
    n_obj = atoi( argv[1] );
  else
    cin >> n_obj;
	do
	{
	    ICondDBObject* condObject = CondDBObjFactory::createCondDBObject(obj_ref, obj_ref+7,
									     "sample data value \n \n object", "sample description" );

	    condDataAccess->storeCondDBObject( folder,condObject );
	    CondDBObjFactory::destroyCondDBObject(condObject);
	    n_obj--;
	}
	while(n_obj != 0);
	
	cout << "Testing - Sucessfully created " << n_obj << "Object " << endl; 
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










