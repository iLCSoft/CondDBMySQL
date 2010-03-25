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
/* $Id: tclCondObj.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

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
#include "CondDBMySQLMgrFactory.h"
#include "CondDBObjFactory.h"

#include <string>
#include <iostream>

#include "tclException.h"
#include "tclWrapper.h"

using namespace std;

extern "C" {

void tclDeleteCondDBObj(ClientData ptr)
{
    ICondDBObject *obj = (ICondDBObject*)ptr;
    CondDBObjFactory::destroyCondDBObject(obj);
}

int tclCondDBObj(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[])
{
    try {
	string method;
	string obj = tclGetStr(objv[0]);

	ICondDBObject *cdo = (ICondDBObject*)ptr;
	if (argc<2)
	    THROW(obj+": must specify a method.", TCL_ERROR);

	method = tclGetStr(objv[1]);

	if (method=="validSince") {
	    tclReturnLLong(interp, cdo->validSince());
	}
	else if (method=="validTill") {
	    tclReturnLLong(interp, cdo->validTill());
	}
	else if (method=="contains") {
	    if (argc!=3) THROW(obj+"wrong argument number",TCL_ERROR);
	    CondDBKey point = tclGetLLong(objv[2]);
	    tclReturnInt(interp, cdo->contains(point));
	}
	else if (method=="isNullInterval") {
	    tclReturnInt(interp, cdo->isNullInterval());
	}
	else if (method=="data") {
	    if (argc!=3) THROW(obj+"wrong argument number",TCL_ERROR);
	    string dataValue;
	    cdo->data(dataValue);
	    Tcl_Obj *object = Tcl_NewStringObj(dataValue.c_str(),dataValue.size());
	    Tcl_ObjSetVar2(interp, objv[2], NULL, object, 0);
	}
	else if (method=="insertionTime") {
	    if (argc!=3) THROW(obj+"wrong argument number",TCL_ERROR);
	    SimpleTime insTime;
	    cdo->insertionTime( insTime );
	    tclSetVarStr(interp,objv[2],insTime.toNetLogger().c_str());
	}
	else if (method=="layer") {
	    tclReturnInt(interp, cdo->layer());
	}
	else if (method=="description") {
	    if (argc!=3) THROW(obj+"wrong argument number",TCL_ERROR);
	    string desc;
	    cdo->description(desc);
	    Tcl_Obj *object = Tcl_NewStringObj(desc.c_str(),desc.size());
	    Tcl_ObjSetVar2(interp, objv[2], NULL, object, 0);
	}
	else {
	    THROW(obj+": unknown method.\nValid methods are: "
		  "validSince, validTill, "
		  "contains, isNullInterval, insertionTime"
		  "layer, data, description", TCL_ERROR);
	}
	/* Not yet implemented
	   /// Returns the list of all associated tags.
	   virtual void allTags(std::list<std::string>::iterator& assocTags) const = 0;
	*/
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
    return TCL_OK;
}

}


