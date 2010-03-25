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
/* $Id: storeTable.cxx,v 1.1 2006-11-15 14:11:51 poeschl Exp $ */

/******************************************************/
/*              Example for table storage             */
/*               in Conditions Database               */
/*            1st version: 2 November 2003            */
/*           Last updated: 15 November 2003           */
/******************************************************/

#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/CondDBMySQLMgrFactory.h"
#include "ConditionsDB/CondDBMySQLTableFactory.h"
#include "ConditionsDB/ICondDBTable.h"
//#include "CondDBTable.h"

using namespace std;

#include <string>
#include <iostream>

void printTable(ICondDBTable *table);

int main ()
{

    //Let's store data in forlder /moun/dcs
    string parent = "/muon/dcs";

    try {

	//Starting set up of the tables in memory:
	//There will be 2 tables. One using CondDBTable features and 
        //the other one using the CondDBTable with ID storage mechanism
	
	vector <string> colNamesTable;        //for the names of the colums 
                                              //in the CondDBTable

	colNamesTable.push_back("Temperature");
	colNamesTable.push_back("Voltage");
	colNamesTable.push_back("Distance");
	colNamesTable.push_back("Name");
	colNamesTable.push_back("LongDistance");
	colNamesTable.push_back("Valid");
	colNamesTable.push_back("Temperatures");
	colNamesTable.push_back("Voltages");
	colNamesTable.push_back("Distances");
	colNamesTable.push_back("Names");
	colNamesTable.push_back("LongDistances");
	colNamesTable.push_back("Valids");

	vector <string> colNamesTableId;      //for the names of the 
                                              //colums in the CondDBTableID

	colNamesTableId.push_back("Id");
	colNamesTableId.push_back("HighVoltage");
	colNamesTableId.push_back("Current");
	
	
	//Creating two CondDBTables
	ICondDBTable* table= CondDBMySQLTableFactory::createCondDBTable(12); //new CondDBTable(12);     //one table with 
                                                     //8 columns

	ICondDBTable* tableId = CondDBMySQLTableFactory::createCondDBTable(3); //new CondDBTable(3);  //one table with 
                                                     //3 columns
	
	//Settig up the Names and types of the columns of ICondDBTable* table

	//1st Column - name: Temperature; type: Float
	table->setName(0, colNamesTable[0]); table->setType(0, ICondDBTable::cdbFloat);

	//2nd Column - name: Voltage; type: Int
	table->setName(1, colNamesTable[1]); table->setType(1, ICondDBTable::cdbInt);
	
	//3rd Column - name: Distance; type: Double
	table->setName(2, colNamesTable[2]); table->setType(2, ICondDBTable::cdbDouble);

	//4th Column - name: Name; type: String
	table->setName(3, colNamesTable[3]); table->setType(3, ICondDBTable::cdbString);

	//5th Column - name: Name; type: Long Long
	table->setName(4, colNamesTable[4]); table->setType(4, ICondDBTable::cdbLongLong);

	//6th Column - name: Name; type: Bool (in reality its a short int)
	table->setName(5, colNamesTable[5]); table->setType(5, ICondDBTable::cdbBool);

	//7th Column - name: Temperatures; type: Array of Float
	table->setName(6, colNamesTable[6]); table->setType(6, ICondDBTable::cdbArrayFloat);

	//8th Column - name: Voltages; type: Array of Int
	table->setName(7, colNamesTable[7]); table->setType(7, ICondDBTable::cdbArrayInt);

	//9th Column - name: Distances; type: Array of Double
	table->setName(8, colNamesTable[8]); table->setType(8, ICondDBTable::cdbArrayDouble);

	//10th Column - name: Names; type: Array of String
	table->setName(9, colNamesTable[9]); table->setType(9, ICondDBTable::cdbArrayString);
	
	//11th Column - name: Name; type: Long Long
	table->setName(10, colNamesTable[10]); table->setType(10, ICondDBTable::cdbArrayLongLong);

	//12th Column - name: Name; type: Bool (again, a short int in reality)
	table->setName(11, colNamesTable[11]); table->setType(11, ICondDBTable::cdbArrayBool);


	//Settig up the Names and types of the columns of ICondDBTable* tableId

	//1st Column - name: Id; type: String
	//This column will handle the desirable Id for the rows
	tableId->setName(0, colNamesTableId[0]); tableId->setType(0, ICondDBTable::cdbString);

	//2nd Column - name: HighVoltage; type: Float
	tableId->setName(1, colNamesTableId[1]); tableId->setType(1, ICondDBTable::cdbFloat);

	//3rd Column - name: Current; type: Int
	tableId->setName(2, colNamesTableId[2]); tableId->setType(2, ICondDBTable::cdbInt);
	
	
	//Tables set up finished. Two diferent tables have been created 
        //and its definition it's in memory. 

	//**************************************************************
	//We can now create the folder in the CondDB database
		cout << "Testing -  DB initialization" << endl;

	//The standard CondDB initialization
	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
	condDBmgr->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");
	condDBmgr->createCondDB();
	condDBmgr->openDatabase();
	ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
	ICondDBFolderMgr*  condFolderMgr  = condDBmgr->getCondDBFolderMgr(); 
	
	cout << "CondFolder verification" << endl;
	
	string folder = parent + "/table";      //A folder for the table
	string folder2 = parent + "/tableId";   //Another folder for the tableId

	//creating folder for table if needed:
	if (!condFolderMgr->exist(folder)){
	    cout << "CondDB folder does not exist \nProceeding to creation of CondDBFolder " << folder << endl;
	    condFolderMgr->createCondDBFolder( folder, table, "", "", true);	    
	}
	else
	    cout << "CondDBFolder already exists...proceeding!" << endl;
	
	//end of folder creation

	//Creating folder for tableId if needed:
	if (!condFolderMgr->exist(folder2)){
	    cout << "CondDB folder does not exist \nProceeding to creation of CondDBFolder " << folder2 << endl;
	    condFolderMgr->createCondDBFolder( folder2, tableId, "", "", true, ICondDBFolder::STRUCTID);
	}
	else
	    cout << "CondDBFolder already exists...proceeding!" << endl;
	
	//End of folder creation
	//********************************************************

	//What we have for the momment is one table with 12 columns, and another with 3. 
        //For the moment, the 
        //number of lines in each table is zero. 
        //The next setep is to resize each one for the desirable size:

	cout << "Resizing Tables" << endl;
	table->resize(1);             //Resizes table for one rows

	tableId->resize(10);          //Resizes tableId for 10 rows
	
	//Let's add some data to the tables....
	//The next operations will exemplify how can one add data to 
	//the the in-memory tables
	
 	vector <string> tmpArray, tmpCell, Id; //Auxiliar vectors 

        //tmpArray will handle with arrays to be stored in one table cell
        //tmpColumn will handle the data of column with single values 
        //in each cell tmpColumnArr will handle the data of column in wich 
        //each cell will be an array of values Id will be the vector that 
        //will handle the names of the desirable Ids for the table rows
	
	//The tmpCell vector will handle with single values to be stored
	//With this vector we'll create the columns 
        //to be stored in the database

	tmpCell.push_back("1234.567");  tmpCell.push_back("1234567");
	tmpCell.push_back("12345678.56789"); tmpCell.push_back("Some String");
	tmpCell.push_back("12345678901234567"); tmpCell.push_back("1");
	tmpCell.push_back("Block1"); tmpCell.push_back("Block2");
	
	Id.push_back("Sensor1"); Id.push_back("Sensor2"); 

	//Let's fill in the data into the table
	for (int c=0; c<6; c++)
	{
	    for (int i=1; i<=10; i++)       //This adds 10 diferent values to
		tmpArray.push_back(tmpCell[c]);  //tmpArray vector
	    
	    //How simply is to add a column of values to the table:
	    table->setCell (c, 0, tmpCell[c]);    //Sets the first 6 
	                                          //columns (single values)
	    
	    table->setCell (c+6, 0, tmpArray);    //Sets the 6 last 
                                                  //columns (each cell is an array)
	    tmpArray.clear();
	}

	//Fill in data into tableId 

	for (int b=0; b<5; b++)  
	    tableId->setCell (0, b, Id[0]); //Sets data for the Ids column
	for (int b=5; b<10; b++)  
	    tableId->setCell (0, b, Id[1]);

	for (int b=0; b<10; b++)  // Adds data to the vector that will handle
	    tableId->setCell(1, b, tmpCell[0]);  //Sets data for the 1st 
                                                     //column in the table

	for (int b=0; b<10; b++)             //Fills the vector that will be 
            tableId->setCell (2, b, tmpCell[1]);


	//Now that we have the two tables with the the values in memmory,
	//lets assign an interval of validity to the rows in the table

	//*****************************
	//get since and till times

  //A.V. Make the output from this test reproducible
	//time_t tnow;
	//time (&tnow);	
	//SimpleTime since (tnow);
	SimpleTime since (2004,04,20,17,00,00);

	SimpleTime till;
	till.setPlusInf();
	
	//since and till are set!
	//******************************

	//The first table only contains one row 
	table->setSinceTime(since);
	table->setTillTime(till);

	//The second table contains 10 rows
	for (int b=0; b<10; b++){
	    tableId->setSinceTime(since);
	    tableId->setTillTime(till);
	}
	

	//Let's see how the tables look like
	printTable (table);
	printTable (tableId);

	//Store the tables using the ConditionsDB persistente mechanism
	//Storing tables in the database
	cout << "Storing table" << endl;
	condDataAccess->storeCondDBObject(folder, table);
	
	cout << "Storing tableId" << endl;
	condDataAccess->storeCondDBObject(folder2, tableId);
	
	//Delete table and tableId objects
	CondDBMySQLTableFactory::destroyCondDBTable( table ); //delete table;
	CondDBMySQLTableFactory::destroyCondDBTable( tableId ); //	delete tableId;
	
	CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );
	return 0;
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
