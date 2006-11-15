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
/* $Id: tclWrapper.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

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
#include <iostream>

#include <string>

#include "tclException.h"
#include "tclWrapper.h"

using namespace std;

extern "C" {

// Tcl conversions
int tclGetInt(Tcl_Obj *obj)
{
    int val;
    if(Tcl_GetIntFromObj(NULL, obj, &val))
	THROW("Error converting object to integer", TCL_ERROR);
    return val;
}

void tclReturnLLong(Tcl_Interp *interp, CondDBKey k)
{
    char tmp[64];
    sprintf(tmp, "%lld", k);
    tclReturnStr(interp, tmp);
}

void tclSetVarLLong(Tcl_Interp *interp, Tcl_Obj *var, CondDBKey k)
{
    char tmp[64];
    sprintf(tmp, "%lld", k);
    tclSetVarStr(interp,var,tmp);
}

// Utils
void tclFindUniqCmd(Tcl_Interp *interp, char *nameptr, char *base)
{
    Tcl_CmdInfo cmdinfo;
    int i = 0;
    
    do {
	i++;
	sprintf(nameptr, "%s_%02d", base, i);
    } while(Tcl_GetCommandInfo(interp, nameptr, &cmdinfo));
}


// Factory methods

int tclCreateCondDBMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[])
{
    char name[16];
    string cmd = tclGetStr(objv[0]);
    ICondDBMgr* condDBmgr;
    try {
	if (argc!=1) THROW(cmd+": wrong argument count.", TCL_ERROR);
	condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
	tclFindUniqCmd(interp, name, "dbmgr");
    }
    catch (CondDBException &e)
    {
    }
    catch (TclException &e) {
	tclReturnStr(interp,e.getMessage());
	return e.getErrorCode();
    }
    Tcl_CreateObjCommand (interp, name, &tclCondDBMgr,
			  (ClientData) condDBmgr,
			  &tclDeleteCondDBMgr);
    tclReturnStr(interp, name);
    return TCL_OK;
}

int tclDestroyCondDBMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[])
{
    try {
	Tcl_CmdInfo cmdinfo;
	char *cmd = tclGetStr(objv[0]);
	if (argc!=2) THROW(string(cmd)+": wrong argument count.", TCL_ERROR);
	char *obj = tclGetStr(objv[1]);
	if(Tcl_GetCommandInfo(interp, obj, &cmdinfo)==0)
	    THROW(string(cmd)+": the specified object doesn't exist.", TCL_ERROR);
	if(cmdinfo.objProc!=&tclCondDBMgr)
	    THROW(string(cmd)+": "+string(obj)+" is not a CondDBMgr.", TCL_ERROR);
	Tcl_DeleteCommand(interp, obj);
    }
    catch (TclException &e)
    {
	tclReturnStr(interp,e.getMessage());
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

int tclCreateCondDBObject(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[])
{
    char name[16];
    char *sptr;
    int len;
    string cmd = tclGetStr(objv[0]);
    ICondDBObject* condDBobj;
    try {
	if (argc!=5) THROW(cmd+": wrong argument count.", TCL_ERROR);
	CondDBKey since = tclGetLLong(objv[1]);
	CondDBKey till = tclGetLLong(objv[2]);
	sptr = Tcl_GetStringFromObj(objv[3], &len);
	string data(sptr, len);
	sptr = Tcl_GetStringFromObj(objv[4], &len);
	string desc(sptr, len);
	condDBobj = CondDBObjFactory::createCondDBObject(since, till, data, desc);
	tclFindUniqCmd(interp, name, "condobj");
	Tcl_CreateObjCommand (interp, name, &tclCondDBObj,
			      (ClientData) condDBobj,
			      &tclDeleteCondDBObj);
	tclReturnStr(interp, name);
    }
    catch (CondDBException &e)
    {
    }
    catch (TclException &e) {
	tclReturnStr(interp,e.getMessage());
	return e.getErrorCode();
    }
    return TCL_OK;
}

int tclDestroyCondDBObject(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[])
{
    try {
	Tcl_CmdInfo cmdinfo;
	char *cmd = tclGetStr(objv[0]);
	if (argc!=2) THROW(string(cmd)+": wrong argument count.", TCL_ERROR);
	char *obj = tclGetStr(objv[1]);
	if(Tcl_GetCommandInfo(interp, obj, &cmdinfo)==0)
	    THROW(string(cmd)+": the specified object doesn't exist.", TCL_ERROR);
	if(cmdinfo.objProc!=&tclCondDBObj)
	    THROW(string(cmd)+": "+string(obj)+" is not a CondObj.", TCL_ERROR);
	Tcl_DeleteCommand(interp, obj);
    }
    catch (TclException &e)
    {
	tclReturnStr(interp,e.getMessage());
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

// Interface to TCL
// Assure C style linkage

int Tclconddb_Init(Tcl_Interp* interp)  
{

    if (Tcl_PkgProvide (interp, "conddb", "1.0") != TCL_OK) {
	return TCL_ERROR;
    }
    Tcl_CreateObjCommand (interp, "CondDBMySQLMgrFactory::createCondDBMgr", 
			  &tclCreateCondDBMgr,
			  (ClientData) NULL,
			  (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand (interp, "CondDBMySQLMgrFactory::destroyCondDBMgr", 
			  &tclDestroyCondDBMgr,
			  (ClientData) NULL,
			  (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand (interp, "CondDBObjFactory::createCondDBObject",
			  &tclCreateCondDBObject,
			  (ClientData) NULL,
			  (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand (interp, "CondDBObjFactory::destroyCondDBObject",
			  &tclDestroyCondDBObject,
			  (ClientData) NULL,
			  (Tcl_CmdDeleteProc *) NULL);
    if(performanceInit(interp) != TCL_OK) {
	return TCL_ERROR;
    }
    return TCL_OK;

}
}
//----------------------------------------------------------------------------------


