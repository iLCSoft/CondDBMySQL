#include <CDBManager.hxx>
#include <CDBResources.hxx>
#include <HotLinkWaitForAnswer.hxx>   // include/Manager
#include <StartDpInitSysMsg.hxx>      // include/Messages
#include <DpMsgAnswer.hxx>            // include/Messages
#include <DpMsgHotLink.hxx>           // include/Messages
#include <DpHLGroup.hxx>              // include/Basics
#include <DpVCItem.hxx>               // include/Basics
#include <ErrHdl.hxx>                 // include/Basics
#include <ErrClass.hxx>               // include/Basics
#include <signal.h>
#include <iostream>
#include <string>

#include <ICondDBMgr.h>
#include <CondDBMySQLMgrFactory.h>
#include <ICondDBTable.h>
#include "CondDBTable.h"

using namespace std;
// Globals

vector <ICondDBTable*> tables;
vector <string> folders;
string dbini="", parent="";
bool run=false, status=true;


ICondDBMgr *CondDBmgr = 0;

// ---------------------------------------------------------------
// Our callback classes

class ReadDpContainer : public HotLinkWaitForAnswer
{
public:
    void hotLinkCallBack(DpMsgAnswer &answer);
    void hotLinkCallBack (DpHLGroup &group);
};

class GetDpValues : public HotLinkWaitForAnswer
{
public:
    GetDpValues (string folderName, ICondDBTable *Table)
	{
	    folder = folderName;
	    table=Table;
	}
    ~GetDpValues ()
	{
	    delete table;
	}
    void hotLinkCallBack(DpMsgAnswer &answer);
    void hotLinkCallBack (DpHLGroup &group);
private:
    string folder;
    ICondDBTable *table;
};

class ConnectDpContainer : public HotLinkWaitForAnswer
{
public:
    void hotLinkCallBack(DpMsgAnswer &answer);
    void hotLinkCallBack (DpHLGroup &group);
private:
    vector <GetDpValues*> handlers;
    vector <vector<DpIdentifier> > dpIdListVec;
};

class ConnectDpContainerIds : public HotLinkWaitForAnswer
{
public:
    void hotLinkCallBack(DpMsgAnswer &answer);
    void hotLinkCallBack (DpHLGroup &group);
private:
    vector <GetDpValues*> handlers;
    vector <vector<DpIdentifier> > dpIdListVec;
};

class ConnectDBConfig : public HotLinkWaitForAnswer
{
	public:
		void hotLinkCallBack(DpMsgAnswer &answer);
		void hotLinkCallBack(DpHLGroup &group);
};

class ConnectStartStop : public HotLinkWaitForAnswer
{
	public:
		void hotLinkCallBack(DpMsgAnswer &answer);
		void hotLinkCallBack(DpHLGroup &group);
};


class ConnectStatus : public HotLinkWaitForAnswer
{
	public:

		void hotLinkCallBack(DpMsgAnswer &answer);
		void hotLinkCallBack(DpHLGroup &group);
};



class GetParentFolder : public HotLinkWaitForAnswer
{
	public:

		void hotLinkCallBack(DpMsgAnswer &answer);
		void hotLinkCallBack (DpHLGroup &group);
};


void connectDB()
{
	if (CondDBmgr)
		CondDBMySQLMgrFactory::destroyCondDBMgr(CondDBmgr);

	CondDBmgr =CondDBMySQLMgrFactory::createCondDBMgr();
	int end = dbini.rfind(":");
	string dbOut = dbini.substr(0, end);
	std::cout << "Connecting to database: " << dbOut << "... ";
//	std::cout << "Init: " << dbini << endl;
    try {
		if (dbini!="")		
			CondDBmgr->init(dbini);
		else
		{
			std::cout << "Failed" << endl << "No database configuration" << endl;
			std::cout << "Please review database configuration" << endl;
			status = false;
			exit(1);
		}	

		CondDBmgr->startUpdate();
		CondDBmgr->createCondDB();
		CondDBmgr->commit();
    
		CondDBmgr->startRead();
		CondDBmgr->openDatabase();
		CondDBmgr->commit();

		std::cout << "OK" << endl;

	}

	catch (CondDBException &e){

		std::cout << "Failed" << endl;
		std::cout << "Please review database configuration" << endl;		
		std::cout << e.getMessage() << endl;

		DpIdentifier runId;
		string managerDp = string(CDBResources::getConfigDP());
		string runDp = managerDp+ ".store:_original.._value";
		if (Manager::getId(runDp.c_str(), runId))
		{
			BitVar runSet=0;
			Manager::dpSet(runId, runSet);
		}
		return;
	}
}

void store(string folder, ICondDBTable *table) 
{
	try 
	{
		ICondDBDataAccess* condDataAccess = CondDBmgr->getCondDBDataAccess();
		CondDBmgr->startUpdate();
		condDataAccess->storeCondDBObject( folder, table);
		CondDBmgr->commit();
	}
	catch (CondDBException &e)
	{
		std::cout << "Store failed" << endl;
		std::cout << e.getMessage() << endl;
		status=false;
		return;
	}
}



void storeNull()
{
	try
	{
		string val="NULL";
		vector <string> valvec;
		valvec.push_back(val);
		for (unsigned tab=0; tab<tables.size(); tab++)
		{
			vector <CondDBTable::cdb_types> types;
			tables[tab]->getTypes(types);
			for (long col=0; col<tables[tab]->getNumColumns(); col++)
			{
				if (types[col]<CondDBTable::cdbArrayFloat)
					tables[tab]->setCell(col, 0, val);
				else
					tables[tab]->setCell(col, 0, valvec);
			}
			time_t tnow;
			time (&tnow);
			SimpleTime since(tnow);
			tables[tab]->changeSinceTime(0, since);
			store(folders[tab], tables[tab]);
	    }
	}
	catch (CondDBException &e)
	{
		std::cout << "Store null failed" << endl;
		std::cout << e.getMessage() << endl;
		CDBManager::doExit = PVSS_TRUE;
		status = false;
		return;
	}
}

