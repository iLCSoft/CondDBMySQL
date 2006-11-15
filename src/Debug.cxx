/*

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
/* $Id: Debug.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "Debug.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




#include "Debug.h"

#include <iostream>
#include <fstream>
#include <iomanip>

// for sprintf
#include <stdio.h>

#ifndef NO_DEBUG

// Define the debug instance
// static ofstream out("/dev/pts/0");
// static ofstream out("cdb.log");

// A.V. Debug output will be to:
// 1. File "cdb.log", if environment variable COND_DB_DEBUGLOG is not set
// 2. Standard error, if COND_DB_DEBUGLOG is set to "/dev/stdout" or "/dev/stderr"
// 3. File with name specified by COND_DB_DEBUGLOG, in all other cases
static const char* outName = 
( getenv("COND_DB_DEBUGLOG") ? 
  ( string(getenv("COND_DB_DEBUGLOG")) == "/dev/stderr" ? 
    NULL : 
    ( string(getenv("COND_DB_DEBUGLOG")) == "/dev/stdout" ? 
      NULL : 
      getenv("COND_DB_DEBUGLOG") ) ) : 
  "cdb.log" );
static ostream* out = ( outName ? new ofstream( outName ) : (ostream*)&cerr );
COND_DB_DebugMessageClassDef(CondDB,out);

inline void string_toupper(string &s)
{
    string::iterator it=s.begin();
    while(it != s.end())
    {
	*it = toupper(*it);
	it++;
    }
}


DebugMessageClass::DebugMessageClass(const string &cname, ostream *os)
    :_cname(cname), _os( (!os) ? cerr : *os)
{
  //cerr << "__DebugMessageClass: Output in " 
  //     <<  ( outName ? outName : "cdb.log" ) << endl;
    string_toupper(_cname);
    string envTag = string("COND_DB_DEBUG_") + _cname;

    const char *setting = getenv(envTag.c_str());

    // if there is no specific setting, try to find a global one
    if (!setting)
	setting  = getenv("COND_DB_DEBUG");
    if (!setting)
	setVerbosity(verb_none);  // default value is none
    else
    {
	if (!strcmp("none",setting))
	    setVerbosity(verb_none);
	else if (!strcmp("prof",setting))
	    setVerbosity(verb_prof);
	else if (!strcmp("user",setting))
	    setVerbosity(verb_user);
	else if (!strcmp("devl",setting))
	    setVerbosity(verb_devl);
	else if (!strcmp("all",setting))
	    setVerbosity(verb_all);
	
	// EXTEND: check for redirection and open another stream 
    }
}

void DebugMessageClass::setVerbosity(verbosity v)
{
    _v = v;
}

#endif

const char *numberToString(long i)
{
    static char buf[32];
    sprintf(buf,"%ld",i);
    return buf;
}
