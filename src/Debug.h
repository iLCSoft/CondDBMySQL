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
/* $Id: Debug.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

// $HEAD 10
//
// File: "Debug.h"
//
// Created at Wed Aug 21 12:02:11 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//



//                         -*- Mode: C++ -*- 
// Debug.h --
// created on Mon Mar  1 18:05:06 1999
// by         Dirk Duellmann 
//
// $Id: Debug.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $
//


//
// Modifica o código para usar a consola para debug ou, preferencialmente,
// a função mysql_debug().
//

#ifndef Debug_h
#define Debug_h

#include "ConditionsDB/CondDBException.h"

#ifndef NO_DEBUG
#include <iostream>
#include <string>
/// verbosity control for a debug message class
/** each message class define one object of this
    class which is later used by the DebugMesg() macro
    Each message defines a message class (typically
    the module it belongs to) and a minimum verbosity level
    to be printed. The verbosity level mean

    none: no message is printed (not to be used when defining a message)
    prof: only those messages are printed which do not disturb profiling
          measurements
    user: only those messages are printed which are of "general" interest
    devl: also those messages are printed which are only of interest for the
          package developer
    more: more paranoid messages are printed 
    all:  any debug message will be printed. 
*/
using namespace std;
class DebugMessageClass {
public:
    /// message verbosity
    typedef enum {verb_none=0, verb_prof, verb_user, verb_devl, verb_more, verb_all=10} verbosity;
    
    DebugMessageClass(const string & cname, ostream *os=0);
    
    void setVerbosity(verbosity v);
    
    bool checkVerbosity(verbosity msg_v) const
	{ return ( _v >= msg_v ) ? true : false; }
    
    ostream & os() 
	{ return _os;}
    
private:
    std::string     _cname;
    verbosity  _v;
    ostream   &_os;
};

// define (or declare) a message class, usually the module name (sm,ls,ps ...) 
# define COND_DB_DebugMessageClassDcl(clss) \
  extern DebugMessageClass clss ## _DebugMessageClass

# define COND_DB_DebugMessageClassDef(clss,out) \
    DebugMessageClass clss ## _DebugMessageClass( #clss,out )

/// check if if debug level for class "msgClass" is higher than "verbose"
/** 
    e.g. if (DebugLevel(sm,user)) 
            do_additional_check_on_user_level_or_higer();
*/
# define DebugLevel(msgClass,verbose) \
 (msgClass ## _DebugMessageClass.checkVerbosity(DebugMessageClass::verb_ ## verbose))


// print a debug message for class "msgClass" if verbosity is >= than verbose
/**
   e.g. DebugMesg(sm,user,"something went wrong here!");
*/
# define DebugMesg(msgClass, verbose, msg) \
   {if (DebugLevel(msgClass,verbose)) \
      { msgClass ## _DebugMessageClass.os() << std::string(#msgClass "(" #verbose "): " ) <<  msg << endl;  } }

// declare global message classes
COND_DB_DebugMessageClassDcl(CondDB);

#else
# define DebugLevel(c,v) (0)
# define DebugMesg(c,v,m)
# define COND_DB_DebugMessageClassDcl(c)
# define COND_DB_DebugMessageClassDef(c,o)
#endif

// Additional macros

#define EXP_MYSQL_CODE 2000

const char *numberToString(long i);



#define THROWEX(a, errCode ) throw(CondDBException( std::string(a) + \
"\n*** in file: " + __FILE__  + " line: " + numberToString( __LINE__ ) \
						  , errCode))

#define MyTHROWEX(mySQL) throw(CondDBException( std::string("MySQL error [") + \
 numberToString(mysql_errno(mySQL)) + "] :\n" + std::string(mysql_error(mySQL)) + "\n" + \
 "\n*** catched in file: " + __FILE__ + " line: " + numberToString( __LINE__ ),\
 EXP_MYSQL_CODE))

#define Assert( exp ) \
    if (!(exp)) THROWEX("Assertion failed: " #exp, 0) // WARN: chose right errorCode



#endif /* Debug_h */