void searchWildCard (string dp, vector<string> &dpNames, vector<DpIdentifier> &dpIds)
{
	dp += "*";
	std::cout << dp.c_str() << endl;
	DpIdentifier *dpIdArr;
	PVSSlong howMany;
	if (Manager::getIdSet(dp.c_str(), dpIdArr, howMany) == PVSS_TRUE)
	{
//	    cout << "Found a set of " << howMany << " datapoints" << endl;
	    if (howMany == 0)
			std::cout << "No datapoints found for" << dp << endl;
		else
		{
			for (long d=0; d<howMany; d++)
			{
				char *name;
				Manager::getName(dpIdArr[d], name);
				dpNames.push_back(name);
				dpIds.push_back(dpIdArr[d]);
			}
		}	
	}
	else
		std::cout << "No datapoint found for" << dp << endl;
}

void getDpProperties(vector <DpIdentifier> dpIdList, vector <string>& names, vector <string>& units, vector <DpElementType>& types)
{
	names.clear();
	units.clear();
	types.clear();
    for (unsigned i=0; i<dpIdList.size(); i++)
    {
		char *name;
		Manager::getName(dpIdList[i], name); // Get DP name
		names.push_back(name);
	
		CharString tmpstr;
		Manager::dpGetUnit(dpIdList[i], tmpstr); // Get DP Unit
		char* unit = tmpstr;
		units.push_back(unit);

		DpElementType type;
		Manager::getElementType(dpIdList[i], type); // Get DP type
		types.push_back(type);
    }
}

void getDpSons(DpIdentifier id, vector <DpIdentifier>& sonList)
{
    unsigned long sons;
    if (Manager::getNumberOfSons(id, sons) == PVSS_TRUE)
	{
//		char *name;
//		Manager::getName (id, name);
//		cout << "Found " << sons << " sons for " << name << endl;
		for (unsigned long num=0; num<sons; num++)
		{
		    DpIdentifier son;
		    if (Manager::getSon(id, num, son) == PVSS_TRUE)
			{
				if (Manager::isLeaf(son) && !Manager::isStruct(son))
				{
					char *name;
					Manager::getName(son, name);
					string dpname = string(name) + ":_online.._value";
//					std::cout << dpname << endl;
					DpIdentifier tmpId;
					if (!Manager::getId (dpname.c_str(), tmpId))
						std::cout << "Failed getting Id for leaf: " << dpname << endl;
					else
					{
//						CharString sonId;
//						Manager::getName (tmpId, name);
//						tmpId.convertToString(sonId);
//						std::cout << "Name: " << name << " Id: " << sonId << endl;
						sonList.push_back(tmpId);
					}
				}
				else
					getDpSons(son, sonList);
			}
			else
				std::cout << "Something went wrong getting sons" << endl;
		}
    }  
}


void getDpSonsId(DpIdentifier id, vector <DpIdentifier>& sonList)
{
    unsigned long sons;
    if (Manager::getNumberOfSons(id, sons) == PVSS_TRUE)
	{
//		char *name;
//		Manager::getName (id, name);
//		cout << "Found " << sons << " sons for " << name << endl;
		DpIdentifier son;
		if (sons==1)
			getDpSonsId(son, sonList);
		else
			for (unsigned long num=0; num<sons; num++)
			{
				if (Manager::getSon(id, num, son) == PVSS_TRUE)
				{
//					char *name;
//					CharString sonId;
//					Manager::getName (son, name);
//					son.convertToString(sonId);
//					std::cout << "Name: " << name << " Id: " << sonId << endl;
					sonList.push_back(son);
				}
				else
					std::cout << "Couldn't get son for Id" << endl;
			}
	}  
	else
		std::cout << "Something went wrong getting sons for Id" << endl;
}

