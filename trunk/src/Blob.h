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
/* $Id: Blob.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

#ifndef WIN32
//# include <ansidecl.h>
#ifdef __GNUC__
# if __GNUC__ < 3
#  include <strstream>
#  define MYSQLSTREAM strstream
#  define USE_OLD_STREAMS
# else
#  include <sstream>
#  define MYSQLSTREAM std::ostringstream 
# endif
#else
#  include <sstream>
#  define MYSQLSTREAM std::ostringstream 
#endif
#endif

#ifdef WIN32
# include <sstream>
# define MYSQLSTREAM std::ostringstream
#endif

#include <stdio.h>
#include <vector>
#include <string>
#include "CondDBTable.h"

template <class T>
void makeBlob(T *array, std::string *Blob, long size);

void vectorToBlob(std::string *blob, std::vector <std::string>& data, CondDBTable::cdb_types type);

template <class T>
void makeVector (T *val, std::vector <std::string>& data, long size);

void blobToVector (char *Blob, std::vector <std::string>& data, CondDBTable::cdb_types type);



