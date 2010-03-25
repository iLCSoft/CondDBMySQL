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
/******************************************************/
/*         Example data storage on CondDBTables       */
/*               in Conditions Database               */
/*          Using method "on-change"                  */
/*            1st version: 8 March 2004               */
/*           Last updated: 11 March 2003              */
/******************************************************/


#include "cdbStore.h"
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(){

    /*********************************************************/
    /*              Configuration Stuff                      */
    /*              Change the next lines if you want        */
    /*********************************************************/

    //Defines the maximum time (in seconds) between updated
    int maxTimeToUpdate = 3;
    //Defines the number of Ids for the CondDBTable
    int numOfIds = 5;
    //Number of updates into the table
    int numberOfUpdates = 10;
    //Folder name
    std::string folder = "/muon/chambers";
    /*********************************************************/
    /*           End of Configuration Stuff                  */
    /*********************************************************/

    cdbStore * myData = new cdbStore(maxTimeToUpdate, numOfIds);

    //Creating table with 3 columns 
    ICondDBTable* tableId = CondDBMySQLTableFactory::createCondDBTable(3); //new CondDBTable(3);
    
    tableId->setName(0, "Id");                tableId->setType(0, ICondDBTable::cdbInt);
    tableId->setName(1, "Temperature");       tableId->setType(1, ICondDBTable::cdbFloat);
    tableId->setName(2, "IntValue");          tableId->setType(2, ICondDBTable::cdbInt);
    
    //Please uncoment the next line if you want to print the table in the screen. 
    //This can cause a lot of information to be printed
    //myData->printTable(tableId);


    //CondDB initialization
    myData->init();

    //Creates the folder in the Database
    myData->createFolder(folder, tableId);

    //Resizing table for the number of necessary rows = number of Ids
    tableId->resize(numOfIds);

    //*****************************
    //get since and till times
    time_t tnow;
    time (&tnow);
    SimpleTime since (tnow);
    SimpleTime till;
    till.setPlusInf();
    //since and till are set!
    //******************************

    
    //Auxiliary variables to store data that will be stored
    long valueInt;
    float valueFloat;

    //Let's fill the table for the first time - For each ID
    for(long row=0; row < numOfIds; row++){
	//Get the values for the table. The method generateNumber generates an random float number
	valueInt = (int)(myData->generateNumber());  //cast to int because generateNumber() returns a float
	valueFloat = myData->generateNumber();
	
	//cout << "Int: " << valueInt << " Float: " << valueFloat << " Row: "  << row << endl;

	//Set the values to the tables
	//Remember that the CondDBTable indexes start in (0,0)
	tableId->setCell (0, row, row);       //position 0,0 of the table (column number=0, row number=row) 
	tableId->setCell (1, row, valueFloat);       //position 0,0 of the table (column number=1, row number=row) 
	tableId->setCell (2, row, valueInt);   //position 1,0 of the table (column number=2, row number=row)

	//Set since and till times
	tableId->setSinceTime(since);
	tableId->setTillTime(till);
    }
    
    //Store the table in the database
    myData->store(folder, tableId);


    //Auxiliar variable. It makes a counter from 0 to the number of updates defined
    int count =0;

    long nextUpdate;    //Variable to store the time for the next update
    long nextId;        //Variable to store the nextId to be updates
    //For each update...
    while(count < numberOfUpdates){
    
	//Get the update parameters...
	nextUpdate = myData->getNextUpdateTime();   //Time for the next update
	nextId = myData->getIdToUpdate();           //Which ID will be updated

	//Again retrieving the values that will be stored in the Table
	valueInt = (int)(myData->generateNumber());
	valueFloat = myData->generateNumber();
	
	//Some print out stuff to know what's going on...
	cout << "|--------------------------------------------------------------------------------------" << endl;
	cout << "\tTable will be updated. ID to be updated: " << nextId << endl;
	cout << "\t The update will take place in " << nextUpdate << " seconds!" << endl;
	cout << "\t\t Updating Table ID " << nextId << " with Values: " << valueFloat << " and " << valueInt << endl;
	cout << "|--------------------------------------------------------------------------------------" << endl << endl;
	
	//Sleeps the number of seconds until the time for the next update
	sleep(nextUpdate);
	
	//Sets the new values in the tables
	//We can use tha same table. The internals of the API will then take care to update
	//in the database ONLY the values that have been changed
	tableId->setCell (0, nextId, nextId);        //position 0,0 of the table (column number=0, row number=nextId) 
	tableId->setCell (1, nextId, valueFloat);    //position 0,0 of the table (column number=1, row number=nextId) 
	tableId->setCell (2, nextId, valueInt);      //position 1,0 of the table (column number=2, row number=nextId)

	//Get's the actual time in order to change the sinceTime
	time (&tnow);
	SimpleTime since (tnow);

	//Changes the sinceTime
	//Note: when we create a table the setSinceTime function is used. 
	//When we want to do an update we use the changeSinceTime function
	//providing also the number of the row - in this case equal to the number of the ID
	tableId->changeSinceTime(nextId, since);

	//Storing the data in the database
	myData->store(folder, tableId);
	
	count++;

	//Please uncoment the next line if you want to print the table in the screen. 
	//This can cause a lot of information to be printed
	//myData->printTable(tableId);

    }

    //Preventing memory leaks!
    CondDBMySQLTableFactory::destroyCondDBTable( tableId); //delete tableId;
    delete myData;

    cout << "****      Done!      ****" << endl;
    return 0;

}