void dpLink(vector<DpIdentifier> &dpIds, vector<string> &dpNames, bool ids)
{
	ICondDBFolderMgr*  condFolderMgr  = CondDBmgr->getCondDBFolderMgr(); 
	CondDBmgr->startUpdate();

	for (unsigned dps=0; dps<dpIds.size(); dps++)
	{
		vector <DpIdentifier> dpIdList;
		vector <string> names, units;
		vector <DpElementType> types;

//		std::cout << dpNames[dps] << endl;

		if (Manager::isStruct(dpIds[dps]))         // STRUCTURE
		{
//			std::cout << "We have a structure " << endl;
			getDpSons (dpIds[dps], dpIdList);
			getDpProperties (dpIdList, names, units, types);
		}
		else if (Manager::isLeaf(dpIds[dps]))           // ELEMENT LEAF
		{
//			std::cout << "We have a leaf" << endl;
			dpIdList.push_back(dpIds[dps]);
			getDpProperties (dpIdList, names, units, types);
		}
		string folder = parent + "/" + dpNames[dps];
//		std::cout << endl << folder << endl;
		DpIdentList dpList;
		ICondDBTable *table=new CondDBTable(names.size());
		table->resize(1);
		for (unsigned c=0; c<names.size(); c++)
		{
			string cname;
			cname = names[c];
			int start, stop;
			stop = cname.rfind(":") - 1 ;
			cname = cname.substr(1,stop);
			start = cname.rfind(".") + 1;
			cname = cname.substr(start);
			if (cname=="")
			{
				cname = names[c];
				cname = cname.substr(1,stop-1);
				start = cname.rfind(":") + 1;
				cname = cname.substr(start);
			}

//			std::cout << "Name:" << cname << endl;

/*			CharString unit;
			Manager::dpGetUnit(dpIdList[c], unit);
			string cunit = string (unit);
			if (cunit!="")
			cname+="-"+cunit;*/
		    table->setName(c, cname);
			CondDBTable::cdb_types ctype;
			switch (types[c]) 
			{
				case 21 : {ctype=CondDBTable::cdbInt; break;}
				case 5  : {ctype=CondDBTable::cdbArrayInt; break;}
				case 22 : {ctype=CondDBTable::cdbDouble; break;}
				case 6  : {ctype=CondDBTable::cdbArrayDouble; break;}
				case 25 : {ctype=CondDBTable::cdbString; break;}
				case 9  : {ctype=CondDBTable::cdbArrayString; break;}
				case 23 : {ctype=CondDBTable::cdbBool; break;}
				case 20 : {ctype=CondDBTable::cdbLongLong; break;}
				case 4  : {ctype=CondDBTable::cdbArrayLongLong; break;}
				default : {std::cout << "Unhandled data type" << endl; status=false;return;}
			}
			table->setType(c,ctype);
//		    cout << names[c] << "  " << units[c] << "  " << types[c] << endl; //This is one table
			dpList.append(dpIdList[c]);
		}
		try
		{
			if (!condFolderMgr->exist(folder))
			{
				if (ids)
					condFolderMgr->createCondDBFolder(folder, table, "", "", true, ICondDBFolder::STRUCTID);
				else
					condFolderMgr->createCondDBFolder(folder, table, "", "", true);
				CondDBmgr->commit();
			}
		}
		catch (CondDBException &e)
		{
			std::cout << "Failed creating folder" << endl;
			std::cout << e.getMessage() << endl;
			status=false;
			return;
		}
		GetDpValues *wait = new GetDpValues (folder, table);
		if (!Manager::dpConnect(dpList, wait))
		{
			std::cout << "Failed (" << dpNames[dps] << ")" << endl;
			status = false;
			return;
		}
//		else
//			std::cout << "Connect OK" << endl;
	}
}


/*

void dpLinkIds(vector<DpIdentifier> &dpIds, vector<string> &dpNames)
{
	ICondDBFolderMgr*  condFolderMgr  = CondDBmgr->getCondDBFolderMgr(); 
	CondDBmgr->startUpdate();

	for (unsigned dps=0; dps<dpIds.size(); dps++)
	{
		vector <DpIdentifier> dpIdList, dpSonIdList;
		vector <string> names, units;
		vector <DpElementType> types;

//		std::cout << dpNames[dps] << endl;

		if (Manager::isStruct(dpIds[dps]))         // STRUCTURE
		{
//			std::cout << "We have a structure " << endl;
			getDpSonsId (dpIds[dps], dpIdList);
			for (unsigned sons = 0; sons <dpIdList.size(); sons++)
			{
				if (Manager::isStruct(dpIdList[sons]))
				{
					getDpSons (dpIdList[sons], dpSonIdList);
//					std::cout << "Sons: " << dpSonIdList.size() << endl;
					getDpProperties (dpSonIdList, names, units, types);
					string folder = parent + "/" + dpNames[dps];
//					std::cout << endl << folder << endl;
					DpIdentList dpList;
					ICondDBTable *table=new CondDBTable(names.size());
					table->resize(1);
					for (unsigned c=0; c<names.size(); c++)
					{
						string cname;
						cname = names[c];
						int		start, stop;
						stop = cname.rfind(":") - 1 ;
						cname = cname.substr(1,stop);
						start = cname.rfind(".") + 1;
						cname = cname.substr(start);
						if (cname=="")
						{
							cname = names[c];
							cname = cname.substr(1,stop-1);
							start = cname.rfind(":") + 1;
							cname = cname.substr(start);
						}
//						std::cout << "Name:" << cname << endl;
	
						CharString unit;
						Manager::dpGetUnit(dpIdList[c], unit);
						string cunit = string (unit);
						if (cunit!="")
						cname+="-"+cunit;

						table->setName(c, cname);
						CondDBTable::cdb_types ctype;
						switch (types[c]) 
						{
							case 21 : {ctype=CondDBTable::cdbInt; break;}
							case 5  : {ctype=CondDBTable::cdbArrayInt; break;}
							case 22 : {ctype=CondDBTable::cdbDouble; break;}
							case 6  : {ctype=CondDBTable::cdbArrayDouble; break;}
							case 25 : {ctype=CondDBTable::cdbString; break;}
							case 9  : {ctype=CondDBTable::cdbArrayString; break;}
							case 23 : {ctype=CondDBTable::cdbBool; break;}
							case 20 : {ctype=CondDBTable::cdbLongLong; break;}
							case 4  : {ctype=CondDBTable::cdbArrayLongLong; break;}
							default : {std::cout << "Unhandled data type" << endl; status=false;return;}
						}
						table->setType(c,ctype);
//						cout << names[c] << "  " << units[c] << "  " << types[c] << endl; //This is one table
//						CharString out;
//						dpSonIdList[c].convertToString(out);
//						std::cout << "Id in list:" << out << endl;
						dpList.append(dpSonIdList[c]);
					}
					dpSonIdList.clear();
					
					try
					{
						if (!condFolderMgr->exist(folder))
						{
							condFolderMgr->createCondDBFolder(folder, table, "", "", true, ICondDBFolder::STRUCTID);
							CondDBmgr->commit();
						}
					}
					catch (CondDBException &e)
					{
						std::cout << "Failed creating folder" << endl;
						std::cout << e.getMessage() << endl;
						status=false;
						return;
					}
					GetDpValues *wait = new GetDpValues (folder, table);
					if (!Manager::dpConnect(dpList, wait))
					{
						std::cout << "Failed (" << dpNames[dps] << ")" << endl;
						status = false;
						return;
					}
//					else
//						std::cout << "Connect OK" << endl;
				}
				else
					std::cout << "Incorrect datapoint structure" << endl;
			}
		}
		else if (Manager::isLeaf(dpIds[dps]))           // ELEMENT LEAF
			std::cout << "Incorrect datapoint structure" << endl;
	}
}
*/

