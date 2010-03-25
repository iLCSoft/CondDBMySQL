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
/* $Id: basicSession.cxx,v 1.1 2006-11-15 14:09:22 poeschl Exp $ */

// $HEAD 10
//
// File: "basicSession.cxx"
//
// Created at Mon Aug 26 18:25:22 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




// Basic example of usage of the interfaces


#include <ConditionsDB/ICondDBMgr.h>
#include <ConditionsDB/CondDBMySQLMgrFactory.h>

#include <string>
#include <iostream>
using namespace std;

int main ( int /*argc*/, char* /*argv[]*/ )
{
    try {

	

	    ICondDBMgr* CondDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
	    
	    
	    cout << "condDBmgr->init" << endl;
	    
	    CondDBmgr->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");
	    // CondDBmgr->init("localhost:test_conddb02:jmal:online");
	    
	    // Create a ConditionsDB
	    
	    cout << "Ok" << endl;
	    CondDBmgr->startUpdate();
	    CondDBmgr->createCondDB();
	    CondDBmgr->commit();
	    
	    CondDBMySQLMgrFactory::destroyCondDBMgr( CondDBmgr );
	
	    return 0; // return success
	}
	
	catch (CondDBException &e)
	{
	    cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
		 << "***   error code: " << e.getErrorCode() << endl;
	    
	    return 1;  // return failure
	}  
}
