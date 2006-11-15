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
/* $Id: cdbadmin.cxx,v 1.1 2006-11-15 14:15:25 poeschl Exp $ */

// $HEAD 10
//
// File: "cdbadmin.cxx"
//
// Created at Mon Aug 26 10:57:14 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//



#include <ICondDBMgr.h>
#include <CondDBMySQLMgrFactory.h>
#include <CondDBObjFactory.h>
#include <SimpleTime.h>
#include <string>
#include <iostream>

using namespace std;
extern "C" {
#include <unistd.h>
#include <limits.h>
}

// Why limits.h doesn't define LLONG_MAX 
#define LLONG_MAX     9223372036854775807LL

#define USAGE_STR ""\
"Usage:"\
"   cdbadmin [options] <command> <args...>"\
""\
"Available commands:"\
"  create:  create a new item."\
"  list:    list item names."\
"  get:     get an item."\
"  tag:     manage tags. For example, tag a folder."\
""\
"where items can be folders, foldersets, tags or objects."\
""\
"Available options:"\
"  -s=initstr: specify the initialization string"\
"  -d or -t:   output points in date/time format."\
"  -f:         output points in double precision floating point format."\
"  -l:         output points in integer decimal format with int64 range."\
"  -i:         same as -l but truncated to fit in the range of int32."


#define LIST_HELP ""\
"Usage of \'list\' command:"\
""\
"First form, listing all folders, foldersets or tags:"\
"  cdbadmin list folders|foldersets|tags"\
""\
"Listing (i.e. iterating) objects:"\
"  cdbadmin list objects v|h|a <folderName> [tag|point]"\
""\
"where"\
"  v = verticaly, h = horizontaly, a = all "\
""\
"Listing tagged folders"\
"  cdbadmin list tagged <tagName>"

#define GET_HELP ""\
"Usage of \'get\' command:"\
"   cdbadmin get folder|folderset|tag|object"\
""\
"   *** NOT YET IMPLEMENTED ***"

#define CREATE_HELP ""\
"Usage of \'create\' command:"\
"  cdbadmin create folder|fset <folderName> [Attributes]"\
""\
"  cdbadmin create tag <tagName> [Description]"\
""\
"  cdbadmin create object <folderName> <t1> <t2> <Description> <ascii data>"

#define DELETE_HELP ""\
"Usage of \'delete\' command:"\
"  cdbadmin delete folder|fset|tag <name>"


#define TAG_HELP ""\
"Usage of \' tag\' command:"\
"Tag folder syntax"\
"  cdbadmin tag folder <folderName> <tagName>"\
""\
"Rename tag syntax:"\
"  cdbadmin tag rename <tagName> <newName>"\
""\
"Delete tag syntax:"\
"  cdbadmin tag delete <tagName>"

#define NOT_YET_IMPLEMENTED "cdbadmin: sorry, this query is not yet implemented!"


class Syntax {
public:
    Syntax(string str) : message(str) {}
    ~Syntax() {}
    string& getMessage() { return message; }
private:
    string message;
};

ICondDBMgr* condDBmgr;
string initstring("");

// Init and close connections
void initConnection()                                      throw(CondDBException);
void closeConnection();
// Create functions
void createFolder(const string& pathname,
		  const string& attributes,
		  const string& description);
void createFolderSet(const string& pathname,
		     const string& attributes,
		     const string& description);
void createTag(const string& tname, const string& tdesc);
// Tag functions
void tagFolder(const string& fpath, const string& tname);
void tagDelete(const string& tag);
void tagRename(const string& tag, const string& newtag)    throw(CondDBException);
void printTaggedFolders(const string& tag)                 throw(CondDBException);
// Print (list) functions
void printObj(ICondDBObject* condDBobj);
void printAllObjs(ICondDBDataIterator* iterObjs);
void printFolders()                                        throw(CondDBException);
void printFolderSets()                                     throw(CondDBException);
void printTags()                                           throw(CondDBException);
// Iterator (list) functions
void iterObjectsV(const string& folder, CondDBKey point)   throw(CondDBException);
void iterObjectsH(const string& folder, const string& tag) throw(CondDBException);
void iterObjectsA(const string& folder)                    throw(CondDBException);


enum format_t { c_llong=0, c_double, c_date, c_integer } ;

static format_t format = c_llong;


