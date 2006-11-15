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
/* $Id: CondDBOracleDBMgrFactory.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */

#ifndef CONDORACLEDBMGR_HH
#define CONDORACLEDBMGR_HH


#include "ConditionsDB/ICondDBMgr.h"


/**
 * Factory method of ICondDBMgr for implementation based on Oracle.
 */

class CondDBOracleDBMgrFactory {

public:
 
  /// Creates an instance of ICondDBMgr.
  static ICondDBMgr* createCondDBMgr()
    throw(CondDBException);

  /// Destroys the instance of ICondDBMgr.
  static void destroyCondDBMgr(ICondDBMgr* CondDBMgr)
    throw(CondDBException);
};

#endif
