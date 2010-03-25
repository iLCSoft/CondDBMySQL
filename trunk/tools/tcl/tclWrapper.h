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
/* $Id: tclWrapper.h,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

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
 * 
 */

#include <tcl.h>
#include "ICondDBObject.h"

// Main file functions
extern "C" {

int tclGetInt(Tcl_Obj *obj);
void tclReturnLLong(Tcl_Interp *interp, CondDBKey k);
void tclSetVarLLong(Tcl_Interp *interp, Tcl_Obj *var, CondDBKey k);

#define tclGetStr(obj)           Tcl_GetString(obj)
#define tclReturnStr(i,str)      Tcl_SetObjResult(i, Tcl_NewStringObj(str,-1) )
#define tclReturnInt(i,val)      Tcl_SetObjResult(i, Tcl_NewIntObj(val) )
#define tclSetVarStr(i,var,str)  Tcl_ObjSetVar2(i, var, NULL, Tcl_NewStringObj(str, -1), 0)
#define tclSetVarInt(i,var,val)  Tcl_ObjSetVar2(i, var, NULL, Tcl_NewIntObj(val), 0)
#define tclGetLLong(obj)          atoll(Tcl_GetString(obj))

/*
ICondDBObject* TclArray2CondObject(Tcl_Interp *interp, char *varname);
void CondObject2TclArray(Tcl_Interp *interp, char *varname, ICondDBObject* condObject);
*/
void tclFindUniqCmd(Tcl_Interp *interp, char *nameptr, char *base);
int tclCreateCondDBMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[]);
int tclDestroyCondDBMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[]);

// Wrapper for ICondDBMgr methods
void tclDeleteCondDBMgr(ClientData ptr);
int tclCondDBMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[]);

// Wrapper for ICondDataAccess methods
void tclDeleteDataMgr(ClientData ptr);
int tclCondDataMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[]);

// Wrapper for ICondFolderMgr methods
void tclDeleteFolderMgr(ClientData ptr);
int tclCondFolderMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[]);

// Wrapper for ICondTagMgr methods
void tclDeleteTagMgr(ClientData ptr);
int tclCondTagMgr(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[]);

// Wrapper for ICondDBDataIterator
void tclDeleteDataIterator(ClientData ptr);
int tclCondDataIterator(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[]);

// Wrapper for ICondDBObject
void tclDeleteCondDBObj(ClientData ptr);
int tclCondDBObj(ClientData ptr, Tcl_Interp *interp, int argc, Tcl_Obj *CONST objv[]);

// Performance measurement tool
void startChronometer(void);
void stopChronometer(void);
int performanceInit(Tcl_Interp *interp);
}