int main ( int argc, char* argv[] )
{
    try {
	int option, idx = 0, count = argc;
	string progname = argv[0];
	while((option = getopt(argc, argv, "idtfls:")) != -1) {
	    switch (option) {
		case 's': // provide the initstring as defined by the specific CondDB API
		    initstring = optarg; break;
		case 'd': // output points date/time format
		case 't': // same as 'd'
		    format = c_date; break;
		case 'f': // outputs points in double floating point format
		    format = c_double; break;
		case 'l': // outputs points in long long format
		    format = c_llong; break;
		case 'i': // outputs points in long (truncated) format. Use at your own risk!
		    format = c_integer; break;
		default:
		    throw(Syntax(USAGE_STR));
	    }
	    idx++;
	    count--;
	}
	if (count<2) throw(Syntax(USAGE_STR));

	initConnection();

	string command = argv[idx+1];

	if (command == "list") {
	    if (count<3) throw(Syntax(LIST_HELP));
	    string arg = argv[idx+2];
	    if (arg == "folders")
		printFolders();
	    else if (arg == "fsets")
		printFolderSets();
	    else if (arg == "tags")
		printTags();
	    else if (arg == "tagged") { // list tagged folders
		if (count<4) throw(Syntax(LIST_HELP));
		string tag(argv[idx+3]);
		printTaggedFolders(tag);
	    }
	    else if (arg == "objects") {
		if (count<5) throw(Syntax(LIST_HELP));
		char mode = *argv[idx+3];
		string tag, folder = argv[idx+4];
		CondDBKey point;
		switch (mode) {
		    case 'a':
			iterObjectsA(folder);
			break;
		    case 'h':
			if (count<6) throw(Syntax(LIST_HELP));
			tag = argv[idx+5];
			iterObjectsH(folder, tag);
			break;
		    case 'v':
			if (count<6) throw(Syntax(LIST_HELP));
			point = atoll(argv[idx+5]);
			iterObjectsV(folder, point);
			break;
		    default:
			throw(Syntax(LIST_HELP));
		}
	    }
	}
	else if (command == "create") {
	    if (count<3) throw(Syntax(CREATE_HELP));
	    string arg = argv[idx+2];
	    if (arg == "folder") {
	    	if (count<4) throw(Syntax(CREATE_HELP));
		string fpath(argv[idx+3]);
		if (count>4)
		    createFolder(fpath, "", string(argv[idx+4]));
		else
		    createFolder(fpath, "", "");
	    }
	    else if (arg == "fset") {
	    	if (count<4) throw(Syntax(CREATE_HELP));
		string fpath(argv[idx+3]);
		if (count>4)
		    createFolderSet(fpath, "", string(argv[idx+4]));
		else
		    createFolderSet(fpath, "", "");		    
	    }
	    else if (arg == "tag") {
	    	if (count<4) throw(Syntax(CREATE_HELP));
		string tname(argv[idx+3]);
		if (count>4)
		    createTag(tname, string(argv[idx+4]));
		else
		    createTag(tname, "");
	    }
	    else if (arg == "object") {
		throw(Syntax(NOT_YET_IMPLEMENTED));
	    }
	}
	else if (command == "get") {
	    throw(Syntax(NOT_YET_IMPLEMENTED));
	}
	else if (command == "tag") {
	    if (count<4) throw(Syntax(TAG_HELP));
	    string arg = argv[idx+2];
	    string tag = argv[idx+3];
	    if (arg == "folder") {
		if (count<5) throw(Syntax(TAG_HELP));
		string fpath = argv[idx+4];
		tagFolder(fpath, tag);
	    }
	    else if (arg == "rename") {
		if (count<5) throw(Syntax(TAG_HELP));
		string newtag = argv[idx+4];
		tagRename(tag, newtag);
	    }
	    else {
		throw(Syntax(TAG_HELP));
	    }
	}
	else if (command == "delete") {
	    if (count<3) throw(Syntax(DELETE_HELP));
	    string arg1 = argv[idx+2];
	    string arg2 = argv[idx+3];
	    if (arg1 == "tag") {
		tagDelete(arg2);
	    }
	    else if (arg1 == "folder") {
	    }
	    else if (arg1 == "fset") {
	    }
	}
	else {
	    throw(Syntax(USAGE_STR));
	}

	closeConnection();
    }
    catch (CondDBException &e)
    {
	cerr << "*** ConditionsDB exception caught: " << e.getMessage() << "\n"
	     << "***   error code: " << e.getErrorCode() << endl;
	return 1;  // return failure
    }
    catch (Syntax &e)
    {
	cerr << e.getMessage();
	return 1;
    }
    return 0;
}


