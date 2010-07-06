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
/* $Id: performanceTests.cxx,v 1.1 2006-11-15 14:11:36 poeschl Exp $ */

#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/CondDBMySQLMgrFactory.h"
#include "ConditionsDB/CondDBMySQLObjectFactory.h"
#include "ConditionsDB/CondDBMySQLTableFactory.h"
#include "ConditionsDB/ICondDBTable.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <cstdlib>

#include <mysql.h>
#include "sys/time.h"

using namespace std;
//Just change this line if you want to perform your own test

string mysql_db = "dbase";
string mysql_passwd = "tripas";
string mysql_user = "nbarros";
string mysql_host = "kdata5.fis.fc.ul.pt";

string init_db = mysql_host + ":" + mysql_db + ":" + mysql_user +  ":" + mysql_passwd;

int killDB = 1;               //Change this one if you want to keep the databases after the test
                              //Changing it can cause a lot of junk stored in the Database Server
                              //Use it only for performanceTests.cxx program debug 
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
		if (3<types[columnNumber]){           //If the values in 
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


//This one creates the DB structure for each test...
void basicSession(int & error){
    try {
	cout << "   ****Initializing Database***" << endl;

        ICondDBMgr* CondDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();

        CondDBmgr->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");
        //CondDBmgr->init(init_db);

        CondDBmgr->startUpdate();
        CondDBmgr->createCondDB();
        CondDBmgr->commit();

        CondDBMySQLMgrFactory::destroyCondDBMgr( CondDBmgr );
        error = 0; // return success
    }

    catch (CondDBException &e)
    {
        cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
             << "***   error code: " << e.getErrorCode() << endl;

        error = 1;  // return failure
    }
}


float storeDatax(long num_obj, string folder, int &error){
    
    //Measurements stuff...
     struct timeval t_start, t_end;
     long long t;
     float t_seconds;
     gettimeofday(&t_start, 0);

    //modify this line if you want to change the interval of validity of the objects
    int since=5, till=15;
    try {
	cout << "\t Performing -- storeDatax --" << endl;

	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();

    //Database initialization
	condDBmgr->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");

	condDBmgr->startRead();
	condDBmgr->openDatabase();
	condDBmgr->commit();

	ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
	ICondDBFolderMgr*  condFolderMgr  = condDBmgr->getCondDBFolderMgr();

	condDBmgr->startUpdate();

	if (!condFolderMgr->exist(folder))
	{
	    cout << "\t \t CondDB folder does not exist \n \t \t Proceding to cretion of CondDBFolder " << folder << endl;
	    condFolderMgr->createCondDBFolder( folder, "", "", true);
	    condDBmgr->commit();
        }
	else{
	    cout << "\t \t CondDBFolder already exists...proceding!" << endl;
	}

	condDBmgr->startUpdate();
	
	//Store the objects...
	do
	{
            ICondDBObject* condObject = CondDBMySQLObjectFactory::createCondDBObject(since, till,"sample data value \n \n object", "sample description" );

	    condDataAccess->storeCondDBObject( folder,condObject );
	    CondDBMySQLObjectFactory::destroyCondDBObject(condObject);
	    num_obj--;
	}
	while(num_obj != 0);

	condDBmgr->commit();
	CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );
	error = 0; // return success
    }

    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;

	error = 1;  // return failure
    }
    gettimeofday(&t_end, 0);
    t_seconds = (((float)t_end.tv_sec + (float)t_end.tv_usec/1000000) - ((float)t_start.tv_sec + (float)t_start.tv_usec/1000000));

    t=((t_end.tv_sec*1000000 + t_end.tv_usec) - (t_start.tv_sec*1000000 + t_start.tv_usec));
    cout << "\t \t Time to perform the test: - "  << t << " - microsenconds ; " << t_seconds << " seconds" << endl;
    return t_seconds;

}

void printObj(ICondDBObject* condDBobj, int &error)
{
    try{
	//CondDBKey since( condDBobj->validSince() );
	//CondDBKey till( condDBobj->validTill() );
	SimpleTime insertionTime;
	condDBobj->insertionTime(insertionTime);
	//long layerNum( condDBobj->layer() );
	string data = "";
	condDBobj->data(data);
	string description;
	condDBobj->description(description);
	error=0; // return success
    }
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;

	error = 1;  // return failure
    }

}

