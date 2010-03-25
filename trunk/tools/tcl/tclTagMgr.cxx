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
/* $Id: tclTagMgr.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

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
#include "ICondDBTagMgr.h"
#include "tclException.h"
#include "tclWrapper.h"

#include <string>
#include <iostream>

using namespace std;


// Wrapper for ICondTagMgr methods

extern "C" {
void tclDeleteTagMgr(ClientData ptr)
{
    ICondDBTagMgr* mgr = (ICondDBTagMgr*)ptr;
    delete mgr;
}

int tclCondTagMgr(ClientData ptr, Tcl_Interp *interp, int argc,  Tcl_Obj *CONST objv[])
{
    try {
	string method;
	string obj = tclGetStr(objv[0]);
	ICondDBTagMgr *mgr = (ICondDBTagMgr*)ptr;
	if (argc<2)
	    THROW(obj+": must specify a method.", TCL_ERROR);

	method = tclGetStr(objv[1]);

	startChronometer();
	if(method=="createCondDBTag") {
	    switch (argc) {
		case 2:
		    mgr->createCondDBTag( tclGetStr(objv[2]) );
		    break;
		case 3:
		    mgr->createCondDBTag( tclGetStr(objv[2]), tclGetStr(objv[3]) );
		    break;
		default:
		    THROW(obj+": wrong argument count.", TCL_ERROR);
	    }
	}
	else if(method=="deleteCondDBTag") {
	    if (argc!=3) THROW(obj+": wrong argument count.", TCL_ERROR);
	    mgr->deleteCondDBTag( tclGetStr(objv[2]) ); 
	}
	else if(method=="getAllCondDBTag") {
	    if (argc!=2) THROW(obj+": wrong argument count.", TCL_ERROR);
	    vector<string> allTag;
	    mgr->getAllCondDBTag( allTag );
	    for (unsigned int i = 0; i < allTag.size(); i++)
		Tcl_AppendElement(interp, allTag[i].c_str());
	}
	else if(method=="tag") {
	    switch (argc) {
		case 4:
		    mgr->tag( tclGetStr(objv[2]), tclGetStr(objv[3]) );
		    break;
		case 5:
		    mgr->tag( tclGetStr(objv[2]), tclGetStr(objv[3]), tclGetStr(objv[4]) );
		    break;
		default:
		    THROW(obj+": wrong argument count.", TCL_ERROR);
	    }
	}  
	else if(method=="untag") {
	    if (argc!=4) THROW(obj+": wrong argument count.", TCL_ERROR);
	    mgr->untag( tclGetStr(objv[2]), tclGetStr(objv[3]) );
	}
	else if(method=="getTaggedFolder") {
	    if (argc!=4) THROW(obj+": wrong argument count.", TCL_ERROR);
	    //getTaggedFolder(std::vector<std::string>& allFolder,
	    //	  std::vector<SimpleTime>& allApplicationTime) const = 0;
	}
	else {
	    THROW(obj+": unknown method.\nValid methods are: "
		  "createCondDBTag, deleteCondDBTag, getAllCondDBTag, "
		  "tag, untag",
		  TCL_ERROR);
	}
    }
    catch (TclException &e)
    {
	Tcl_AppendResult(interp, e.getMessage(), NULL);
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