void initConnection()
    throw(CondDBException)
{
    cout << "# DB initialization [" << initstring << "]" << endl;    
    condDBmgr = CondDBMySQLMgrFactory::createCondDBMgr();
    condDBmgr->init(initstring);                        	
    condDBmgr->startRead();
    condDBmgr->openDatabase();
    condDBmgr->commit();
}

void closeConnection()
{
    CondDBMySQLMgrFactory::destroyCondDBMgr( condDBmgr ); 
}

void createFolder(const string& pathname, 
		  const string& attributes,
		  const string& description)
{
    condDBmgr->startUpdate();
    ICondDBFolderMgr* folderMgr = condDBmgr->getCondDBFolderMgr();
    folderMgr->createCondDBFolder(pathname);
    condDBmgr->commit();
}

void createFolderSet(const string& pathname,
		     const string& attributes,
		     const string& description)
{
    condDBmgr->startUpdate();
    ICondDBFolderMgr* folderMgr = condDBmgr->getCondDBFolderMgr();
    folderMgr->createCondDBFolderSet(pathname);
    condDBmgr->commit();
}

void createTag(const string& tname, const string& tdesc)
{
    condDBmgr->startUpdate();
    ICondDBTagMgr* tagMgr = condDBmgr->getCondDBTagMgr();
    tagMgr->createCondDBTag(tname, tdesc);
    condDBmgr->commit();
}

// Tag functions
void tagFolder(const string& fpath, const string& tname)
{
    condDBmgr->startUpdate();
    ICondDBTagMgr* tagMgr = condDBmgr->getCondDBTagMgr();
    tagMgr->tag(fpath, tname);
    condDBmgr->commit();
}

void tagDelete(const string& tag)
{
    condDBmgr->startUpdate();
    ICondDBTagMgr* tagMgr = condDBmgr->getCondDBTagMgr();
    tagMgr->deleteCondDBTag(tag);
    condDBmgr->commit();
}

void tagRename(const string& tagname, const string& newtag)
    throw(CondDBException)
{
    ICondDBTag *tagPtr;
    condDBmgr->startUpdate();
    ICondDBTagMgr* tagMgr = condDBmgr->getCondDBTagMgr();
    tagMgr->getCondDBTag(tagname, tagPtr);
    if (tagPtr==0) throw(CondDBException("tagPtr==NULL",0));
    tagPtr->changeName(newtag);
    condDBmgr->commit();
}

void printTaggedFolders(const string& tagname)
    throw(CondDBException)
{
    ICondDBTag *tagPtr;
    condDBmgr->startRead();
    ICondDBTagMgr* tagMgr = condDBmgr->getCondDBTagMgr();
    cout << "# All folders tagged with: " << tagname << endl << endl;
    vector<string> taggedFolders;
    vector<SimpleTime> insTimes;
    tagMgr->getCondDBTag(tagname, tagPtr);
    if (tagPtr==0) throw(CondDBException("tagPtr==NULL",0));
    tagPtr->getTaggedFolder(taggedFolders, insTimes);
    condDBmgr->commit();
    for (unsigned int i = 0; i < taggedFolders.size(); i++)
	cout << "tagged \"" << taggedFolders[i] << "\" \"" << insTimes[i] << "\"" << endl;
}

// Print (list) functions
void printFolders()
    throw(CondDBException)
{
    condDBmgr->startRead();
    ICondDBFolderMgr* folderMgr = condDBmgr->getCondDBFolderMgr();
    cout << "# All folders:" << endl << endl;
    vector<string> allFolders;
    folderMgr->getAllCondDBFolder( allFolders );
    condDBmgr->commit();
    for (unsigned int i = 0; i < allFolders.size(); i++)
	cout << "folder \"" << allFolders[i] << "\"" << endl;
}

