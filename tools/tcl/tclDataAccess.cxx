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
/* $Id: tclDataAccess.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

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
#include "ICondDBDataAccess.h"
#include "ICondDBObject.h"
#include "CondDBObjFactory.h"
#include "CondDBKey.h"
#include <string>
#include <iostream>

#include "tclWrapper.h"
#include "tclException.h"


using namespace std;


// Wrapper for ICondDataAccess methods

extern "C" {

void tclDeleteDataMgr(ClientData ptr)
{
    ICondDBDataAccess* mgr = (ICondDBDataAccess*)ptr;
    delete mgr;
}

int tclCondDataMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[])
{
    try {
	string method, data, desc;
	string obj = tclGetStr(objv[0]);
	char name[16];
	ICondDBDataAccess *mgr = (ICondDBDataAccess*)ptr;
	if (argc<2)
	    THROW(obj+": must specify a method.", TCL_ERROR);
	
	method = tclGetStr(objv[1]);

	startChronometer();

	if(method=="storeCondDBObject") {
	    if (argc!=4) THROW(obj+" wrong argument number",TCL_ERROR);
	    Tcl_CmdInfo cmdinfo;
	    char *cdbObj = tclGetStr(objv[3]);
	    char *folder = tclGetStr(objv[2]);
	    if (Tcl_GetCommandInfo(interp, cdbObj, &cmdinfo) == 0)
		THROW(string(cdbObj)+" doesn't exist",TCL_ERROR);
	    if (cmdinfo.objProc!=&tclCondDBObj)
		THROW(string(cdbObj)+" is not a CondObj",TCL_ERROR);
	    ICondDBObject* condObject = (ICondDBObject *)(cmdinfo.objClientData);
	    mgr->storeCondDBObject(folder, condObject);
	}
	else if(method=="findCondDBObject") {
	    ICondDBObject* condObject;
	    switch (argc) {
		case 5:
		    mgr->findCondDBObject(condObject,tclGetStr(objv[3]),tclGetInt(objv[4]));
		    break;
		case 6:
		    mgr->findCondDBObject(condObject,tclGetStr(objv[3]),tclGetInt(objv[4]),tclGetStr(objv[4]));
		    break;
		default:
		    THROW(obj+"wrong argument number",TCL_ERROR);
	    }
	    if (condObject==0) {
		tclSetVarInt(interp, objv[2], 0);
	    }
	    else {
		tclFindUniqCmd(interp, name, "condobj");
		Tcl_CreateObjCommand (interp, name, &tclCondDBObj,
				      (ClientData) condObject,
				      &tclDeleteCondDBObj);
		tclSetVarStr(interp, objv[2], name);
	    }
	}
	else if(method=="browseObjectsAtPoint") {
	    if (argc!=5) THROW(obj+"wrong argument number",TCL_ERROR);
	    ICondDBDataIterator* oCondIt = 0;
	    mgr->browseObjectsAtPoint(oCondIt, tclGetStr(objv[3]), tclGetInt(objv[4]));
	    if (oCondIt==0) {
		tclSetVarInt(interp, objv[2], 0);
	    }
	    else {
		tclFindUniqCmd(interp, name, "objiter");
		Tcl_CreateObjCommand (interp, name, &tclCondDataIterator,
				      (ClientData) oCondIt,
				      &tclDeleteDataIterator);
		tclSetVarStr(interp, objv[2], name);
	    }
	}
	else if(method=="browseObjectsInTag") {
	    ICondDBDataIterator* oCondIt = 0;
	    switch (argc) {
		case 4:
		    mgr->browseObjectsInTag(oCondIt, tclGetStr(objv[3]));
		    break;
		case 5:
		    mgr->browseObjectsInTag(oCondIt, tclGetStr(objv[3]), tclGetStr(objv[4]));
		    break;
		default:
		    THROW(obj+"wrong argument number",TCL_ERROR);
	    }
	    if (oCondIt==0) {
		tclSetVarInt(interp, objv[2], 0);
	    }
	    else {
		tclFindUniqCmd(interp, name, "objiter");
		Tcl_CreateObjCommand (interp, name, &tclCondDataIterator,
				      (ClientData) oCondIt,
				      &tclDeleteDataIterator);
		tclSetVarStr(interp, objv[2], name);
	    }
	}
	else if(method=="browseAllObjects") {
	    if (argc!=4) THROW(obj+"wrong argument number",TCL_ERROR);
	    ICondDBDataIterator* oCondIt = 0;
	    mgr->browseAllObjects(oCondIt, tclGetStr(objv[3]));
	    if (oCondIt==0) {
		tclSetVarInt(interp, objv[2], 0);
	    }
	    else {
		tclFindUniqCmd(interp, name, "objiter");
		Tcl_CreateObjCommand (interp, name, &tclCondDataIterator,
				      (ClientData) oCondIt,
				      &tclDeleteDataIterator);
		tclSetVarStr(interp, objv[2], name);
	    }
	}
	else if(method=="dump") {
	    if (argc!=3) THROW(obj+"wrong argument number",TCL_ERROR);
	    tclReturnStr(interp, "Not yet implemented");
	}
	else {
	    THROW(obj+": unknown method.\nValid methods are: "
		  "storeCondDBObject, findCondDBObject, "
		  "browseObjectsAtPoint, browseObjectsInTag, "
		  "browseAllObjects", TCL_ERROR);
	}
    }
    catch (TclException &e) {
	tclReturnStr(interp, e.getMessage());
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




