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
/* $Id: tclDBMgr.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

/* 
 * TCL Wrapper for ConditionsDB
 * Lisbon ATLAS-DAQ Collaboration
 * Written by Jorge Lima (2003/04/15)
 * 
 * This is a conddb wrapper for TCL. The main purppose is to provide an 
 * easily configurable test framework. So do not expect the complete 
 * functionality of the bare C++ API. Nevertheless, care was taken to mimic
 * the object oriented nature of the C++ API into the TCL API. So we can, 
 * in principle, build any kind of performance tests in a way that exposes
 * what is being done at the C++ language level. 
 */

#include <tcl.h>
#include "ICondDBMgr.h"
#include "ICondDBFolderMgr.h"
#include "ICondDBTagMgr.h"
#include "ICondDBDataAccess.h"
#include "CondDBMySQLMgrFactory.h"

#include <iostream>
#include <string>
#include "tclException.h"
#include "tclWrapper.h"


using namespace std;

// wrapper for ICondDBMgr methods


extern "C" {

void tclDeleteCondDBMgr(ClientData ptr)
{
    ICondDBMgr *mgr = (ICondDBMgr*)ptr;
    CondDBMySQLMgrFactory::destroyCondDBMgr(mgr);
}

int tclCondDBMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[])
{
    try {
	string method;
	string obj = Tcl_GetString(objv[0]);
	char name[16];
	ICondDBMgr *mgr = (ICondDBMgr*)ptr;
	if (argc<2)
	    THROW(obj+": must specify a method.", TCL_ERROR);

	method = tclGetStr(objv[1]);

	startChronometer();
	if(method=="startUpdate") {
	    if (argc>2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    mgr->startUpdate();
	}
	else if(method=="startRead") {
	    if (argc>2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    mgr->startRead();
	}
	else if(method=="createCondDB") {
	    switch (argc) {
		case 2: mgr->createCondDB(); break;
		case 3: mgr->createCondDB(Tcl_GetString(objv[2])); break;
		default: THROW(obj+": wrong argument count.", TCL_ERROR);
	    }
	}
	else if(method=="openDatabase") {
	    if (argc>2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    mgr->openDatabase();
	}
	else if(method=="commit") {
	    if (argc>2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    mgr->commit();
	}
	else if(method=="init") {
	    switch (argc) {
		case 2: mgr->init(); break;
		case 3: mgr->init(Tcl_GetString(objv[2])); break;
		default: THROW(obj+": wrong argument count.", TCL_ERROR);
	    }
	}
	else if(method=="isCondDBcreated") {
	    if (mgr->isCondDBcreated())
		tclReturnInt(interp, 1);
	    else 
		tclReturnInt(interp, 0);
	}
	else if(method=="getCondDBFolderMgr") {
	    ICondDBFolderMgr* foldermgr = mgr->getCondDBFolderMgr(); 
	    tclFindUniqCmd(interp, name, "fldmgr");
	    Tcl_CreateObjCommand (interp, name, &tclCondFolderMgr,
				  (ClientData) foldermgr,
				  &tclDeleteFolderMgr);
	    tclReturnStr(interp, name);	    
	}
	else if(method=="getCondDBDataAccess") {
	    ICondDBDataAccess* datamgr = mgr->getCondDBDataAccess();
	    tclFindUniqCmd(interp, name, "objmgr");
	    Tcl_CreateObjCommand (interp, name, &tclCondDataMgr,
				  (ClientData) datamgr,
				  &tclDeleteDataMgr);
	    tclReturnStr(interp, name);
	}
	else if(method=="getCondDBTagMgr") {
	    ICondDBTagMgr* tagmgr = mgr->getCondDBTagMgr(); 
	    tclFindUniqCmd(interp, name, "tagmgr");
	    Tcl_CreateObjCommand (interp, name, &tclCondTagMgr,
				  (ClientData) tagmgr,
				  &tclDeleteTagMgr);
	    tclReturnStr(interp, name);
	}
	else {
	    THROW(obj+": unknown method.\nValid methods are: "
		  "startUpdate, startRead, commit, createCondDB, "
		  "openDatabase, init, getFolderMgr, getDataMgr, getTagMgr",
		  TCL_ERROR);
	}
    }
    catch (TclException &e) {
	Tcl_SetObjResult(interp, Tcl_NewStringObj(e.getMessage(),-1) );
	return e.getErrorCode();
    }
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;
	return TCL_ERROR;
    }
    stopChronometer();
    return TCL_OK;
}

}



