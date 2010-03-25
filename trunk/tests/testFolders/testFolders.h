#include <ConditionsDB/ICondDBMgr.h>
#include <ConditionsDB/CondDBMySQLMgrFactory.h>
#include <ConditionsDB/CondDBMySQLObjectFactory.h>
#include <ConditionsDB/CondDBMySQLTableFactory.h>
#include "ConditionsDB/ICondDBTable.h"
//#include "CondDBTable.h"

#include <string>
#include <iostream>
#include <vector>

#include <mysql.h>

using namespace std;

// setting some global variables to define the connection to the database
string mysql_db = "conddb_folderstest";
string mysql_passwd = "pwd4conddb";
string mysql_user = "conddbuser";
string mysql_host = "pcatlas211.cern.ch";

string init_db = mysql_host + ":" + mysql_db + ":" + mysql_user +  ":" + mysql_passwd;

ICondDBTable * createTable()
{
   try {

	//Starting set up of the tables in memory:
	//There will be 2 tables. One using CondDBTable features and
        //the other one using the CondDBTable with ID storage mechanism

	vector <string> colNamesTable;        //for the names of the colums
                                              //in the CondDBTable
	colNamesTable.push_back("Col1");
	colNamesTable.push_back("Col2");
	colNamesTable.push_back("Col3");


	//Creating the table
	ICondDBTable* table= CondDBMySQLTableFactory::createCondDBTable(3);//new CondDBTable(3);     //one table with
                                                     //8 columns

	//Settig up the Names and types of the columns of ICondDBTable* table
	table->setName(0, colNamesTable[0]); table->setType(0, ICondDBTable::cdbFloat);
	table->setName(1, colNamesTable[1]); table->setType(1, ICondDBTable::cdbInt);
	table->setName(2, colNamesTable[2]); table->setType(2, ICondDBTable::cdbDouble);

	//Return the table created
	return table;

    }

    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;
  throw;
    }
}

void basicSession()
{
    try {
	cout << "   ****Initializing Database***" << endl;

        ICondDBMgr* CondDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();

        //CondDBmgr->init();
        CondDBmgr->init(init_db);

        CondDBmgr->startUpdate();
        CondDBmgr->createCondDB();
        CondDBmgr->commit();

        CondDBMySQLMgrFactory::destroyCondDBMgr( CondDBmgr );
    }

    catch (CondDBException &e)
    {
        cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
             << "***   error code: " << e.getErrorCode() << endl;
    }
}

void deleteDB(int &error)
{
    MYSQL       mysql;
    string query_buf = "DROP DATABASE " + mysql_db;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,mysql_host.c_str(),mysql_user.c_str(),mysql_passwd.c_str(),NULL,0,NULL,0))
    {
	cerr << "Couldn't connect to engine!" << mysql_error(&mysql) << endl;
        error = 1; // Return failure
    }
    else
    {
	if (mysql_select_db(&mysql, mysql_db.c_str()))
	{
	    cerr << "Couldn't select database: " << mysql_db
                 << "!" << endl << mysql_error(&mysql);
	}
	else
            {
	        if(mysql_query(&mysql,query_buf.c_str()))
                {
		        cerr << "Query failed " << mysql_error(&mysql) << endl;
	        }
	        else
                {
		        cout << "   ....Database Deleted!" << endl;
	        }
            }
    }
        mysql_close(&mysql);
}