//Read dps from PVSS for which we want to make a dpConnect
void ReadDpContainer::hotLinkCallBack(DpMsgAnswer &answer)
{
    DpIdentifier dpIdConnect, dpTemp;
    AnswerGroup *resposta;
    AnswerItem *iten;
    Variable *valorapon;
    string dpoint, dptypes, dpnames, field;
    DynVar *arrVari;
    TextVar *vari;
    VariableType tipo;
    
    for (resposta = answer.getFirstGroup(); resposta; resposta = answer.getNextGroup() )
	{
		for (iten = resposta->getFirstItem(); iten; iten = resposta->getNextItem())
		{
		    valorapon = iten->getValuePtr();
			arrVari = (DynVar*)valorapon;
			tipo = arrVari->isA();
			if (tipo == 0x00120000 && arrVari->getNumberOfItems()) // Dyn_Text
			{
				CharString init = CDBResources::getDbInit();
				dbini = (string) init;
				connectDB();

				char * tmp = CDBResources::getParentFol();
				string parent = tmp;
				for (vari = (TextVar*)arrVari->getFirst(); vari; vari = (TextVar*)arrVari->getNext())
				{
					string dp = string(vari->formatValue("")); //Read dpname
					vector <DpIdentifier> dpIds;
					vector <string> dpNames;

					if (Manager::getId(dp.c_str(), dpTemp) == PVSS_FALSE) // does the dp exist?
					{
						std::cout << "Datapoint not found. Searching with wildcard" << endl; 
						searchWildCard (dp, dpNames, dpIds);
						std::cout << "End of search with wildcard" << endl;
					}
					else           // It exists so lets use it
					{
						char *name;
						dpIds.push_back(dpTemp);
						Manager::getName(dpTemp, name);
						dpNames.push_back(name); 
					}
					dpLink(dpIds, dpNames, false);
				}
			}
			else
			{
				std::cout << "No datapoints to connect to" << endl;
				status=false; return;
			}
		}
    }
}

void ReadDpContainer::hotLinkCallBack(DpHLGroup &group)
{
}

// Handle the dpConnect for the dpList
void GetDpValues::hotLinkCallBack(DpMsgAnswer &answer)
{
//  cout << "HotLinkCallBack for answer" << endl;
    AnswerGroup *resposta;
    AnswerItem *iten;
    Variable *valorapon, *arrItem;
    vector < vector<string> > value;
    vector <string> valTemp;
    string tmp;
//  cout << folder << ": " << endl;;

	TimeVar timestamp = answer.getOriginTime(); 
	long sec = timestamp.getSeconds();
	int milli = timestamp.getMilli();

    for (resposta = answer.getFirstGroup(); resposta; resposta = answer.getNextGroup())
    {
	int col=0;
	for (iten = resposta->getFirstItem(); iten!=NULL; iten = resposta->getNextItem())
	{
//	    cout << col << endl;
	    valorapon = iten->getValuePtr();
	    if (valorapon==NULL)
	    {
			std::cout << "Failed getting values" << endl;
		status=false; return;
	    }
	    else if (valorapon->isDynVar())
	    {
		//cout << "Array" << endl;
		DynVar *array = (DynVar*)valorapon;
		for (arrItem = array->getFirst(); arrItem; arrItem= array->getNext())
		{
		    tmp = arrItem->formatValue("");
		    valTemp.push_back(tmp);
		}
		table->setCell(col, 0, valTemp);
	    }
	    else
	    {
		//	cout << "Single" << endl;
		tmp = valorapon->formatValue("");
		table->setCell(col, 0, tmp);
	    }
	    value.clear();
	    valTemp.clear();
	    col++;
	}
    }
    value.push_back(valTemp);
	time_t tstamp = sec;
    SimpleTime since(tstamp);
	CondDBKey sincekey = since + milli * 1000000;
	table->setSinceTime(SimpleTime(sincekey));
    SimpleTime till;
    till.setPlusInf();
    table->setTillTime(till);

    if (!table->getNumRows())
    {
		std::cout << "Table inconsistent" << endl;
		status=false; return;
    }
    
    if (run) store(folder, table);
}

//Call handleHotLink
void GetDpValues::hotLinkCallBack(DpHLGroup &group)
{
//	std::cout << "HotLinkCallBack for values (group)" << endl;
//    cout << folder << ": " << endl;
    vector <vector<string> > value = ((CDBManager *) Manager::getManPtr())->handleHotLinkforConnect(group);
    vector <CondDBTable::cdb_types> types;
    table->getTypes(types);

	TimeVar timestamp = group.getOriginTime(); 
	long sec = timestamp.getSeconds();
	int milli = timestamp.getMilli();

    for (unsigned col=0; col<value.size(); col++)
    {
		if (types[col]<CondDBTable::cdbArrayFloat)

			table->setCell(col, 0, value[col][0]);

		else
			table->setCell(col, 0, value[col]);
    }

	time_t tstamp = sec;
    SimpleTime since(tstamp);
	CondDBKey sincekey = since + milli * 1000000;

    table->changeSinceTime(0, SimpleTime(sincekey));
    if (run) store(folder, table);
}

