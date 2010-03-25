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
/* $Id: tclDataIterator.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

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
#include "ICondDBFolderMgr.h"
#include "ICondDBDataIterator.h"
#include <string>
#include <iostream>

#include "tclWrapper.h"
#include "tclException.h"

using namespace std;

// Wrapper for ICondFolderMgr methods

extern "C" void tclDeleteDataIterator(ClientData ptr)
{
    ICondDBDataIterator *mgr = (ICondDBDataIterator*)ptr;
    delete mgr;
}

extern "C" int tclCondDataIterator(ClientData ptr, Tcl_Interp *interp, int argc,  Tcl_Obj *CONST objv[])
{
    try {
	char name[64];
	string method;
	string obj = tclGetStr(objv[0]);
	ICondDBDataIterator *it = (ICondDBDataIterator*)ptr;
	if (argc<2)
	    THROW(obj+": must specify a method.", TCL_ERROR);

	method = tclGetStr(objv[1]);

	startChronometer();

	if(method=="hasNext") {
	    if (argc!=2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    if(it->hasNext())
		tclReturnInt(interp,1);
	    else 
		tclReturnInt(interp,0);
	}
	else if(method=="hasPrevious") {
	    if (argc!=2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    if(it->hasPrevious())
		tclReturnInt(interp,1);
	    else 
		tclReturnInt(interp,0);
	}
	else if(method=="current") {
	    if (argc!=2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    ICondDBObject* obj = it->current();
	    tclFindUniqCmd(interp, name, "condobj");
	    Tcl_CreateObjCommand (interp, name, &tclCondDBObj,
				  (ClientData) obj,
				  &tclDeleteCondDBObj);
	    tclReturnStr(interp,name);
	}
	else if(method=="previous") {
	    if (argc!=2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    ICondDBObject* obj = it->previous();
	    if ( obj==0 ) {
		tclReturnInt(interp, 0);
	    }
	    else {
		tclFindUniqCmd(interp, name, "condobj");
		Tcl_CreateObjCommand (interp, name, &tclCondDBObj,
				      (ClientData) obj,
				      &tclDeleteCondDBObj);
		tclReturnStr(interp,name);
	    }
	}
	else if(method=="next") {
	    if (argc!=2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    ICondDBObject* obj = it->next();
	    if ( obj==0 ) {
		tclReturnInt(interp, 0);
	    }
	    else {
		tclFindUniqCmd(interp, name, "condobj");
		Tcl_CreateObjCommand (interp, name, &tclCondDBObj,
				      (ClientData) obj,
				      &tclDeleteCondDBObj);
		tclReturnStr(interp,name);
	    }
	}
	else if(method=="goToLast") {
	    if (argc!=2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    it->goToLast();
	}
	else if(method=="goToFirst") {
	    if (argc!=2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    it->goToFirst();
	}
	else {
	    THROW(obj+": unknown method.\nValid methods are: "
		  "hasNext, hasPrevious, previous, next, goToLast, goToFirst",
		  TCL_ERROR);
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

