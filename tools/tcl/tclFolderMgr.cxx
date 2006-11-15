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
/* $Id: tclFolderMgr.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

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
#include <string>
#include <iostream>

#include "tclWrapper.h"
#include "tclException.h"

using namespace std;

// Wrapper for ICondFolderMgr methods

extern "C" void tclDeleteFolderMgr(ClientData ptr)
{
    ICondDBFolderMgr* mgr = (ICondDBFolderMgr*)ptr;
    delete mgr;
}

extern "C" int tclCondFolderMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[])
{
    try {
	string method;
	int parent;
	string obj = tclGetStr(objv[0]);
	ICondDBFolderMgr *mgr = (ICondDBFolderMgr*)ptr;
	if (argc<2)
	    THROW(obj+": must specify a method.", TCL_ERROR);

	method = tclGetStr(objv[1]);

	startChronometer();

	if(method=="createCondDBFolderSet") {
	    switch (argc) {
		case 3:
		    mgr->createCondDBFolderSet(tclGetStr(objv[2]));
		    break;
		case 4:
		    mgr->createCondDBFolderSet(tclGetStr(objv[2]),tclGetStr(objv[3]));
		    break;
		case 5:
		    mgr->createCondDBFolderSet(tclGetStr(objv[2]),tclGetStr(objv[3]),tclGetStr(objv[4]));
		    break;
		case 6:
		    if (Tcl_GetBoolean(interp, tclGetStr(objv[5]), &parent))
			THROW(obj+": bad argument", TCL_ERROR);
		    mgr->createCondDBFolderSet(tclGetStr(objv[2]),tclGetStr(objv[3]),tclGetStr(objv[4]),parent);
		    break;
		default:
		    THROW(obj+": wrong argument count.", TCL_ERROR);
	    }
	}
	else if(method=="createCondDBFolder") {
	    switch (argc) {
		case 3:
		    mgr->createCondDBFolder(tclGetStr(objv[2]));
		    break;
		case 4:
		    mgr->createCondDBFolder(tclGetStr(objv[2]),tclGetStr(objv[3]));
		    break;
		case 5:
		    mgr->createCondDBFolder(tclGetStr(objv[2]),tclGetStr(objv[3]),tclGetStr(objv[4]));
		    break;
		case 6:
		    if (Tcl_GetBoolean(interp, tclGetStr(objv[5]), &parent))
			THROW(obj+": bad argument", TCL_ERROR);
		    mgr->createCondDBFolder(tclGetStr(objv[2]),tclGetStr(objv[3]),tclGetStr(objv[4]),parent);
		    break;
		default:
		    THROW(obj+": wrong argument count.", TCL_ERROR);
	    }
	}
	else if(method=="exist") {
	    if (argc!=3) THROW(obj+": wrong argument count.", TCL_ERROR);
	    if(mgr->exist( tclGetStr(objv[2]) ))
		tclReturnInt(interp,1);
	    else 
		tclReturnInt(interp,0);
	}
	else if(method=="deleteFolder") {
	    if (argc!=3) THROW(obj+": wrong argument count.", TCL_ERROR);
	    mgr->deleteFolder( tclGetStr(objv[2]) );
	}
	else if(method=="getAllCondDBFolder") {
	    if (argc!=3) THROW(obj+": wrong argument count.", TCL_ERROR);
	    std::vector<std::string> folders;
	    mgr->getAllCondDBFolder( folders );
	    Tcl_Obj *listObj = Tcl_NewObj();
	    for (unsigned int i = 0; i < folders.size(); i++)
		Tcl_ListObjAppendElement(interp, listObj, Tcl_NewStringObj(folders[i].c_str(),-1));
	    Tcl_ObjSetVar2(interp, objv[2], NULL, listObj, 0);
	}
	else if(method=="getAllCondDBFolderSet") {
	    if (argc!=3) THROW(obj+": wrong argument count.", TCL_ERROR);
	    std::vector<std::string> folderSets;
	    mgr->getAllCondDBFolderSet( folderSets );
	    Tcl_Obj *listObj = Tcl_NewObj();
	    for (unsigned int i = 0; i < folderSets.size(); i++)
		Tcl_ListObjAppendElement(interp, listObj, Tcl_NewStringObj(folderSets[i].c_str(),-1));
	    Tcl_ObjSetVar2(interp, objv[2], NULL, listObj, 0);
	}
	else if(method=="getAllCondDBFolderBeneath") {
	    if (argc!=3) THROW(obj+": wrong argument count.", TCL_ERROR);
	    //    mgr->getAllCondDBFolderBeneath( const std::string& fullPathName,
	    //				  std::vector<std::string>& folderList );
	}
	else {
	    THROW(obj+": unknown method.\nValid methods are: "
		  "createCondDBFolderSet, createCondDBFolder, "
		  "exist, deleteFolder, getAllCondDBFolder, "
		  "getAllCondDBFolderSet, getAllCondDBFolderBeneath",
		  TCL_ERROR);
	}
    }
    catch (TclException &e) {
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