void ConnectDpContainer::hotLinkCallBack(DpMsgAnswer &answer)
{
	DpIdentifier dpIdConnect, dpTemp;
    AnswerGroup *resposta;
    AnswerItem *iten;
    Variable *valorapon;
    string dpoint, dptypes, dpnames, field;
    DynVar *arrVari;
    TextVar *vari;
    VariableType tipo;
  
	std::cout << "Connected to dpContainer" << endl;
    
    for (resposta = answer.getFirstGroup(); resposta; resposta = answer.getNextGroup() )
    {
		for (iten = resposta->getFirstItem(); iten; iten = resposta->getNextItem())
		{
		    valorapon = iten->getValuePtr();
			arrVari = (DynVar*)valorapon;
			tipo = arrVari->isA();
			if (tipo == 0x00120000 && arrVari->getNumberOfItems()) // Dyn_Text
			{
				ICondDBFolderMgr*  condFolderMgr  = CondDBmgr->getCondDBFolderMgr(); 
				CondDBmgr->startUpdate();
				for (vari = (TextVar*)arrVari->getFirst(); vari; vari = (TextVar*)arrVari->getNext())
				{
					string dp = string(vari->formatValue("")); //Read dpname
				    vector <DpIdentifier> dpIds;
					vector <string> dpNames;
				    if (Manager::getId(dp.c_str(), dpTemp) == PVSS_FALSE) // does the dp exist?
				    {
						std::cout << "Datapoint not found. Searching with wildcard" << endl; 
						searchWildCard (dp, dpNames, dpIds);
					}
					else           // It exists so lets use it
					{
						char *name;
						dpIds.push_back(dpTemp);
						Manager::getName(dpTemp, name);
						dpNames.push_back(name); 
					}
					dpLink(dpIds, dpNames, false);
				}
			}
			else
			{
				std::cout << "No datapoints to connect to" << endl;
				return;
			}
		}
    }
}

void ConnectDpContainer::hotLinkCallBack(DpHLGroup &group)
{
	std::cout << "Updating values...";
	storeNull();
	std::cout << "OK" << std::endl;
	folders.clear();
	tables.clear();
    std::cout << "Disconnecting old dps...";
	for (unsigned old=0; old<dpIdListVec.size(); old++)
	{
		DpIdentList oldList;
		for (unsigned l=0; l<dpIdListVec[old].size(); l++)
			oldList.append(dpIdListVec[old][l]);
		Manager::dpDisconnect(oldList, handlers[old]);
	}
	dpIdListVec.clear();
	handlers.clear();
	std::cout << "OK" << endl;
     
	vector <string> dpNames = ((CDBManager *) Manager::getManPtr())->handleHotLinkforContainer(group); //Get new list
	if (!dpNames.size())
	{
		std::cout << "No datapoint names" << std::endl;	
		return;
	}
	vector <DpIdentifier> dpIds;
	for (unsigned n=0; n<dpNames.size(); n++)
	{
		DpIdentifier tmpId;
		if (Manager::getId(dpNames[n].c_str(), tmpId))
			dpIds.push_back(tmpId);
		else
			std::cout << "Couldn't get Id for " << dpNames[n] << std::endl;
	}
	std::cout << "Connecting to new dps...";
	dpLink(dpIds, dpNames, false);
	std::cout << "OK" << endl;
}

void ConnectDpContainerIds::hotLinkCallBack(DpMsgAnswer &answer)
{
	DpIdentifier dpIdConnect, dpTemp;
    AnswerGroup *resposta;
    AnswerItem *iten;
    Variable *valorapon;
    string dpoint, dptypes, dpnames, field;
    DynVar *arrVari;
    TextVar *vari;
    VariableType tipo;
    	
	std::cout << "Connected to dpContainer for Ids" << endl;

    for (resposta = answer.getFirstGroup(); resposta; resposta = answer.getNextGroup() )
    {
		for (iten = resposta->getFirstItem(); iten; iten = resposta->getNextItem())
		{
		    valorapon = iten->getValuePtr();
			arrVari = (DynVar*)valorapon;
			tipo = arrVari->isA();
			if (tipo == 0x00120000 && arrVari->getNumberOfItems()) // Dyn_Text
			{
				ICondDBFolderMgr*  condFolderMgr  = CondDBmgr->getCondDBFolderMgr(); 
				CondDBmgr->startUpdate();
				for (vari = (TextVar*)arrVari->getFirst(); vari; vari = (TextVar*)arrVari->getNext())
				{
					string dp = string(vari->formatValue("")); //Read dpname
				    vector <DpIdentifier> dpIds;
					vector <string> dpNames;
				    if (Manager::getId(dp.c_str(), dpTemp) == PVSS_FALSE) // does the dp exist?
				    {
						std::cout << "Datapoint not found. Searching with wildcard" << endl; 
						searchWildCard (dp, dpNames, dpIds);
					}
					else           // It exists so lets use it
					{
						char *name;
						dpIds.push_back(dpTemp);
						Manager::getName(dpTemp, name);
						dpNames.push_back(name); 
					}
					dpLink(dpIds, dpNames, true);
				}
			}
			else
			{
				std::cout << "No datapoints to connect to for Ids" << endl;
				return;
			}
		}
    }
}

