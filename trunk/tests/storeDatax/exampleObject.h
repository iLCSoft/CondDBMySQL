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
/* $Id: exampleObject.h,v 1.1 2006-11-15 14:11:47 poeschl Exp $ */

// $HEAD 10
//
// File: "genericObject.h"
//
// Created at Mon Aug 26 18:25:22 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//




//                         -*- Mode: C++ -*- 
// exampleObject.h --
// created on Tue Jan 30 16:57:07 2001
// by         Dirk Duellmann 
//
// $Id: exampleObject.h,v 1.1 2006-11-15 14:11:47 poeschl Exp $
//
#ifndef exampleObject_h
#define exampleObject_h

#include "ConditionsDB/CondDBVector.h"

class ExampleVector : public CondDBVector<double> {

public:
  virtual ~ExampleVector() {}        
};


#endif /* exampleObject_h */