void printAllObjs(ICondDBDataIterator* iterObjs, int &error)
{
    try{
	ICondDBObject* oCondObject = 0;
	oCondObject = iterObjs->current();
	while ((oCondObject = (iterObjs->next()))){
	    printObj(oCondObject, error);
	    delete oCondObject;
	}
    }
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;
	error = 1;  // return failure
    }
}

float readDataAll(string folder, int &error){
    
    //Measurements stuff...
     struct timeval t_start, t_end;
     long long t;
     float t_seconds;
     gettimeofday(&t_start, 0);

     

    try {
	cout << "\t Performing -- readAll --" << endl;
	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();

	condDBmgr->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");
    
	condDBmgr->startRead();
	condDBmgr->openDatabase();
	condDBmgr->commit();

	ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();

	ICondDBObject* oCondObject = 0;
	ICondDBDataIterator* oCondIt = 0;
	condDBmgr->startRead();

	condDataAccess->browseAllObjects(oCondIt, folder);

	if ( oCondIt != 0 )
	{
	    printAllObjs(oCondIt, error);
	    delete oCondIt;
	    CondDBMySQLObjectFactory::destroyCondDBObject(oCondObject);
	}
	else
	    cout << "No object found " << endl;
	condDBmgr->commit();

	CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );

	error = 0; // return success

    }
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
         << "***   error code: " << e.getErrorCode() << endl;
	error = 1;  // return failure
    }
    gettimeofday(&t_end, 0);
    t_seconds = (((float)t_end.tv_sec + (float)t_end.tv_usec/1000000) - ((float)t_start.tv_sec + (float)t_start.tv_usec/1000000));
    t=((t_end.tv_sec*1000000 + t_end.tv_usec) - (t_start.tv_sec*1000000 + t_start.tv_usec));
    //t=(long long)t_seconds*1000000;
    cout << "\t \t Time to perform the test: - "  << t << " - microsenconds; " << t_seconds  << " seconds" << endl;
    return t_seconds;

}


//Stores objects with diferent intervals of validity
float storeDatay(long num_obj, string folder, int &error){
    //Measurements stuff...
    struct timeval t_start, t_end;
    long long t;
    float t_seconds;
    gettimeofday(&t_start, 0);

    long i = 1;
    try {
	cout << "\t Performing -- storeDatayAll --" << endl;

	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();

	condDBmgr->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");

	condDBmgr->startRead();
	condDBmgr->openDatabase();
	condDBmgr->commit();

	ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
	ICondDBFolderMgr*  condFolderMgr  = condDBmgr->getCondDBFolderMgr();

	condDBmgr->startUpdate();
	if (!condFolderMgr->exist(folder))
	{
	    cout << "\t \t CondDB folder does not exist \n \t \t Proceding to cretion of CondDBFolder " << folder << endl;
	    condFolderMgr->createCondDBFolder( folder, "", "", true);
        // add the system name of an existing DB if you wish
	    condDBmgr->commit();
	}
	else
	    cout << "\t \t CondDBFolder already exists...proceding!" << endl;

	condDBmgr->startUpdate();

	do
	{
	    ICondDBObject* condObject = CondDBMySQLObjectFactory::createCondDBObject(i, i+10,"sample data value \n \n object", "sample description" );

	    condDataAccess->storeCondDBObject( folder,condObject );
	    CondDBMySQLObjectFactory::destroyCondDBObject(condObject);
	    i++;
	}
	while(i < num_obj+1);

	condDBmgr->commit();
	CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );
	error = 0; // return success
    }

    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;

	return 1;  // return failure
    }
    gettimeofday(&t_end, 0);
    t_seconds = (((float)t_end.tv_sec + (float)(t_end.tv_usec)/1000000) - ((float)t_start.tv_sec + (float)t_start.tv_usec/1000000));
    t=((t_end.tv_sec*1000000 + t_end.tv_usec) - (t_start.tv_sec*1000000 + t_start.tv_usec));
    //t=(long long)t_seconds*1000000;
    cout << "\t \t Time to perform the test: - "  << t << " - microsenconds; " << t_seconds  << " seconds" << endl;
    return t_seconds;

}