void ConnectDpContainerIds::hotLinkCallBack(DpHLGroup &group)
{
	std::cout << "Updating values for Ids...";
	storeNull();
	std::cout << "OK" << std::endl;
	folders.clear();
	tables.clear();
    std::cout << "Disconnecting old dps for Ids...";
	for (unsigned old=0; old<dpIdListVec.size(); old++)
	{
		DpIdentList oldList;
		for (unsigned l=0; l<dpIdListVec[old].size(); l++)
			oldList.append(dpIdListVec[old][l]);
		Manager::dpDisconnect(oldList, handlers[old]);
	}
	dpIdListVec.clear();
	handlers.clear();
	std::cout << "OK" << endl;
     
	vector <string> dpNames = ((CDBManager *) Manager::getManPtr())->handleHotLinkforContainer(group); //Get new list
	if (!dpNames.size())
	{
		std::cout << "No datapoint names for Ids" << std::endl;	
		return;
	}
	vector <DpIdentifier> dpIds;
	for (unsigned n=0; n<dpNames.size(); n++)
	{
		DpIdentifier tmpId;
		if (Manager::getId(dpNames[n].c_str(), tmpId))
			dpIds.push_back(tmpId);
		else
			std::cout << "Couldn't get Id for " << dpNames[n] << std::endl;
	}
	std::cout << "Connecting to new dps...";
	dpLink(dpIds, dpNames, true);
	std::cout << "OK" << endl;
}


void ConnectDBConfig::hotLinkCallBack(DpMsgAnswer &answer)
{
    AnswerGroup *resposta;
    AnswerItem *iten;
    Variable *valorapon;
    vector<string> tmp;
//	std::cout << "Getting db init string" << endl;
    for (resposta = answer.getFirstGroup(); resposta; resposta = answer.getNextGroup())
    {
	for (iten = resposta->getFirstItem(); iten!=NULL; iten = resposta->getNextItem())
	{
	    valorapon = iten->getValuePtr();
	    if (valorapon==NULL)
	    {
			std::cout << "Failed getting values for db" << endl;
		status=false; return;
	    }
	    else
		tmp.push_back(string(valorapon->formatValue("")));
	}
    }
	
	if (tmp.size())
		dbini = tmp[0];

	for (unsigned db=1; db<tmp.size(); db++)
		if (tmp[db]!="")
			dbini+= ":" + tmp[db];
	connectDB();

}

void ConnectDBConfig::hotLinkCallBack(DpHLGroup &group)
{
	std::cout << "Update db connection" << endl;
    dbini = ((CDBManager *) Manager::getManPtr())->handleHotLinkforConfig(group);

	connectDB();
}

void ConnectStartStop::hotLinkCallBack(DpMsgAnswer &answer)
{
    AnswerGroup *resposta;
    AnswerItem *iten;
    Variable *valorapon;
    string tmp;
    for (resposta = answer.getFirstGroup(); resposta; resposta = answer.getNextGroup())
    {
	for (iten = resposta->getFirstItem(); iten!=NULL; iten = resposta->getNextItem())
	{
	    valorapon = iten->getValuePtr();
	    if (valorapon==NULL)
	    {
			std::cout << "Failed getting value for start" << endl;
			status=false; return;
	    }
	    else
		tmp=valorapon->formatValue("");
	}
    }
    if (tmp=="FALSE")
    {
		std::cout << "Storing OFF" << endl;
		run = false;
    }
    else 
    {
		std::cout << "Storing ON" << endl;
		run = true;
    }
}

void ConnectStartStop::hotLinkCallBack(DpHLGroup &group)
{
    run = ((CDBManager *) Manager::getManPtr())->handleHotLinkforStart(group);
}



void ConnectStatus::hotLinkCallBack(DpMsgAnswer &answer)
{
    AnswerGroup *resposta;
    AnswerItem *iten;
    Variable *valorapon;
    string tmp;

    for (resposta = answer.getFirstGroup(); resposta; resposta = answer.getNextGroup())
    {
		for (iten = resposta->getFirstItem(); iten!=NULL; iten = resposta->getNextItem())
		{
		    valorapon = iten->getValuePtr();
		    if (valorapon==NULL)
		    {
				std::cout << "Failed getting value for start" << endl;
				status=false; return;
		    }
		    else
				tmp=valorapon->formatValue("");
		}
    }
    if (tmp=="FALSE")
		status = false;
//	std::cout << "Status: " << status << endl;
}

void ConnectStatus::hotLinkCallBack(DpHLGroup &group)
{
    status = ((CDBManager *) Manager::getManPtr())->handleHotLinkforStatus(group);
//	std::cout << "Updating status" << endl;
}


void GetParentFolder::hotLinkCallBack(DpMsgAnswer &answer)
{
	AnswerGroup *resposta;
    AnswerItem *iten;
    Variable *valorapon;

	string tmpStr;
	char * tmp = CDBResources::getParentFol();
	parent = tmp;
	for (resposta = answer.getFirstGroup(); resposta; resposta = answer.getNextGroup())
	{
		for (iten = resposta->getFirstItem(); iten!=NULL; iten = resposta->getNextItem())
		{
			valorapon = iten->getValuePtr();
			if (valorapon==NULL)
			{
				std::cout << "Failed getting value for parent folder" << endl;
				status=false; return;
			}
			else
				tmpStr = string(valorapon->formatValue(""));
//				std::cout << "Parent folder: " << parent << endl;
		}
	}
	if (tmpStr!="")
		parent = tmpStr;
}


void GetParentFolder::hotLinkCallBack(DpHLGroup &group)
{
	parent = ((CDBManager *) Manager::getManPtr())->handleHotLinkParent(group);
}

// -------------------------------------------------------------------------
// Our manager class
PVSSboolean CDBManager::doExit = PVSS_FALSE;


// The constructor defines Manager type (API_MAN) and Manager number
CDBManager::CDBManager() 
           : Manager(ManagerIdentifier(API_MAN, Resources::getManNum()))
{
}


