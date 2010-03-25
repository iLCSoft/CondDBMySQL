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
/* $Id: CondDBTimePath.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

#include "CondDBTimePath.h"


// Constructor
CondDBTimeStep::CondDBTimeStep()
{

}

CondDBTimeStep::~CondDBTimeStep()
{

}


CondDBKey CondDBTimeStep::getSince()
{
    return since;
}

CondDBKey CondDBTimeStep::getTill()
{
    return till;
}

long CondDBTimeStep::getId()
{
    return id;
}

void CondDBTimeStep::setSince(CondDBKey newSince)
{
    since = newSince;
}
void CondDBTimeStep::setTill(CondDBKey newTill)
{
    till = newTill;
}

void CondDBTimeStep::setId(long newId)
{
    id = newId;
}



