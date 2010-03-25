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
/* $Id: tclException.h,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

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


#include <string>

using namespace std;

class TclException {
public:
    TclException(const string msg, unsigned long errcode) :
	message(msg), errorCode(errcode) { }
    const char*     getMessage() const { return message.c_str(); }
    unsigned long   getErrorCode() const { return errorCode; }
private:
    string message;
    unsigned long errorCode;
};

#define THROW(msg,c)  throw(TclException(msg,c))