// Handle subsequent calls from the connect
vector < vector<string> > CDBManager::handleHotLinkforConnect(const DpHLGroup &group)
{
    vector<string> valTemp;
    vector <vector <string> > value;
    string tmp;

    for (DpVCItem *item = group.getFirstItem(); item; item = group.getNextItem())
    {
		Variable *varPtr = item->getValuePtr();
		if (varPtr) 
		{
			if (varPtr->isDynVar())
			{
				Variable *arrItem;
				DynVar *array = (DynVar*)varPtr;
				for (arrItem = array->getFirst(); arrItem; arrItem= array->getNext())
				{
					tmp=arrItem->formatValue("");
					valTemp.push_back(tmp);
				}
			}
			else
			{
				tmp = varPtr->formatValue("");
				valTemp.push_back(tmp);
			}
		}
		else
		{
			std::cout << "Failed to update values" << endl;
			status=false;
			return value;
		}
		value.push_back(valTemp);
		valTemp.clear();
    }
    return value;
}

vector <string> CDBManager::handleHotLinkforContainer(const DpHLGroup &group)
{
//	std::cout << "Hotlink for container" << endl;
    vector <string> dpNames;
	vector <DpIdentifier> dpIds;
    DynVar *arrVari;
    VariableType tipo;
    for (DpVCItem *item = group.getFirstItem(); item; item = group.getNextItem())
    {
		Variable *valorapon = item->getValuePtr();
		arrVari = (DynVar*)valorapon;
		tipo = arrVari->isA();
		if (tipo == 0x00120000 && arrVari->getNumberOfItems()) // Dyn_Text
		{
		    char * tmp = CDBResources::getParentFol();
		    string parent = tmp;
		    for (TextVar *vari = (TextVar*)arrVari->getFirst(); vari; vari = (TextVar*)arrVari->getNext())
		    {
				string dp = string(vari->formatValue("")); //Read dpname
				DpIdentifier dpTemp;
				if (Manager::getId(dp.c_str(), dpTemp) == PVSS_FALSE) // does the dp exist?
				{
		//			cout << "Datapoint not found. Searching with wildcard" << endl; 
					if (Manager::getId(dp.c_str(), dpTemp) == PVSS_FALSE) // does the dp exist?
				    {
						std::cout << "Datapoint not found. Searching with wildcard" << endl; 
						searchWildCard (dp, dpNames, dpIds);
					}
		//		    cout << "End of search with wildcard" << endl;
				}
				else           // It exists so lets use it
				{
					char *name;
					Manager::getName(dpTemp, name);
					dpNames.push_back(name); 
				}
			}
			return dpNames;
		}
		else
		{
			std::cout << "No datapoints to connect to" << endl;
		}
	}
    return dpNames;
}


string CDBManager::handleHotLinkforConfig (const DpHLGroup &group)
{
    vector<string> tmp;
    for (DpVCItem *item = group.getFirstItem(); item; item = group.getNextItem())
    {
		Variable *varPtr = item->getValuePtr();
		if (varPtr) 
			tmp.push_back(string(varPtr->formatValue("")));
		else
		{
			std::cout << "Failed to update values for config" << endl;
			break;
		}
    }

	string temp;
	if (tmp.size())
		temp = tmp[0];

	for (unsigned db=1; db<tmp.size(); db++)
		if (tmp[db]!="")
			temp+= ":" + tmp[db];

	return temp;    
}

bool CDBManager::handleHotLinkforStart(const DpHLGroup &group)
{
    string tmp;
    for (DpVCItem *item = group.getFirstItem(); item; item = group.getNextItem())
    {
	Variable *varPtr = item->getValuePtr();
	if (varPtr) 
	    tmp=varPtr->formatValue("");
	else
	{
		std::cout << "Failed to update values for config" << endl;
	    status=false; return false;
	}
    }
    if (tmp=="FALSE")
    {
		std::cout << "Storing OFF" << endl;
		return false;
    }
    else 
    {
		std::cout << "Storing ON" << endl;
		return true;
    }
}



bool CDBManager::handleHotLinkforStatus(const DpHLGroup &group)
{
    string tmp;
    for (DpVCItem *item = group.getFirstItem(); item; item = group.getNextItem())
    {
		Variable *varPtr = item->getValuePtr();
		if (varPtr) 
		    tmp=varPtr->formatValue("");
		else
		{
			std::cout << "Failed to update values for status" << endl;
		    status=false; return false;
		}
    }
    if (tmp=="FALSE")
    	return false;
    else 
    	return true;
}

string CDBManager::handleHotLinkParent (const DpHLGroup &group)
{
    string tmp;
    for (DpVCItem *item = group.getFirstItem(); item; item = group.getNextItem())
    {
		Variable *varPtr = item->getValuePtr();
		if (varPtr) 
		    tmp=varPtr->formatValue("");
		else
		{
			std::cout << "Failed to update values for status" << endl;
		    status=false; return false;
		}
    }
	if (tmp!="")
		return tmp;
	else
	{
		return CDBResources::getParentFol();
	}
}


