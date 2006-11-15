///////////////////////////////////////////////////////////////////////
///           cdbStore.cxx                                          ///
///                                                                 ///
///               Luis Pedro 2004                                   ///
///     Implementation of the methods to access                     ///
///            the database methods                                 ///
///            1st version: 8 March 2004                            ///
///           Last updated: 11 March 2003                           ///
///                                                                 ///
///////////////////////////////////////////////////////////////////////

#include "cdbStore.h"


cdbStore::cdbStore(int maxTimeToUpdate, int ids){
    m_maxTimeToUpdate = maxTimeToUpdate;
    m_numIds = ids;
    srand(time(NULL));

}

cdbStore::~cdbStore(){
    condDBmgr->commit();
    CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );
}

void cdbStore::init(std::string profile){
//The standard CondDB initialization
    try {

	condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
	
	if(profile=="")
	    condDBmgr->init();
	else
	    condDBmgr->init(profile);
	
	condDBmgr->createCondDB();
	
	condDBmgr->openDatabase();
	
	condDataAccess = condDBmgr->getCondDBDataAccess();
	
	condFolderMgr  = condDBmgr->getCondDBFolderMgr(); 
    }
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;
	
    }
}


void cdbStore::createFolder(std::string folder, ICondDBTable * table){
    
//creating folder for table if needed:
    try{

	if (!condFolderMgr->exist(folder)){
	    cout << "CondDB folder does not exist \nProceeding to creation of CondDBFolder " << folder << endl;
	    condFolderMgr->createCondDBFolder( folder, table, "", "", true, ICondDBFolder::STRUCTID);
	}
	else
	    cout << "CondDBFolder already exists...proceeding!" << endl;
	 }
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;
	
    }
}


void cdbStore::store(std::string folder, ICondDBTable* table){
    
    condDataAccess->storeCondDBObject(folder, table);

}



int cdbStore::getNextUpdateTime  (){
    m_timeToNextUpdate = m_maxTimeToUpdate + 2;
    while(m_timeToNextUpdate > m_maxTimeToUpdate)
	setNextUpdateTime();
    return m_timeToNextUpdate;
}


int cdbStore::getIdToUpdate(){
    m_idToUpdate = m_numIds + 2;
    while(m_idToUpdate > m_numIds)
	setNextIdToUpdate();
    return  m_idToUpdate;
}

double cdbStore::generateNumber(){
    float rnd; //Variable to handle wiht the random number     

    return rnd = (double)rand()/RAND_MAX*100;
    //printf("aaaaa %f\n",rnd);
}

void cdbStore::setNextUpdateTime (){
    
    m_timeToNextUpdate = (int)generateNumber() + 1;

}

void cdbStore::setNextIdToUpdate (){

    m_idToUpdate = (int)generateNumber();


}

void cdbStore::printTable(ICondDBTable *table){
    
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

