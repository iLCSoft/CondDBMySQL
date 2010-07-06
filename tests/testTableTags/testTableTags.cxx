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
/* $Id: testTableTags.cxx,v 1.1 2006-11-15 14:11:58 poeschl Exp $ */

#include <ConditionsDB/ICondDBMgr.h>
#include <ConditionsDB/CondDBMySQLMgrFactory.h>
#include <ConditionsDB/CondDBMySQLTableFactory.h>
#include <ConditionsDB/ICondDBTable.h>

#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

void printTable(ICondDBTable *table);

bool createTag (string tagName, string tagDescr, ICondDBTagMgr *tagMgr)
{
    try
    {
	tagMgr->createCondDBTag(tagName, tagDescr);
	return false;
    }
    catch (CondDBException &e)
    {
	return true;
    }
}


int main ( int /*argc*/, char* /*argv[]*/ )
{
    string folder = "/muon/dcs/tableTag";

    try {

	cout << "Example -  tag objects" << endl;
    
	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
  
	condDBmgr->init();
    
	condDBmgr->startRead();
	condDBmgr->openDatabase();
	condDBmgr->commit();

	ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
	ICondDBFolderMgr*   condFolderMgr = condDBmgr->getCondDBFolderMgr(); 
	ICondDBTagMgr*         condTagMgr = condDBmgr->getCondDBTagMgr(); 

	// Create tags
	if (createTag("Production", "Table with tags", condTagMgr))
	    cout << "Tag already exists, proceeding" << endl;
	else
	    cout << "Tag created" << endl;

	// Tag folders
	cout << " Tagging folder" << endl;
	if (condFolderMgr->exist(folder))
	    condTagMgr->tag(folder, "Production");
	else
	{
	    cout << "Folder " << folder << " doesn't exist" << endl;
	    cout << "Run storeTableTag first" << endl;
	    return 1;
	}

    	condDBmgr->commit();
	
	// Retrieve by tag

	//Create tables
	ICondDBTable *table = CondDBMySQLTableFactory::createCondDBTable(); //new CondDBTable();

	//Setting up search times

	CondDBKey time = 15000000;
	CondDBKey since=5000000, till=100000000;

	// find with tag
	cout << "Find tag" << endl;
	condDataAccess->findCondDBObject(table, folder, time, "", "", 0, "Production");
	printTable (table);
	
	// Clear table
	CondDBMySQLTableFactory::destroyCondDBTable( table ); //delete table;
	table = CondDBMySQLTableFactory::createCondDBTable(); //new CondDBTable();

	// browseHistory with tag
	
	cout << "Browse hist" << endl;
	condDataAccess->browseHistory(folder, table, since, till, "", "", 0, "Production");
	printTable (table);

	// Clear table
	CondDBMySQLTableFactory::destroyCondDBTable( table ); //delete table;
	table = CondDBMySQLTableFactory::createCondDBTable(); //new CondDBTable();

	// browseTag
	cout << "Browse tag" << endl;
	condDataAccess->browseObjectsInTag(table, folder, "Production");
	printTable (table);

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


void printTable(ICondDBTable *table)
{
    //********* Function that prints the ICondDBTable table ******//
    //    It uses the proper methos to access to the data 
    //    that is in memory

    if (table->getNumColumns())
    {
	vector<string> names;                     //A vector for table names
	vector<ICondDBTable::cdb_types> types;    //A vector for table types

	SimpleTime since, till;                   //Simpletime variables to
	                                          //store since 
	                                          //and till times

	table->getNames(names);                   //Retrieving column names
	table->getTypes(types);                   //Retrieving column types
		
	cout << "--------------------------- Table BEGIN  -----------------------------" << endl;
	cout << "Table [" << table->getNumRows() << "]x[" << names.size() << "]" << endl; 
	for (unsigned i=0; i< table->getNumRows(); i++){  //for each row, 
                                                          //get the values
	    table->getSinceTime(i, since);
	    table->getTillTime(i, till);
	    
	    cout << "{ Row  " << i+1 << " Begin }" << endl;
	    cout <<"Since: " << since << " ----  Till: " << till << endl;
	    
	    vector<string> tmpStr;        // temporary vector for row storage

	    table->getRow(i, tmpStr);     //get one entire row
	                                  //The first element of the 
	                                  //tmpStr vector will be the number 
                                          //of elements in case of an array
	                                  //followed by the values
	    
	    long pos=0;                  //Set the position in the 
                                         //tmpSrt vector
	    
	    for (unsigned columnNumber=0;columnNumber<names.size();columnNumber++){
		cout << "Column name: " << names[columnNumber]  << endl;

		int n_vals;
		if (ICondDBTable::cdbLongLong < types[columnNumber]){           //If the values in 
                                                      //the column cells are 
		    n_vals=atol(tmpStr[pos].c_str()); //arrays of some type
		    pos++;                            //Incrementing the 
		                                      //position 
		                                      //in the tmpStr vector
		}
		else 
		    n_vals=1;     //Otherwise, the number of values is only 1

		cout <<"Data in cell ["<<i+1<< "]x["<<columnNumber+1<< "]: ";
		//printing the data for each cell
		for (long c=pos; c< pos+n_vals; c++){
		    cout << tmpStr[c];
		    if (n_vals!=1)
			cout << " ; ";
		}

		pos+=n_vals;        //Incrementing the position in the 
		                    //tmpStr vector
		cout << endl;
	    }
	    cout << "{ Row  " << i +1  << " End }" << endl << endl;
	}
	cout << "--------------------------- Table end  -----------------------------" << endl;

    }
    else
	cout << "Table empty" << endl;
}








