
#include "testFolders.h"
#include <vector>

#define DONE char(27) << "[1;32;40m "/*Bold green on black background*/ \
	       << "done!" \
               << char(27) << "[0m" 

int main()
{

    std::vector<std::string> log;

    try
    {
	vector<string> f_list;
	vector<ICondDBFolder::folder_types> t_list;
	f_list.push_back("/layer1");
	f_list.push_back("/struct");
	f_list.push_back("/structID");
	f_list.push_back("/structTAG");
	
	t_list.push_back(ICondDBFolder::BLOBTAG);
	t_list.push_back(ICondDBFolder::STRUCT);
	t_list.push_back(ICondDBFolder::STRUCTID);
	t_list.push_back(ICondDBFolder::STRUCTTAG);
	
	basicSession();
	
	ICondDBMgr* condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
	
	cout << "testFolders - Initializing database" << endl;
	
	condDBmgr->init("atlmysql01.cern.ch:test_conddb:conditions:conditions");
	
	condDBmgr->startUpdate();
	condDBmgr->createCondDB();
	condDBmgr->commit();
	
	condDBmgr->startRead();
	condDBmgr->openDatabase();
	condDBmgr->commit();
	
	//ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
	ICondDBFolderMgr*   condFolderMgr = condDBmgr->getCondDBFolderMgr();
	
	cout << "*** STARTING TESTING THE FOLDERS ***" << endl << endl;
	cout << "Creating a folder for data without structure [BLOB/BLOBTAG]...";
	condDBmgr->startUpdate();
	condFolderMgr->createCondDBFolder(f_list[0], "teste", "<addrHeader><address_header service_type=\"72\" clid=\"94049889\" /></addrHeader><typeName>GenericDbTable</typeName><timeStamp>run-event</timeStamp>", true);
	condDBmgr->commit();
	cout << DONE /*"done!"*/ << endl;
	
	// let's try to create folders for data with structure
	cout << "Creating folders for data with structure [STRUCT/STRUCTID/STRUCTTAG]..." << endl << endl;
	
	for (unsigned int i=1; i<f_list.size(); i++)
	{
	    cout << "\t->Creating a folder with type " << t_list[i];
	    condDBmgr->startUpdate();
	    condFolderMgr->createCondDBFolder(f_list[i], createTable(), "", "<addrHeader><address_header service_type=\"72\" clid=\"94049889\" /></addrHeader><typeName>GenericDbTable</typeName><timeStamp>run-event</timeStamp>", true, t_list[i]);
	    condDBmgr->commit();
	    cout << " ..." << DONE /*done!"*/ << endl;
	}

// let's verify if the folders exist and are of correct type
	cout << "Verifying if all folders exist and are of correct type..." << endl << endl;
	cout << "\t*>Folders that should exist:" << endl;
	for (unsigned int i=0; i<f_list.size(); i++)
	    cout << "\t\t-> " << f_list[i] << endl;
	
	cout << "\t*>Folders in database:" << endl;
	for (unsigned int i=0; i < f_list.size(); i++)
	{
	    cout << "\t\t->Folder \"" << f_list[i];
	    if (condFolderMgr->exist(f_list[i]))
		cout << " exists ";
	    else
		throw("Folder don't exist!!!");
	    
	    if (condFolderMgr->getFolderType(f_list[i])==t_list[i])
		cout << "and is of correct type" << endl;
	    else
		throw("Folder is not of expected type!");
	    
	}

	// let's get all information for each folder

	cout << "\n\nTesting folder retrieval..." << endl << endl;
	
	ICondDBFolder *pFolder;
	
	for (unsigned int i=0; i<f_list.size(); i++) {
	    cout << "\t->Fetching folder " << f_list[i]  << endl;
	    
	    try {
		condDBmgr->startRead();
		condFolderMgr->getCondDBFolder(f_list[i], pFolder);
		condDBmgr->startUpdate(); 
	    
		if (pFolder == NULL)
		    throw("Pointer is NULL something is wrong");
	    }
	    catch (CondDBException &e) {
		cerr << "^[[97;101;1m *** ERROR FECTCHING FOLDER ^[[m" << endl;
		cerr << "^[[97;101;1m *** ConditionsDB exception caught: " << e.getMessage() << "\n"
		     << "***   error code: " << e.getErrorCode() << "^[[m" << endl;
	    }
	    cout << "\n\t->Dumping folder: " << pFolder->getName() << endl << endl;
	    cout << " NAME        : ->[" << pFolder->getFullPathName() << "]" << endl;
	    cout << " ATTRIBUTES  : ->[" << pFolder->getAttributes() << "]" << endl;
	    cout << " DESCRIPTION : ->[" << pFolder->getDescription() << "]" << endl << endl;
	    
	}

	// everything is ok...so far...
	//let's try to create folders that already exist

	cout << "\n\nTesting the creation of a folder that exists already..." << endl << endl;

	try {
	    cout << "Trying to create the folder for data without structure [BLOB/BLOBTAG]...";
	    condDBmgr->startUpdate();
	    condFolderMgr->createCondDBFolder(f_list[0], "", "", true);
	    condDBmgr->commit();
	    cout << DONE /*"done!"*/ << endl;
	}
	
	catch (CondDBException &e){
	    
	    std::string msg;
	    
	    msg = "*** ConditionsDB exception caught: ";
	    msg += e.getMessage();
	    msg += "\n ***   error code: ";
	    msg += e.getErrorCode();
	    
	    log.push_back(msg);
	}
	
	// let's try to create folders for data with structure
	cout << "Trying to create the folders for data with structure [STRUCT/STRUCTID/STRUCTTAG]..." << endl << endl;
	
	for (unsigned int i=1; i<f_list.size(); i++)
	{
		cout << "\t->Creating a folder with type " << t_list[i];
		try {
		    condDBmgr->startUpdate();
		    condFolderMgr->createCondDBFolder(f_list[i], createTable(), "", "", true, t_list[i]);
		    condDBmgr->commit();
		}
		catch (CondDBException &e){
		    std::string msg;
		    
		    msg  = "*** ConditionsDB exception caught: ";
		    msg += e.getMessage();
		    msg += "\n ***   error code: ";
		    msg += e.getErrorCode();
		    
		    log.push_back(msg);
		}
		
		cout << " ..." << DONE /*done!"*/ << endl;
		
	}
	
	
    	
	// let's see how much exceptions we have here...we should have four (one for each folder type)
	cout << "\n\t->The number of exceptions found is " << log.size() << " which is ";
	if (log.size() == 4)
	  cout << char(27) /*escape character*/ 
	       << "[1;32;40m "/*Bold green on black background*/ 
	       << "CORRECT!" 
	       << char(27) << "[0m" /*reset the text to normal*/
	       << endl;
	else
	    cout << char(27) /*escape character*/ 
		 << "[1;31;40m "/*Bold red on black background*/ 
		 << "WRONG!!!" 
		 << char(27) << "[0m" /*reset the text to normal*/
		 << endl;
	
	
	


	// since averything is ok let's try to delete the folders
    
    cout << "testFolders - Proceeding to folder deletion" << endl << endl;
    for (unsigned int i=0; i<f_list.size(); i++)
    {
	cout << "\t-> Deleting folder " << f_list[i] << "...";
	condDBmgr->startUpdate();
	condFolderMgr->deleteFolder(f_list[i]);
	condDBmgr->commit();
	if (!condFolderMgr->exist(f_list[i]))
	  cout << DONE /*"done!"*/ << endl;
	else
	{
	    cout << char(27) /*escape character*/ 
		 << "[1;31;40m "/*Bold red on black background*/ 
		 << "failed!" 
		 << char(27) << "[0m" /*reset the text to normal*/
		 << endl;
	    throw("Could not delete folder "+ f_list[i]);
	}

    }

    
        CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr );
	int error;
        deleteDB(error);
    }

    catch (CondDBException &e)
    {
        cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
             << "***   error code: " << e.getErrorCode() << endl;

    }
}
