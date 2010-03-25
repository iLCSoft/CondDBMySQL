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
/* $Id: CondDBTimePath.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

#ifndef CONDDBTIMEPATH_HH
#define CONDDBTIMEPATH_HH

#include "ConditionsDB/ICondDBTimePath.h"

class CondDBTimeStep : public ICondDBTimeStep {

 public:

    CondDBTimeStep ();

    virtual ~CondDBTimeStep ();
    
    virtual CondDBKey getSince();
    virtual CondDBKey getTill();
    virtual long getId();

    virtual void setSince(CondDBKey newSince);
    virtual void setTill(CondDBKey newTill);
    virtual void setId(long newId);

 private:

    CondDBKey  since;
    CondDBKey  till;
    long       id;

};

#endif