// Run our demo manager
void CDBManager::run()
{
    long sec, usec;
    
    // First connect to Data manager.
    // We want Typecontainer and Identification so we can resolve names
    // This call succeeds or the manager will exit
    connectToData(StartDpInitSysMsg::TYPE_CONTAINER | StartDpInitSysMsg::DP_IDENTIFICATION);
    
    // While we are in STATE_INIT we are initialized by the Data manager
    while (getManagerState() == STATE_INIT)
    {
	// Wait max. 1 second in select to receive next message from data.
	// It won't take that long...
		sec = 1;
		usec = 0;
		dispatch(sec, usec);
    }
    
    // We are now in STATE_ADJUST and can connect to Event manager
    // This call will succeed or the manager will exit
    connectToEvent();
    
    // We are now in STATE_RUNNING. This is a good time to connect 
    // to our Datapoint
    
    BitVar stat = 1;
    DpIdentifier statusId;
    string statusDp= string(CDBResources::getConfigDP()) + ".status:_original.._value";
	
    if (Manager::getId(statusDp.c_str(), statusId))
		dpSet(statusId, stat);

    string dpName = string(CDBResources::getDpName());
    if (dpName!="")
    {
		if (Manager::getId(CDBResources::getDpName(), dpHolder) == PVSS_FALSE)
		{
	    // This name was unknown.
	    // The parameters are in Bascis/ErrClass.hxx
			std::cout << "Wrong datapoint name or datapoint missing !!!!" << endl;
			ErrHdl::error(ErrClass::PRIO_SEVERE,      // It is a severe error
				ErrClass::ERR_PARAM,        // wrong name: blame others
				ErrClass::UNEXPECTEDSTATE,  // fits all
				"CDBManager",               // our file name
				"run",                      // our function name
				CharString("Datapoint ")+CDBResources::getDpName()+
				CharString(" missing"));
		}
		else
		{
			ReadDpContainer *wait = new ReadDpContainer;
			dpGet (dpHolder, wait);  // connect to datapoint which contains the dpnames of the dps we want to store
		}
    }
    else
    {
		DpIdentifier containerId, containerIdForIds, configId, startId, getStatusId, parentId;
		string managerDp = string(CDBResources::getConfigDP());
		string containerDp = managerDp+ ".dpoints:_online.._value";
		string containerIds = managerDp + ".dpointsId:_online.._value";
		string configDp = managerDp + ".config";
		string startDp = managerDp + ".store:_online.._value";
		string parentDp = managerDp+ ".parent:_online.._value";
		string getstatusDp= managerDp + ".status:_online.._value";
	
		if (Manager::getId(configDp.c_str(), configId))
		{
			vector<DpIdentifier> idList;
			DpIdentList dpList;
			if (Manager::isStruct(configId))    
				getDpSons (configId, idList);
			if (idList.size())
			{
				std::cout << "Connecting to config... ";
				for (unsigned pos=0; pos<idList.size(); pos++)
					dpList.append(idList[pos]);
				ConnectDBConfig *wait = new ConnectDBConfig;
				if (dpConnect(dpList, wait))
					std::cout << "OK" << endl;
				else
					std::cout << "Failed" << endl;
				if (Manager::getId(startDp.c_str(), startId))
				{
					std::cout << "Connecting to start... ";
				    ConnectStartStop *wait = new ConnectStartStop;
				    if (dpConnect(startId, wait))
						std::cout << "OK" << endl;
					else
						std::cout << "Failed" << endl;
					if (Manager::getId(parentDp.c_str(), parentId))
					{
						std::cout << "Connecting to parent... ";
						GetParentFolder *waitFolder = new GetParentFolder();
						if (dpConnect(parentId, waitFolder))
							std::cout << "OK" << endl;
						else
							std::cout << "Failed" << endl;	
						if (Manager::getId(containerDp.c_str(), containerId))
						{
							std::cout << "Connecting to container... ";
							ConnectDpContainer *wait = new ConnectDpContainer; 
							if (dpConnect(containerId, wait)) 
								std::cout << "OK" << endl;
							else
								std::cout << "Failed" << endl;
							if (Manager::getId(containerIds.c_str(), containerIdForIds))
							{
								std::cout << "Connecting to container for ids... ";
								ConnectDpContainerIds *wait = new ConnectDpContainerIds; 
								if (dpConnect(containerIdForIds, wait)) 
									std::cout << "OK" << endl;
								else
									std::cout << "Failed" << endl;
								if (Manager::getId(getstatusDp.c_str(), getStatusId))
								{
									std::cout << "Connecting to status... ";
									ConnectStatus *wait = new ConnectStatus; 
									if (dpConnect(getStatusId, wait)) 
										std::cout << "OK" << endl;
									else
										std::cout << "Failed" << endl;
								}
								else
								{
									std::cout << "Datapoint " << getstatusDp.c_str() << " doesn't exist" << endl;
									status=false;
								}
							}
							else
							{
								std::cout << "Datapoint " << containerIds.c_str() << " doesn't exist" << endl;
								status=false;
							}
						}
						else
						{
							std::cout << "Datapoint " << containerDp.c_str() << " doesn't exist" << endl;
							status=false;
						}
					}
					else
					{	
						std::cout << "Datapoint " << parentDp.c_str() << " doesn't exist" << endl;
						status=false;
					}
				}
				else
				{
					std::cout << "No datapoints in " << startDp.c_str() << endl;
					status=false;				
				}	
			}
			else	
		    {
				std::cout << "Datapoint " << configDp.c_str() << " doesn't exist" << endl;
				status=false;
		    }
		}
		else
		{
			std::cout << "Datapoint " << configDp.c_str() << " doesn't exist" << endl;
		    status=false;
		}
	}
    // Now loop until we are finished
    while (1)
    {
	// Exit flag set ?
		if (doExit || !status)
		{
		    stat = 0;
		    if (Manager::getId(statusDp.c_str(), statusId))	
			{
				if(!dpSet(statusId, stat))
					std::cout << "Couldn't set status to NOT running!" << endl;
			}
			else
				std::cout << "Couldn't set status to NOT running!" << endl;

			if (!status && !doExit)
				storeNull();
			if (CondDBmgr)
				CondDBMySQLMgrFactory::destroyCondDBMgr(CondDBmgr);
			std::cout << "Exiting" << endl;
			return;
		}
	// Wait 100 ms
		sec = 0;
		usec = 100000;
		dispatch(sec, usec);
    }
}


// Receive Signals.
// We are interested in SIGINT and SIGTERM. 
void CDBManager::signalHandler(int sig)
{
  if ( (sig == SIGINT) || (sig == SIGTERM) )
  {
	  CDBManager::doExit = PVSS_TRUE;
  }
  else
      Manager::signalHandler(sig);
}
