void deleteDB(int &error){
    MYSQL       mysql;
    string query_buf = "DROP DATABASE test_conddb";// + mysql_db;
    mysql_init(&mysql);	
    if (!mysql_real_connect(&mysql,mysql_host.c_str(),mysql_user.c_str(),mysql_passwd.c_str(),NULL,0,NULL,0)){
	cerr << "Couldn't connect to engine!" << mysql_error(&mysql) << endl;
        error = 1; // Return failure
    }else{
	if (mysql_select_db(&mysql, mysql_db.c_str()))
	{
	    cerr << "Couldn't select database: " << mysql_db << "!" << endl << mysql_error(&mysql);
	    error = 0;
	}
	else{
	    if(mysql_query(&mysql,query_buf.c_str())){
		cerr << "Query failed " << mysql_error(&mysql) << endl;
		error = 1;
	    }
	    else{
		error = 0;
		cout << "   ....Database Deleted!" << endl;
	    }
	}
    }
    mysql_close(&mysql);


}

float storeTable(int testNumber=1, int testType=0, int sensorNumber=0, int numberobjects=0, int arraySize=0, int idSize=0,string addDBName=""){ 

//testNumber: Number of columns in the table, by default = 1 column - from 5 to 8 the columns will be of array type
//testType: 0 for table, for tableId
//sensorNumber: valid if testType=1, it defines the number of IDs
//number of objects: number of values rows stored
//arraySize:  if the fields are array, defines the array size. Valid only if testType=0
//idSize: if testType=1, the size in rows of each diferent Id

    vector <string> type;
    type.push_back("cdbFloat"); type.push_back("cdbInt"); type.push_back("cdbDouble"); type.push_back("cdbString"); 
    type.push_back("cdbArrayFloat"); type.push_back("cdbArrayInt"); type.push_back("cdbArrayDouble"); type.push_back("cdbArrayString"); 

    //Let's store data in forlder /moun/dcs
    string parent = "/muon/dcs";

    cout << "\t Performing -- storeTable --" << endl;

     //Measurements stuff...
    struct timeval t_start, t_end;
    long long t;
    float t_seconds;
    gettimeofday(&t_start, 0);

    try {

	vector <string> colNamesTable;         
	vector <string> colNamesTableId;      
	if(testType==0){
	    if(testNumber>=1 && testNumber < 5){
		colNamesTable.push_back("Temperature");
		if(testNumber>=2){
		    colNamesTable.push_back("Voltage");
		    if(testNumber>=3){
			colNamesTable.push_back("Distance");
			if(testNumber>=4){
			    colNamesTable.push_back("Name");
			}
		    }
		}
	    }
	    if(testNumber>=5){
		colNamesTable.push_back("Temperatures");
		if(testNumber>=6){
		    colNamesTable.push_back("Voltages");
		    if(testNumber>=7){
			colNamesTable.push_back("Distances");
			if(testNumber>=8){
			    colNamesTable.push_back("Names");
			}
		    }
		}
	    }
	}//End of test Type==0
	else{
	    colNamesTableId.push_back("Id");
	    if(testNumber>=1 && testNumber < 5){
		colNamesTableId.push_back("HighVoltage");
		if(testNumber>=2){
		    colNamesTableId.push_back("Current");
		    if(testNumber>=3){
			colNamesTableId.push_back("Distance");
			if(testNumber>=4){
			    colNamesTableId.push_back("Name");
			}
		    }
		}
	    }
	}

	//Creating two CondDBTables
	int createTableColumns=testNumber;
	if(testNumber>=5)
	    createTableColumns=testNumber-4;

	ICondDBTable* table= CondDBMySQLTableFactory::createCondDBTable(createTableColumns); //new CondDBTable(createTableColumns); 
	ICondDBTable* tableId = CondDBMySQLTableFactory::createCondDBTable(createTableColumns+1); //new CondDBTable(createTableColumns+1); 
	
	//printTable(tableId);
	//Settig up the Names and types of the columns of ICondDBTable* table

	if(testType==0){
	    for(int columnNumber=0; columnNumber < createTableColumns ; columnNumber++){
		table->setName(columnNumber, colNamesTable[columnNumber]); 
		if(testNumber>=5)
		    table->setType(columnNumber,  ICondDBTable::cdb_types(columnNumber+4));
		else
		    table->setType(columnNumber,  ICondDBTable::cdb_types(columnNumber+1));
	    }
	}
	else{
	    tableId->setName(0,colNamesTableId[0]); tableId->setType(0,ICondDBTable::cdbInt);
	    for(int columnNumber=1; columnNumber < createTableColumns+1 ; columnNumber++){
		tableId->setName(columnNumber, colNamesTableId[columnNumber]); 
		if(testNumber>=5)
		    exit(1);
		else{
		    tableId->setType(columnNumber, ICondDBTable::cdb_types(columnNumber-1));
		}
	    }
	}

	//vector<string> names_;

	//tableId->getNames(names_); cout << "++++++++++++  " <<  names_.size() << endl;
	//Tables set up finished. Two diferent tables have been created 
        //and its definition it's in memory. 
	//What we have is one table with 8 columns, and another with 3. 
        //For the moment, the 
        //number of lines in each table is zero. 
        //The next setep is to resize each one for the desirable size:

	int myIdSize = 1;
	if(testType==1)
	    myIdSize=idSize*sensorNumber; //each Id will have 5 rows

	//cout << "Resizing Tables" << endl;

	

	//Let's add some data to the tables....
	//The next operations will exemplify how can one add data to 
	//the the in-memory tables
	
 	vector <string> tmpArray, tmpColumn, Id, tmpCell, tmpCellId; //Auxiliar vectors 
	vector <vector <string> > tmpColumnArr;           //to add data into 
	                                                  //the tables

        //tmpArray will handle with arrays to be stored in one table cell
        //tmpColumn will handle the data of column with single values 
        //in each cell tmpColumnArr will handle the data of column in wich 
        //each cell will be an array of values Id will be the vector that 
        //will handle the names of the desirable Ids for the table rows
	
	//The tmpCell vector will handle with single values to be stored
	//With this vector we'll create the columns 
        //to be stored in the database
	
	tmpCell.push_back("1234.567");
	tmpCell.push_back("1234567");
	tmpCell.push_back("Some String"); tmpCell.push_back("98899998898.344567");
	tmpCell.push_back("Block1"); tmpCell.push_back("Block2");
	
	tmpCellId.push_back("1234567"); tmpCellId.push_back("1234.567"); 
	tmpCellId.push_back("Some text into the database");  tmpCellId.push_back("98899998898.344567");
	
	if(testType==1){
	    if(sensorNumber>=1){
		Id.push_back("1"); 
		if(sensorNumber>=2){
		    Id.push_back("2"); 
		    if(sensorNumber>=3){
			Id.push_back("3");
			if(sensorNumber>=4){
			    Id.push_back("4");
			    if(sensorNumber>=5)
				Id.push_back("5");
			}
		    }
		}
	    }
	    cout << "Fiz o set do vector dos IDs (sensores)" << endl;
	}
	//printTable(tableId);

	string folder = parent + "/table";   //A folder for the table
	string folder2 = parent + "/tableId";//Another folder for the tableId

	//The standard CondDB initialization
	ICondDBMgr* condDBmgrTable = CondDBMySQLMgrFactory::createCondDBMgr();
	if(killDB!=1)
	     init_db =mysql_host + ":" + mysql_db + addDBName + ":"  + mysql_user + ":" + mysql_passwd;

	condDBmgrTable->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");
	condDBmgrTable->createCondDB();
	condDBmgrTable->openDatabase();
	condDBmgrTable->commit();
	
	ICondDBDataAccess* condDataAccessTable = condDBmgrTable->getCondDBDataAccess();
	ICondDBFolderMgr*  condFolderMgr  = condDBmgrTable->getCondDBFolderMgr(); 
	
	//creating folder for table if needed:

	//printTable (table);

	if(testType==0){
	    condDBmgrTable->startUpdate();
	    if (!condFolderMgr->exist(folder)){
		cout << "\t \t CondDB folder does not exist \n \t \t Proceding to cretion of CondDBFolder " << folder << endl;
		condFolderMgr->createCondDBFolder( folder, table, "", "", true, ICondDBFolder::STRUCT);	    
		cout << "\t \t Folder created ! " << endl;
	    }
	    else
		cout << "CondDBFolder already exists...proceeding!" << endl;
	}
	//end of folder creation
	
	//Creating folder for tableId if needed:



	if(testType==1){
	    condDBmgrTable->startUpdate();
	    if (!condFolderMgr->exist(folder2)){
		cout << "\t \t CondDB folder does not exist \n \t \t Proceding to cretion of CondDBFolder " << folder2 << endl;
		condFolderMgr->createCondDBFolder( folder2, tableId, "", "", true, ICondDBFolder::STRUCTID);
	    }
	    else
		cout << "CondDBFolder already exists...proceeding!" << endl;
	}


	//End of folder criation


	if(testType==0)
	    table->resize(1);             //Resizes table for one rows
	else{
	    tableId->resize(myIdSize);          //Resizes tableId
	}
 

	//Let's fill in the data into the table
	if(testType==0){
	    for (int c=0; c<createTableColumns; c++)
	    {
		if(testNumber >4){
		    for (int i=1; i<=arraySize; i++){       //This adds 10 diferent values to
			tmpArray.push_back(tmpCell[c]);    //tmpArray vector
			table->setCell (c, 0, tmpArray);
		    }
		}
		else
		    table->setCell (c, 0, tmpCell[c]);
	    
		//Know preventing memory leaks:
		tmpArray.clear();

	    }
	}
	else{//testType for tableWithID
	    
	    //Fill in data into tableId 
	    for(int setIdData_=0; setIdData_<idSize; setIdData_++){
		for(int setIdData=0; setIdData<sensorNumber; setIdData++){
		    tableId->setCell (0, (setIdData_+1)*setIdData, Id[setIdData]);
		}
	    }
	    
	    for(int contColumns=1; contColumns<createTableColumns+1; contColumns++){
		for (int b=0; b<myIdSize; b++)  // Adds data to the vector that will handle
		    tableId->setCell (contColumns, b, tmpCellId[createTableColumns-1]);  //the data for the 1st column 
	    }

	}

	//Now that we have the two tables with the the values in memmory,
	//lets assign an interval of validity to the rows in the table

	//*****************************
	//get since and till times
	time_t tnow;
	time (&tnow);
	
	SimpleTime since (tnow);
	SimpleTime till;
	till.setPlusInf();
	
	//since and till are set!
	//******************************

	if(testType==0){
	    //The first table only contains one row 
	    table->setSinceTime(since);
	    table->setTillTime(till);
	}
	else{
	
	    //The second table contains more than one row
	    for (int b=0; b<myIdSize; b++){
		tableId->setSinceTime(since);
		tableId->setTillTime(till);
	    }
	}
	


	//Storing tables in the database
	 if(testType==0){
	     
	     for(int my=0; my< numberobjects ; my++){
		 //sleep(1);
		 SimpleTime till_n (tnow + my*10);
		 
		 SimpleTime since_n (tnow + (my*10 -10));
	    
		 table->changeSinceTime(0,since_n);
		 table->changeTillTime(0,till_n);
		 condDBmgrTable->startUpdate();
		 condDataAccessTable->storeCondDBObject(folder, table);
	     }
	     
	 }
	 else{

	     SimpleTime till_n (tnow + 10);
	     SimpleTime since_n (tnow -10);
	     for (int b=0; b<myIdSize; b++){
		 tableId->changeSinceTime(b,since_n);
		 tableId->changeTillTime(b,till_n);
	     }
	     
	     //printTable (tableId);
	     condDBmgrTable->startUpdate();
 	     condDataAccessTable->storeCondDBObject(folder2, tableId);
	 }
	
	
	//Delete table and tableId objects
	
	CondDBMySQLTableFactory::destroyCondDBTable( table ); //delete table;
	CondDBMySQLTableFactory::destroyCondDBTable( tableId ); //delete tableId;

	condDBmgrTable->commit();	
	//delete condFolderMgr;
	CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgrTable );
    }
    
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;
	
    }
    gettimeofday(&t_end, 0);
    t_seconds = (((float)t_end.tv_sec + (float)(t_end.tv_usec)/1000000) - ((float)t_start.tv_sec + (float)t_start.tv_usec/1000000));
    t=((t_end.tv_sec*1000000 + t_end.tv_usec) - (t_start.tv_sec*1000000 + t_start.tv_usec));
    //t=(long long)t_seconds*1000000;
    cout << "\t \t Time to perform the test: - "  << t << " - microsenconds; " << t_seconds  << " seconds" << endl;
    return t_seconds;
}



