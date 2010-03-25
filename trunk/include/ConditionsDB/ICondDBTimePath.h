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
/* $Id: ICondDBTimePath.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */

#ifndef ICONDDBTIMEPATH_HH
#define ICONDDBTIMEPATH_HH

#include "ConditionsDB/CondDBException.h"

#include <string>
#include <vector>
#include "ConditionsDB/CondDBKey.h"

class ICondDBTimeStep;

typedef std::vector<ICondDBTimeStep*> ICondDBTimePath;

class ICondDBTimeStep {

 public:

    virtual ~ICondDBTimeStep () {}
    
    virtual CondDBKey getSince() = 0;
    virtual CondDBKey getTill() = 0;
    virtual long getId() = 0;

    virtual void setSince(CondDBKey newSince) = 0;
    virtual void setTill(CondDBKey newTill) = 0;
    virtual void setId(long newId) = 0;
};



/*
class ICondDBTimePath {

 public:

    virtual ~ICondDBTimePath () {}

    virtual std::string getFolder() = 0;
    virtual ICondDBTimeStep operator [] (unsigned i) = 0; 

 private:

    std::string folder;
    std::vector <ICondDBTimeStep> path;

};
*/
#endif
