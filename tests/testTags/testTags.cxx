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
/* $Id: testTags.cxx,v 1.1 2006-11-15 14:12:00 poeschl Exp $ */

// $HEAD 10
//
// File: "testTags.cxx"
//
// Created at Mon Aug 26 18:25:22 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




// Basic example of data storage.


#include <ConditionsDB/ICondDBMgr.h>
#include <ConditionsDB/CondDBMySQLMgrFactory.h>
#include <ConditionsDB/CondDBMySQLObjectFactory.h>

#include <string>
#include <iostream>
using namespace std;
// this requires the previous run of example basicSession

int main ( int /*argc*/, char* /*argv[]*/ )
{
    string folder = "/LHCb/Atlas/Tile/cal/Module_1";

    try {

	cout << "Example -  tag objects" << endl;
    
	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
  
	condDBmgr->init();
	// condDBmgr->init("localhost:test_conddb02:jmal:online");
    
	condDBmgr->startRead();
	condDBmgr->openDatabase();
	condDBmgr->commit();

	ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
	ICondDBFolderMgr*   condFolderMgr = condDBmgr->getCondDBFolderMgr(); 
	ICondDBTagMgr*         condTagMgr = condDBmgr->getCondDBTagMgr(); 

	cout << " Creating folder \"" << folder << "\"" << endl;
	condDBmgr->startUpdate();    
	condFolderMgr->createCondDBFolder(folder, "", "", true); 
	condDBmgr->commit();
    
	cout << " Storing objects in folder" << endl;
    
	condDBmgr->startUpdate();
    
	CondDBKey since, till;
	std::string data, desc;
	for (int obj = 1; obj <= 10; obj++)
	{
	    since = obj * 10;
	    till = since + 10;
	    data = "Object sample data";
	    desc = "Sample description";
		
	    ICondDBObject* condObject = CondDBMySQLObjectFactory::createCondDBObject(since, till, data, desc);
	    condDataAccess->storeCondDBObject( folder, condObject );
	    CondDBMySQLObjectFactory::destroyCondDBObject(condObject);
	}
	// Tag object
	cout << " Tagging the folder." << endl;
	condTagMgr->tag(folder, "Prodution");

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