float readTable(string type="", string addDBName=""){

    
    //Chose a folder to retrieve data
    string parent = "/muon/dcs";

    cout << "\t Performing -- readTable " << type << "--" << endl;

        //Measurements stuff...
    struct timeval t_start, t_end;
    long long t;
    float t_seconds;
    gettimeofday(&t_start, 0);

    try {
	
	//Conditions Database initialization stuff
	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
        if(killDB!=1)
		init_db =mysql_host + ":" + mysql_db + addDBName + ":"  + mysql_user + ":" + mysql_passwd;
	condDBmgr->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");
	
	condDBmgr->startRead();
	condDBmgr->openDatabase();
	condDBmgr->commit();

	ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
	
	condDBmgr->startRead();
  
	//create a new object to get a new table from the Database
	ICondDBTable *table = CondDBMySQLTableFactory::createCondDBTable(); //new CondDBTable();

	//Set up of a time interval...
	SimpleTime sinces=SimpleTime (2002,10,15,0,0,0);
	SimpleTime tills=SimpleTime (2005,1,1,0,0,0);
	CondDBKey since=sinces, till=tills;

	string name = "table" + type;
	string folder = parent + '/' + name;

	//Retrive the table from  "2002,10,15,0,0,0" "2005,1,1,0,0,0" - all the objects
	//using method browseHistory
	condDataAccess->browseHistory(folder, table, since, till);

	cout << "Printing CondDBTable resulting from browseHistory(folder, table, since, till)" << endl;
  	//printTable(table);

	//The table is not needed any more... Let's delete it
	CondDBMySQLTableFactory::destroyCondDBTable( table ); //delete table;

	condDBmgr->commit();

	CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );

    }
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;
	
    }     
    gettimeofday(&t_end, 0);
    t_seconds = (((float)t_end.tv_sec + (float)(t_end.tv_usec)/1000000) - ((float)t_start.tv_sec + (float)t_start.tv_usec/1000000));
    t=((t_end.tv_sec*1000000 + t_end.tv_usec) - (t_start.tv_sec*1000000 + t_start.tv_usec));
    //t=(long long)t_seconds*1000000;
    cout << "\t \t Time to perform the test: - "  << t << " - microsenconds; " << t_seconds  << " seconds" << endl;
    return t_seconds;


}