void printFolderSets()
    throw(CondDBException)
{
    condDBmgr->startRead();
    ICondDBFolderMgr* folderMgr = condDBmgr->getCondDBFolderMgr();
    cout << "# All folders:" << endl << endl;
    vector<string> allFolders;
    folderMgr->getAllCondDBFolderSet( allFolders );
    condDBmgr->commit();
    for (unsigned int i = 0; i < allFolders.size(); i++)
	cout << "folderset \"" << allFolders[i] << "\"" << endl;
}

void printTags()
    throw(CondDBException)
{
    condDBmgr->startRead();
    ICondDBTagMgr* tagMgr = condDBmgr->getCondDBTagMgr();
    cout << "# All tags:" << endl << endl;
    vector<string> allTags;
    tagMgr->getAllCondDBTag( allTags );
    condDBmgr->commit();
    for (unsigned int i = 0; i < allTags.size(); i++)
	cout << "tag \"" << allTags[i] << "\""<< endl;
}


void iterObjectsV(const string& folder, CondDBKey point)
    throw(CondDBException)
{
    condDBmgr->startRead();
    ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
    ICondDBDataIterator* iterObjs = 0;
    // Browsing verticaly
    cout << "# All intervals at point:" << endl << endl;    
    condDataAccess->browseObjectsAtPoint(iterObjs, folder, point);
    condDBmgr->commit();
    if ( iterObjs == 0 )
    {
	cerr << "iterObjectsV: iterObjs == 0" << endl;
	return;
    }
    printAllObjs(iterObjs);
    delete iterObjs;
}

void iterObjectsH(const string& folder, const string& tag)
    throw(CondDBException)
{
    condDBmgr->startRead();
    ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
    ICondDBDataIterator* iterObjs = 0;
    // Browsing horizontally
    cout << "# All intervals in the HEAD:" << endl << endl;    
    condDataAccess->browseObjectsInTag(iterObjs, folder, tag);
    condDBmgr->commit();
    if ( iterObjs == 0 )
    {
	cerr << "iterObjectsH: iterObjs == 0" << endl;
	return;
    }
    printAllObjs(iterObjs);
    delete iterObjs;
}

void iterObjectsA(const string& folder)
    throw(CondDBException)
{
    condDBmgr->startRead();
    ICondDBDataAccess* condDataAccess = condDBmgr->getCondDBDataAccess();
    ICondDBDataIterator* iterObjs = 0;
    // Browsing horizontally
    cout << "# All objects in folder" << endl << endl;    
    condDataAccess->browseAllObjects(iterObjs, folder);
    condDBmgr->commit();
    if ( iterObjs == 0 )
    {
	cerr << "iterObjectsA: iterObjs == 0" << endl;
	return;
    }
    printAllObjs(iterObjs);
    delete iterObjs;
}


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
    cout << "object " << layerNum;
    if (format==c_double) {
	double dsince = static_cast<double>(since);
	double dtill = static_cast<double>(till);
	if ( since==0 )
	    cout << " -Inf";
	else 
	    cout << " " << dsince;
	if ( till==LLONG_MAX )
	    cout << " +Inf";
	else 
	    cout << " " << dtill;
    }
    else if (format==c_date) {
	SimpleTime ssince(since), still(till);
	cout << " \"" << ssince << "\" \"" << still << "\"";	
    }
    else if (format==c_integer) {
	// This format will suffer of every kind of weird problems.
	// We only provide this for very simple tests and because 
	// bare TCL doesn't support very long integers and +Inf is
	// bewond long integer representation.
	if (since > INT_MAX) since = INT_MAX;
	if (till > INT_MAX) till = INT_MAX;
	long lsince = static_cast<long>(since);
	long ltill = static_cast<long>(till);
	if ( since==0 )
	    cout << " -Inf";
	else 
	    cout << " " << lsince;
	if ( till==INT_MAX )
	    cout << " +Inf";
	else 
	    cout << " " << ltill;
    }
    else {
	if ( since==0 )
	    cout << " -Inf";
	else 
	    cout << " " << since;
	if ( till==LLONG_MAX )
	    cout << " +Inf";
	else 
	    cout << " " << till;
    }
    cout << " \"" << description << "\"" << endl;
}


void printAllObjs(ICondDBDataIterator* iterObjs)
{
    ICondDBObject* oCondObject = 0;
  
    do 
    {
	oCondObject = iterObjs->current();
	printObj(oCondObject);
	delete oCondObject;
    } while ( iterObjs->next() );
}


//
// THE END
//
