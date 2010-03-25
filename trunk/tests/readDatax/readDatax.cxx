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
/* $Id: readDatax.cxx,v 1.1 2006-11-15 14:11:38 poeschl Exp $ */

//
// File: "readData.cpp"
//


#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/CondDBMySQLMgrFactory.h"
#include "ConditionsDB/CondDBMySQLObjectFactory.h"

//NA IMPLEMENTACAO ORACLE JA NAO E NECESSARIO?
//#include <HepUtilities/SimpleTime.h>

#include <string>
#include <iostream>
using namespace std;

//void printObj(ICondDBObject* condDBobj);
//void printAllObjs(ICondDBDataIterator* iterObjs);


void printObj(ICondDBObject* condDBobj)
{
  CondDBKey since( condDBobj->validSince() );
  CondDBKey till( condDBobj->validTill() );
  SimpleTime insertionTime; 
  condDBobj->insertionTime(insertionTime);
  long layerNum( condDBobj->layer() ); 
  string data = "";      
  condDBobj->data(data);
  string description;
  condDBobj->description(description);
  
  cout << "Interval      : since " << since << " till " << till << endl;
  cout << "insertionTime : " << insertionTime << endl;
  cout << "        layer : " << layerNum << endl;
  cout << "         data : " << data << endl;
  cout << "  description : " << description << endl;
}


void printAllObjs(ICondDBDataIterator* iterObjs)
{
  ICondDBObject* oCondObject = 0;
  
  do 
  {
    oCondObject = iterObjs->current();
    printObj(oCondObject);
    cout << endl;
    delete oCondObject;
  } while ( iterObjs->next() );
}



int main ( int /*argc*/, char* /*argv[]*/ )
{
  string folder = "/cal/temp";
  //  CondDBKey point = 15;

  try {
    
    cout << "Testing - DB initialization" << endl;    
    ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
  
    //    condDBmgr->init("host:user:passwd");
    condDBmgr->init();
    
    condDBmgr->startRead();
    condDBmgr->openDatabase();
    condDBmgr->commit();

    ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();

    cout << "Example - looking into folder " << folder << endl;
    
    ICondDBObject* oCondObject = 0;
    ICondDBDataIterator* oCondIt = 0;
    condDBmgr->startRead();
    CondDBKey point = 15;

    cout << "Example - find of CondObj at point " << point << endl;

    //condDataAccess->findCondDBObject( oCondObject, folder, point);
    condDataAccess->browseAllObjects(oCondIt, folder);

    if ( oCondIt != 0 )
    {
      //CondDBKey since( oCondIt->current()->validSince() );
      //CondDBKey till( oCondIt->current()->validTill());//oCondObject->validTill() );
      SimpleTime insertionTime; 
      //long layerNum( oCondIt->current()->layer());//oCondObject->layer() ); 
      string data = "";

      //funcao especificada no inicio do programa para simplificar o codigo, nao pertence ao CondDB
      cout << "Object found for CondDBKey = " << point << endl;

//    printObj(oCondObject);
      
      printAllObjs(oCondIt);

      delete oCondIt;
/*
      // APRESENTACAO DOS RESULTADOS DOS OBJECTOS
      cout << "Object found for CondDBKey = " << point << endl; 
      cout << "     validity : since " << since << " till " << till << endl;
      cout << "insertionTime : " << insertionTime << endl; 
      cout << "        layer : " << layerNum << endl;
      cout << "         data : " << data << endl;
*/
      CondDBMySQLObjectFactory::destroyCondDBObject(oCondObject);
    }
    else
    {
      cout << "No object found for CondDBKey = " << point << endl;
    }
  
    condDBmgr->commit();
    
/* OUtro genero de teste. Faz procuras quer na 'horizontal' (procura por pasta e por tag), quer na 'vertical' (procura por CondDBpoints) 
    // Browsing
    
    ICondDBDataIterator* iterObjs = 0;

    // Browsing vertically

    cout << "\nAll intervals at point:" << point << endl;
      
    condDBmgr->startRead();

    //faz a procura
    condDataAccess->browseObjectsAtPoint(iterObjs, folder, point);
    
    printAllObjs(iterObjs);
    
    delete iterObjs;
    

    // Browsing horizontally

    cout << "\nAll intervals in the HEAD:" << endl;

    condDataAccess->browseObjectsInTag(iterObjs, folder);
    printAllObjs(iterObjs);
    delete iterObjs;
    
    condDBmgr->commit();
*/
    CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );

    return 0; // return success 
  }
  catch (CondDBException &e)
  {
    cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
         << "***   error code: " << e.getErrorCode() << endl;
    
    return 1;  // return failure
  }  

}







