int main ()
{
    FILE * fp;
    fp = fopen("./testOutput.txt","wr");
    //Variable to control the all process
    int error = 0;

    //****************************************************************
    //Edit the following lines if you want to 
    //set up your own performance test

    //Tests to the BLOB interface:
    long n_obj[5]={10,100,1000,10000,50000};
    int num_tests = 5;           //Nº of tests to perform = number of values in the array n_obj

    //Tests to the CondDBTable interface:
    int numOfColumns[8] = {1,2,3,4,5,6,7,8};              //Array to the maximum number of columns
    int numOfObjects[5] = {10,100,1000,10000,100000};     //Array for the number of objects

    int useColumns = 8;                //Set this to 1,2,3,4,5,6,7 or 8 - index to the numOfColumns array
    int useObjects = 5;                //Set this to 1,2,3,4 or 5       - index to the numOfObjects

    //Tests to the CondDBTable ID interface:
    int numOfSensors[5] = {1,2,3,4,5};                    //Array for the number of IDs that will be stored
    int idSize[5] = {10,100,1000,10000,100000};                       //Array for the size of each ID

    int useColumnsId = 4;               //Set this to 1,2,3 or 4 - index o the numOfColumns array
    int useSensors = 5;                 //Set this to 1,2,3,4 or 5 - index o the numOfSensors array
    int useIds = 5;                     //Set this to 1,2,3,4 or 5 - ndex o the idSize array
 
    //A simple counter
    int count = 0;

    //To store the execution time (in seconds) for each test
    float time_to_execute;

    //Make sure that we drop the Database
    //This may cause an error if it doesn't
    deleteDB(error);
    
    
    //The folder in wich the tests will be performed
    string folder="/cal/temp";

    //Lets performe each test
    for (count = 0; count < num_tests; count++){
	cout << endl << endl << "Initializing test for - " << n_obj[count] << " - objects" << endl ;
	fprintf(fp, "Entering test for %ld objects \n", n_obj[count]);
	basicSession(error);
	if (error==0){
	    fprintf(fp, "StoreDatax\t ReadAll\t StoreDatay\t ReadAll\t \n");
	    time_to_execute=storeDatax(n_obj[count], folder, error);
	    if (error==0){
		fprintf(fp, "%f", time_to_execute);
		time_to_execute=readDataAll(folder, error);
		if (error==0){
		    fprintf(fp, "\t%f", time_to_execute);
		    deleteDB(error);
		    basicSession(error);
		    if(error==0){
			time_to_execute=storeDatay(n_obj[count], folder, error);
			if (error==0){
			    fprintf(fp, "\t%f", time_to_execute);
			    time_to_execute=readDataAll(folder, error);
			    if (error==0){
				fprintf(fp, "\t%f\n", time_to_execute);
			    }
			    else{
				cout << "Error in test ReadAll." << "Going out..." << endl;			
				return 0;
			    }
			}
			else{
			    cout << "Error in test StoreDatay." << "Going out..." << endl;
			    return 0;
			}
		    }
		    else{
			cout << "Error in Basic Session." << "Going out..." << endl;
			return 0;
		    }
		}///////
		else{
		    cout << "Error in test ReadAll." << "Going out..." << endl;
		    return 0;
		}
		    
	    }
	    else{
		cout << "Error in test storeDatax." << "Going out..." << endl;
		return 0;
	    
	    }
	}
	else{
	    cout << "Error in test basicSession." << "Going out..." << endl;
	    return 0;
	}
	deleteDB(error);

	}
    //Let's have some tests to the CondDBTable facilities!

    deleteDB(error);                 //Cleaning up the database
    
    
    //First we'll iterate in the table without ID
    
    fprintf(fp,	"\nTests for CondDBTable\n");

    char db_name[64];

    for(int myCount = 0; myCount < useColumns; myCount++){
	cout << endl << endl << "Initializing TABLE test for - " << numOfColumns[myCount] << " - Columns" << endl ;
	for(int myOtherCount = 0; myOtherCount < useObjects; myOtherCount++){
	    cout << "\t Entering test to storeTable for " << numOfColumns[myCount] << " columns " << numOfObjects[myOtherCount] << " objects" << endl;
	    fprintf(fp, "Entering test for %d columns %d objects \n", numOfColumns[myCount],numOfObjects[myOtherCount]);

	    sprintf(db_name,"_%d_%d",numOfColumns[myCount], numOfObjects[myOtherCount]);
	    string addDBName = (string)db_name;

	    time_to_execute=storeTable(numOfColumns[myCount],0,0,numOfObjects[myOtherCount],20,0,addDBName);

	    fprintf(fp,"StoreTable \t BrowseHistory \n");
	    fprintf(fp, "%f ", time_to_execute);
	    cout << "\t Entering test to readTable for " << numOfColumns[myCount] << " columns " << numOfObjects[myOtherCount] << " objects" << endl;
	    //fprintf(fp, "Entering test to read (Browse History) for %d columns %d objects \n", numOfColumns[myCount],numOfObjects[myOtherCount]);
	    time_to_execute=readTable("",addDBName);
	    fprintf(fp, "\t%f \n", time_to_execute);
	    deleteDB(error);

	}
    }
    

    deleteDB(error);
    //Iterate in table withID

    useColumns = useColumnsId;

    fprintf(fp, "\nTests for CondDBTable with ID\n");

    for(int myCount = 0; myCount < useColumns; myCount++){
	cout << endl << endl << "Initializing TABLE with ID test for - " << numOfColumns[myCount] << " - Columns" << endl ;
	for(int myOtherCount = 0; myOtherCount < useSensors; myOtherCount++){
	    for(int myThirdCount = 0; myThirdCount < useIds; myThirdCount++){
		cout << "\t Entering test to storeTableID for " << numOfColumns[myCount] << " columns " << numOfSensors[myOtherCount] << " Sensors with " << idSize[myThirdCount] << " rows each one" << endl;
		fprintf(fp, "Entering test for %d columns %d sensors with %d rows each one\n", numOfColumns[myCount],numOfSensors[myOtherCount], idSize[myThirdCount]);
		sprintf(db_name,"_%d_%d_%d",numOfColumns[myCount], numOfSensors[myOtherCount],idSize[myThirdCount]);
                string addDBName = (string)db_name;	
		time_to_execute=storeTable(numOfColumns[myCount],1,numOfSensors[myOtherCount],1,1,idSize[myThirdCount], addDBName);
		fprintf(fp,"StoreTableID \t BrowseHistory \n");
		fprintf(fp, "%f ", time_to_execute);
		cout << "\t Entering test to readTableID for " <<  numOfColumns[myCount] << " columns " << numOfSensors[myOtherCount] << " Sensors with " << idSize[myThirdCount] << " rows each one" << endl;
		//fprintf(fp, "Entering test to read (Browse History) for %d columns %d objects \n", numOfColumns[myCount],numOfObjects[myOtherCount]);
		time_to_execute=readTable("ID",addDBName);
		fprintf(fp, "\t%f \n", time_to_execute);
		deleteDB(error);
		
	    }
	}
    }
    
    return 0;
}

