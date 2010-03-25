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
/* $Id: MySqlOnlineMgr.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"

#include "MySqlOnlineMgr.h"
#include "MySqlTypes.h"
#include "boot-sql.h"
#include "Blob.h"

//#include "sys/time.h"


MySqlOnlineMgr::MySqlOnlineMgr(MySqlDBMgr *dbMgr,
			       MySqlHandle *mysqlH)
    : MySqlConnection(mysqlH)
{
    databaseMgr = dbMgr;
}

MySqlOnlineMgr::MySqlOnlineMgr(MySqlDBMgr *dbMgr)
    : MySqlConnection()
{
    databaseMgr = dbMgr;
}

MySqlOnlineMgr::~MySqlOnlineMgr()
{

// que raio e que isto faz?????
//    if (next != 0) delete next;
}

/**
 * Find for structures
 * @param time Time for which we want to search
 * @param fldId Folder Id
 * @param partId Partition ID
 * @param table The CondDBTable in which we want to store the data found
 */

void MySqlOnlineMgr::findOnl(CondDBKey time, int fldId, int partId, CondDBTable *table, string Id, string selection, const vector <string> *nullValues)
	throw(CondDBException)
{
    MYSQLSTREAM query, tmpQuery;

    MySqlResult *res;

    query << "SELECT * FROM " << DATA_TBL_ONL_N << fldId << "_" << partId << " WHERE (" << time << " >= since_t AND " << time << " < till_t";
    if (Id != "")
    {
	tmpQuery << "SHOW COLUMNS FROM " << DATA_TBL_ONL_N << fldId << "_" << partId ;
	res = select(tmpQuery);
	res->seek(2);
	string name = res->getField(0);
	delete res;

	query << " AND " << name << "=\'" << Id << "\'";
    }
    if (selection != "")
    {
	query << " AND (" << selection << ")";
    }
    query << ")";

    res = select(query);
    makeTable(res, table, fldId, partId, nullValues);
    delete res; 
}

void MySqlOnlineMgr::findTag(CondDBKey time, int fldId, int partId, int tagId, CondDBTable *table, string selection,
			     const std::vector <std::string> *nullValues)
    throw(CondDBException)
{
    MYSQLSTREAM query, tmpQuery;
    
    MySqlResult *res;

    if (tagId != 0) // Tagged folder
	query << "SELECT t.since_t, t.till_t, d.*, o.layer FROM " << DATA_TBL_TAG_N << fldId << "_" << partId << " AS d, " OBJECT_KEY_TBL_N << fldId << " AS o, " TAG2OBJ_TBL_N << fldId << " AS t WHERE (" << time << " >= t.since_t AND " << time << " < t.till_t AND t.tag_id=" << tagId << " AND t.obj_id=o.obj_id AND d.dat_id=o.dat_id";
    else  // No tag, search head
	query << "SELECT h.since_t, h.till_t, d.*, o.layer FROM " << DATA_TBL_TAG_N << fldId << "_" << partId << " AS d, " OBJECT_KEY_TBL_N << fldId << " AS o, " OBJECT_KEY_TBL_HEAD_N << fldId << " AS h WHERE (" << time << " >= h.since_t AND " << time << " < h.till_t AND d.dat_id=o.dat_id AND o.obj_id=h.obj_id";

    if (selection != "")
    {
	query << " AND (" << selection << ")";
    }
    query << ")";
    
    res = select(query);
    makeTable(res, table, fldId, partId, nullValues, 1);
    delete res; 
}

    
/**
 * Browse for structures
 * @param since The lower limit of the time interval
 * @param till The upper limit of the time interval
 * @param fld_id The folder Id
 * @param part_id The partition Id
 * @param table The CondDBTable in which the data found is stored
 */

void MySqlOnlineMgr::browseOnl(CondDBKey since, CondDBKey till, 
				int fld_id, int part_id, CondDBTable *table, string Id, string selection, const vector <string> *nullValues)
    throw(CondDBException)
{
    
    MYSQLSTREAM query, tmpQuery;

    query << "SELECT * FROM " << DATA_TBL_ONL_N << fld_id << "_" << part_id << " WHERE \
(((since_t<" << since << " AND " << since << "<till_t) \
OR (since_t<" << till << " AND " << till << "<till_t) \
OR (since_t>=" << since << " AND " << till << ">=since_t))";
    if (Id != "")
    {
	tmpQuery << "SHOW COLUMNS FROM " << DATA_TBL_ONL_N << fld_id << "_" << part_id ;
	MySqlResult *res2 = select(tmpQuery);
	res2->seek(2);
	string name = res2->getField(0);
	delete res2;

	query << " AND " << name <<"=\'" << Id << "\'";
    }
    if (selection != "")
    {
	query << " AND (" << selection << ")";
    }
    query << ")";
    
    MySqlResult *res = select(query);
    makeTable(res, table, fld_id, part_id, nullValues);
    delete res; 
}

void MySqlOnlineMgr::browseHist(CondDBKey since, CondDBKey till, 
				int fld_id, int part_id, int tag_id, CondDBTable *table, string selection, const vector <string> *nullValues)
    throw(CondDBException)
{
    
    MYSQLSTREAM query, tmpQuery;

    if (tag_id != 0) // Tagged folder
	query << "SELECT t.since_t, t.till_t, d.*, o.layer FROM " << DATA_TBL_TAG_N << fld_id << "_" << part_id << " AS d, " OBJECT_KEY_TBL_N << fld_id << " AS o, " TAG2OBJ_TBL_N << fld_id << " AS t WHERE (((t.since_t<" << since << " AND " << since << "<t.till_t) OR (t.since_t<" << till << " AND " << till << "<t.till_t) OR (t.since_t>=" << since << " AND " << till << ">=t.since_t)) AND t.tag_id=" << tag_id << " AND t.obj_id=o.obj_id AND d.dat_id=o.dat_id";
    else // No tag, so we search head
	query << "SELECT h.since_t, h.till_t, d.*, o.layer FROM " << DATA_TBL_TAG_N << fld_id << "_" << part_id << " AS d, " OBJECT_KEY_TBL_N << fld_id << " AS o, " OBJECT_KEY_TBL_HEAD_N << fld_id << " AS h WHERE (((h.since_t<" << since << " AND " << since << "<h.till_t) OR (h.since_t<" << till << " AND " << till << "<h.till_t) OR (h.since_t>=" << since << " AND " << till << ">=h.since_t)) AND h.obj_id=o.obj_id AND d.dat_id=o.dat_id";

    if (selection != "")
    {
	query << " AND (" << selection << ")";
    }
    query << ")";
    MySqlResult *res = select(query);
  
    makeTable(res, table, fld_id, part_id, nullValues, 1);
    delete res;
}

void MySqlOnlineMgr::browsePoint(CondDBKey time,
				 int fldId, 
				 int partId, 
				 CondDBTable *table, 
				 string selection,
				 const std::vector <std::string> *nullValues)
    throw(CondDBException)
{
    MYSQLSTREAM query, tmpQuery;
    
    query << "SELECT o.since_t, o.till_t, d.*, o.layer FROM " OBJECT_KEY_TBL_N << fldId << " AS o, " DATA_TBL_TAG_N << fldId << "_" << partId << " AS d WHERE ((" << time << " BETWEEN o.since_t AND o.till_t) AND d.dat_id=o.dat_id";
    
    if (selection != "")
    {
	query << " AND (" << selection << ")";
    }
    query << ")";

    MySqlResult *res = select(query);
    makeTable(res, table, fldId, partId, nullValues, 1);
    delete res;
}

void MySqlOnlineMgr::browseTag(int fldId, int tagId, CondDBTable *table, string selection, const std::vector <std::string> *nullValues)
    throw(CondDBException)
{
    MYSQLSTREAM query, tmpQuery;
    
    MySqlResult *res;

    int partId = 1; //Just until partitions are implemented

    if (tagId != 0) // Tagged folder
	query << "SELECT t.since_t, t.till_t, d.*, o.layer FROM " << DATA_TBL_TAG_N << fldId << "_" << partId << " AS d, " OBJECT_KEY_TBL_N << fldId << " AS o, " TAG2OBJ_TBL_N << fldId << " AS t WHERE (t.tag_id=" << tagId << " AND t.obj_id=o.obj_id AND d.dat_id=o.dat_id";
    else  // No tag, search head
	query << "SELECT h.since_t, h.till_t, d.*, o.layer FROM " << DATA_TBL_TAG_N << fldId << "_" << partId << " AS d, " OBJECT_KEY_TBL_N << fldId << " AS o, " OBJECT_KEY_TBL_HEAD_N << fldId << " AS h WHERE (d.dat_id=o.dat_id AND o.obj_id=h.obj_id";

    if (selection != "")
    {
	query << " AND (" << selection << ")";
    }
    query << ")";

    res = select(query);
    makeTable(res, table, fldId, partId, nullValues, 1);
    delete res;
}

void MySqlOnlineMgr::browseAll(int fldId,
			       CondDBTable *table,
			       string selection,
			       const vector <string> *nullValues)
    throw(CondDBException)
{
    MYSQLSTREAM query, tmpQuery;
    
    int partId = 1; // This is only until partitions are implemented
    
    query << "SELECT o.since_t, o.till_t, d.*, o.layer FROM " OBJECT_KEY_TBL_N << fldId << " AS o, " DATA_TBL_TAG_N << fldId << "_" << partId << " AS d WHERE (o.dat_id=d.dat_id";
    
    if (selection != "")
    {
	query << " AND (" << selection << ")";
    }
    query << ")";

    MySqlResult *res = select(query);
    makeTable(res, table, fldId, partId, nullValues, 1);
    delete res;
}

void MySqlOnlineMgr::makeTable(MySqlResult *res, CondDBTable *table, int fldId, int partId, const std::vector <std::string> *nullValues, int version)
    throw(CondDBException)
{
    if (res->countRows())
    {
	getSchema(fldId, partId, table, version);
	do {
	    if (res->getField(version)!=NULL)
	    {
		unsigned fields=res->countFields();
		table->resize(res->countRows());
		vector<string> nullValuesCopy;
		if (nullValues)
		{
		    nullValuesCopy = *nullValues;
		    Assert(nullValuesCopy.size()==table->getNumColumns());
		    for (unsigned i=0; i<nullValuesCopy.size(); i++)
			table->setNull(i, nullValuesCopy[i]);
		}
		vector <string> tmp;
		vector <CondDBTable::cdb_types> types;
		table->getTypes(types);
		for (int l=1; l<=res->countRows(); l++)
		{
		    for (unsigned i=2+version; i<fields; i++)
		    {
			if (CondDBTable::cdbLongLong<types[i-2-version])
			{
			    if (res->getField(i)!=NULL)
			    {
				char *value=res->getField(i);
				blobToVector (value, tmp, types[i-2-version]);
			    }
			    else
			    {
				if (nullValues)
				    tmp.push_back(nullValuesCopy[i-2-version]);
				else
				    tmp.push_back("NULL");
			    }
			    vector <vector <string> > tmpCol;
			    tmpCol.push_back(tmp);
			    table->setColumndata (i-2-version, tmpCol);
			}
			else
			{
			    if (res->getField(i)!=NULL)
				tmp.push_back(res->getField(i));
			    else
			    {
				if (nullValues)
				    tmp.push_back(nullValuesCopy[i-2-version]);
				else
				    tmp.push_back("NULL");
			    }
			    table->setColumndata (i-2-version, tmp);
			}
			tmp.clear();
		    }
		    SimpleTime since=res->getCondDBKeyField(0);
		    SimpleTime till=res->getCondDBKeyField(1);
		    table->setSinceTime(since);
		    table->setTillTime(till);
		    res->nextRow();
		}
	    }
	} while (res->nextRow());
    }
}
/**
 * Insert values into a structure table
 * @param since The initial time of the time validity of the stored data
 * @param till The end time of the time validity of the stored data
 * @param fldId The Folder Id
 * @param tblId The database Id
 * @param data The data to be stored
 * @param name The names of the columns in which the data is inserted
 */

void MySqlOnlineMgr::insertOnl(CondDBKey since, CondDBKey till,
			       int fldId, int partId, string& data,
			       string& name)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "INSERT INTO " DATA_TBL_ONL_N << fldId << "_" << partId << "\n"
	"  (since_t,till_t" << name << ")\n VALUES(" << since << "," << till << "," << data << ")";
    execute(query);
}

void MySqlOnlineMgr::insertData(int fldId, int partId, int datId, string& data, string& name)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "INSERT INTO " DATA_TBL_TAG_N << fldId << "_" << partId << "\n"
	"  (dat_id" << name << ")\n VALUES(" << datId << "," << data << ")";
    execute(query);
}


/**
 * Create tables for Structures
 * @param folderId Id of the folder associated to the tables we want to create
 * @param names Names of the columns in which the data are stored
 * @param types Data types for each column
 * @param units Units for each column
 */

void MySqlOnlineMgr::createTablesOnl(int folderId, vector <string>& names, vector <CondDBTable::cdb_types>& types, bool Id)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    query << "CREATE TABLE " PARTITION_TBL_N << folderId << " " PARTITION_TBL_SCHEMA;
    execute(query);

    SimpleTime minf, pinf; minf.setMinusInf(); pinf.setPlusInf();

    int partId = createPartition(minf, pinf, folderId, getDatabaseId());

    query << "CREATE TABLE " DATA_TBL_ONL_N << folderId << "_" << partId << " (since_t bigint, till_t bigint";

    string initialValue, fnamelist;
    vector <string> fname, ftypes;
    for (unsigned j=0; j<types.size(); j++)
    {
	string typeName;
	switch (types[j])
	{
	    case CondDBTable::cdbFloat: case CondDBTable::cdbArrayFloat: { typeName = "FLOAT"; break;}
	    case CondDBTable::cdbInt: case CondDBTable::cdbArrayInt: { typeName = "INT"; break;}
	    case CondDBTable::cdbString: case CondDBTable::cdbArrayString: { typeName = "TEXT"; break;}
	    case CondDBTable::cdbDouble: case CondDBTable::cdbArrayDouble: { typeName = "DOUBLE"; break;}
	    case CondDBTable::cdbBool: case CondDBTable::cdbArrayBool: { typeName = "TINYINT"; break;}
	    case CondDBTable::cdbLongLong: case CondDBTable::cdbArrayLongLong: { typeName = "BIGINT"; break;}
	    default : break;
	}
	ftypes.push_back(typeName);
    }

    for (unsigned i=0; i<names.size(); i++)
    {
	string tmp=names[i];
	int n_type=types[i];
	if ( n_type>CondDBTable::cdbLongLong)
	{
	    tmp+='$'+ftypes[i];
	    ftypes[i]="BLOB";
	}
	fnamelist+=','+tmp;
	fname.push_back(tmp);
	query << "," << tmp << " " << ftypes[i];
	initialValue+="NULL,";
    }
    initialValue.erase(initialValue.size()-1);
    
    if (Id)
    {
	if (types[0] == CondDBTable::cdbString)
	    query << ", FULLTEXT INDEX ind (" << names[0] << ")";
	else 
	    query << ", INDEX ind (" << names[0] << ")";
    }
    query << ", INDEX isin (since_t), INDEX itil (till_t)) TYPE=MyISAM";
    
    execute(query);

    insertOnl(minf, pinf, folderId, partId, initialValue, fnamelist);
}


void MySqlOnlineMgr::createTablesTag(int folderId, vector <string>& names, vector <CondDBTable::cdb_types>& types, bool Id)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    query << "CREATE TABLE " TAG2OBJ_TBL_N << folderId << " " TAG2OBJ_TBL_SCHEMA;
    execute(query);
    query << "CREATE TABLE " PARTITION_TBL_N << folderId << " " PARTITION_TBL_SCHEMA;
    execute(query);
    query << "CREATE TABLE " OBJECT_KEY_TBL_N << folderId << " " TABLE_KEY_TBL_SCHEMA;
    execute (query);
    query << "CREATE TABLE " OBJECT_KEY_TBL_HEAD_N << folderId << " " TABLE_KEY_TBL_HEAD_SCHEMA;
    execute (query);

    SimpleTime minf, pinf; minf.setMinusInf(); pinf.setPlusInf();
    int partId = createPartition(minf, pinf, folderId, getDatabaseId());

    query << "CREATE TABLE " DATA_TBL_TAG_N << folderId << "_" << partId << " (dat_id int(11) NOT NULL ";

    vector <string> ftypes;
    for (unsigned j=0; j<types.size(); j++)
    {
	string typeName;
	switch (types[j])
	{
	    case CondDBTable::cdbFloat: case CondDBTable::cdbArrayFloat: { typeName = "FLOAT"; break;}
	    case CondDBTable::cdbInt: case CondDBTable::cdbArrayInt: { typeName = "INT"; break;}
	    case CondDBTable::cdbString: case CondDBTable::cdbArrayString: { typeName = "TEXT"; break;}
	    case CondDBTable::cdbDouble: case CondDBTable::cdbArrayDouble: { typeName = "DOUBLE"; break;}
	    case CondDBTable::cdbBool: case CondDBTable::cdbArrayBool: { typeName = "TINYINT"; break;}
	    case CondDBTable::cdbLongLong: case CondDBTable::cdbArrayLongLong: { typeName = "BIGINT"; break;}
	    default : break;
	}
	ftypes.push_back(typeName);
    }

    for (unsigned i=0; i<names.size(); i++)
    {
	string tmp=names[i];
	int n_type=types[i];
	if ( n_type>CondDBTable::cdbLongLong)
	{
	    tmp+='$'+ftypes[i];
	    ftypes[i]="BLOB";
	}
	query << "," << tmp << " " << ftypes[i];
    }
    
    if (Id)
    {
	if (types[0] == CondDBTable::cdbString)
	    query << ", FULLTEXT INDEX ind (" << names[0] << ")";
	else 
	    query << ", INDEX ind (" << names[0] << ")";
    }

    query << ", INDEX idat (dat_id)) TYPE=MyISAM";
    execute(query);
}


/**
 * Create a partition (time partition). Its like a folder but based on time
 * instead of category. This eases the scaling over time. 
 */

int MySqlOnlineMgr::createPartition(CondDBKey since, CondDBKey till,
				    int folderId, int dbId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "INSERT INTO " PARTITION_TBL_N << folderId 
	  << " (since_t, till_t, db_id)\n  VALUES("
	  << since << "," << till << "," << dbId << ")";
    execute(query);
    query << "SELECT LAST_INSERT_ID()";
    MySqlResult *res = select(query);
    int part_id = res->getIntField(0);
    delete res;
    return part_id;
}

/**
 * Return the databaseId for the current open connection
 */

int MySqlOnlineMgr::getDatabaseId()
{
    return databaseId;
}

/**
 * Return the partition id for the partition that contains
 * the time given by 'point'.
 */

int MySqlOnlineMgr::getPartitionId(CondDBKey point, int folderId)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    query << "SELECT part_id FROM " PARTITION_TBL_N << folderId << "\n"
	"  WHERE (" << point << " BETWEEN since_t AND till_t)\n";

    MySqlResult *res = select(query);
    
    if ( res->countRows() == 0 ) {
	THROWEX("None partition has been created yet!",0);
    }

    int partitionId = res->getIntField(0);
    delete res;
    return partitionId;
}

/**
 * Destroy the tables associated with the folder given by folderId 
 */

void MySqlOnlineMgr::deleteTables(int folderId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "DROP TABLE " PARTITION_TBL_N << folderId;
    execute(query);
    query << "DROP TABLE " DATA_TBL_ONL_N << folderId << "_1";
    execute(query);
}

/**
 * Store data in a structure
 * @param since The since time for the values stored
 * @param till The till time for the values stored
 * @param fldId The folder Id
 * @param partId The partition Id
 * @param table The CondDBTable which contains the data to be stored
 */

void MySqlOnlineMgr::storeOnl(int fldId, int partId, CondDBTable *table)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    
    query << "SELECT * FROM " DATA_TBL_ONL_N << fldId << "_" << partId << " WHERE since_t=0";
    MySqlResult *res = select(query);

    unsigned fields=res->countFields();
    string fname="";
    unsigned columns=table->getNumColumns();
    vector <string> columnNames, values;
    vector <CondDBTable::cdb_types> types;
    table->getNames(columnNames);
    table->getTypes(types);
    Assert (columns==fields-2); // Check that columns = fields
      
    for (unsigned t=2; t<fields; t++)
    {
	string fnameTemp=res->getFieldName(t);
	fname+=','+fnameTemp;
	string name, type;
	breakUp (fnameTemp, name, type);
	Assert (name==columnNames[t-2]); //Check columns are coincident
    }

    for (unsigned l=0; l<table->getNumRows(); l++)
    {
	table->getRow (l, values);
	int pos=0;
	
	string dat;
	for (unsigned i=2; i<fields; i++)
	{
	    int n_vals;
	    if (CondDBTable::cdbLongLong<types[i-2])
	    {
		n_vals=atol(values[pos].c_str());
		pos++;
	    }
	    else 
		n_vals=1;
	    vector <string> data;
	    string null;
	    CondDBTable::cdb_types type=types[i-2];
	    table->getNull(i-2, null);
	    for (long c=pos; c<pos+n_vals; c++)
	    {
		if (!compareNull(type, values[c], null) || n_vals>1)
		{
		    string tmp;
		    escapeBinaryString(tmp, values[c]);
		    data.push_back(tmp);
		}
		else 
		    data.push_back("NULL");
	    }
	    pos+=n_vals;
	    
	    if (n_vals==0)
		data.push_back("NULL");
	    
	    int n_type=types[i-2];
	    if (n_type > CondDBTable::cdbLongLong)
	    {
		if (data[0]!="NULL")
		{
		    string tmp1, tmp2;
		    vectorToBlob(&tmp1, data, types[i-2]);
		    escapeBinaryString(tmp2, tmp1);
		    dat+="'"+tmp2+"'"+',';
		}
		else
		    dat+=data[0]+',';
	    }
	    else
	    {
		if (data[0]!="NULL")
		    dat+="'"+data[0]+"',";
		else
		    dat+=data[0]+',';
	    }
	    data.clear();
	}
	dat.erase(dat.size()-1);
	
	query << "LOCK TABLES " DATA_TBL_ONL_N << fldId
	      << "_" << partId << " WRITE";
	execute(query);
	
	SimpleTime tills, sinces;
//	table->getTillTime(l, tills);
	tills.setPlusInf();
	table->getSinceTime(l, sinces);
	CondDBKey since(sinces), till(tills);
	
	query << "UPDATE " DATA_TBL_ONL_N << fldId << "_" << partId << " SET till_t=" << since << " WHERE till_t>" << since;
	execute(query);
	
    // Finally insert the object
	insertOnl(since, till, fldId, partId, dat, fname);
	
//	dat.clear();
	values.clear();
    }
    query << "UNLOCK TABLES";
    execute(query);
    delete res;
}

void MySqlOnlineMgr::storeOnlId (int fldId, int partId, CondDBTable *table)
	throw(CondDBException)
{
    MYSQLSTREAM query;
    
    query << "SELECT * FROM " DATA_TBL_ONL_N << fldId << "_" << partId << " WHERE since_t=0";
    MySqlResult *res = select(query);

    unsigned fields=res->countFields();
    string fname="", Id, blockId, colId;
    unsigned columns=table->getNumColumns();
    vector <string> columnNames, values, block;
    vector <CondDBTable::cdb_types> types;
    CondDBKey since, till;
    SimpleTime tills;
    tills.setPlusInf();
    till = tills;
    table->getNames(columnNames);
    table->getTypes(types);
    Assert (columns==fields-2); // Check that columns = fields

    colId = res->getFieldName(2);
      
    for (unsigned t=2; t<fields; t++)
    {
	string fnameTemp=res->getFieldName(t);
	fname+=','+fnameTemp;
	string name, type;
	breakUp (fnameTemp, name, type);
	Assert (name==columnNames[t-2]); //Check columns are coincident
    }
    
    query << "LOCK TABLES " DATA_TBL_ONL_N << fldId << "_" << partId << " WRITE";
    execute(query);
    
    query << "SELECT " << colId << ", since_t FROM " DATA_TBL_ONL_N << fldId << "_" << partId << " WHERE till_t=" << till << " GROUP BY " << colId << ",since_t";
    res = select(query);

    //Build Map Id to since_t
    
    map<string, CondDBKey> idToTime;

    CondDBKey mapSince;
    string mapId;
    if (res->countRows())    
	do
	{
	    if (res->getField(0)!=0)
	    {
		mapId = res->getField(0);
		mapSince = res->getCondDBKeyField(1);
		idToTime[mapId] = mapSince;
	    }
	} while (res->nextRow());

    table->getRow (0, values);
    Id = values[0];
    unsigned l=0;
    while (l<table->getNumRows())
    {
	do
	{
	    int pos=0;
	    string dat;
	    for (unsigned i=2; i<fields; i++)
	    {
		int n_vals;
		if (CondDBTable::cdbLongLong<types[i-2])
		{
		    n_vals=atol(values[pos].c_str());
		    pos++;
		}
		else 
		    n_vals=1;
		vector <string> data;
		string null;
		CondDBTable::cdb_types type=types[i-2];
		table->getNull(i-2, null);
		for (long c=pos; c<pos+n_vals; c++)
		{
		    if (!compareNull(type, values[c], null) || n_vals>1)
		    {
			string tmp;
			escapeBinaryString(tmp, values[c]);
			data.push_back(tmp);
		    }
		    else 
			data.push_back("NULL");
		}
		pos+=n_vals;
		
		if (n_vals==0)
		    data.push_back("NULL");

		int n_type=types[i-2];
		if (n_type > CondDBTable::cdbLongLong)
		{
		    string tmp1, tmp2;
		    if (data[0]!="NULL")
		    {
			vectorToBlob(&tmp1, data, types[i-2]);
			escapeBinaryString(tmp2, tmp1);
			dat+="'"+tmp2+"'"+',';
		    }
		    else dat+=data[0]+',';
		}
		else
		{
		    if (data[0]!="NULL")
			dat+="'"+data[0]+"',";
		    else 
			dat+=data[0]+',';
		}
		data.clear();
	    }

	    dat.erase(dat.size()-1);
	    block.push_back(dat);
	    blockId = Id;
	    l++;
	    values.clear();
	    table->getRow (l, values);
	    Id = values[0];
	} while ( blockId==Id && l<table->getNumRows());
    
	SimpleTime tills, sinces;
	table->getSinceTime(l-1, sinces);
	since=sinces;
	
	if (idToTime[blockId]<since || idToTime.empty())
	{
	    
	    query << "UPDATE " DATA_TBL_ONL_N << fldId << "_" << partId << " SET till_t=" << since << " WHERE (till_t>" << since << " AND (" << columnNames[0] << "='" << blockId << "' OR "<< columnNames[0] << " IS NULL))";
	    execute(query);
	    
	    for (unsigned b=0; b<block.size(); b++)
		insertOnl(since, till, fldId, partId, block[b], fname);
	}
	block.clear();
    }
    
    query << "UNLOCK TABLES";
    execute(query);
    delete res;
}


void MySqlOnlineMgr::storeVer(int fldId, int partId, CondDBTable *table)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    query << "LOCK TABLES " DATA_TBL_TAG_N << fldId << "_" << partId << " WRITE, " OBJECT_KEY_TBL_N << fldId
	  << " AS o WRITE, " OBJECT_KEY_TBL_N << fldId << " WRITE," OBJECT_KEY_TBL_HEAD_N << fldId 
       	  << " AS h WRITE, " OBJECT_KEY_TBL_HEAD_N << fldId << " WRITE, " TAG2OBJ_TBL_N << fldId << " WRITE";
    execute(query);
    
    query << "SELECT MAX(dat_id) AS dat_id FROM " DATA_TBL_TAG_N << fldId << "_" << partId ;
    MySqlResult *res = select(query);
    Assert (res->countRows()==1);
    int datId;
    if (res->getField(0)!=NULL)
	datId = res->getIntField(0)+1;
    else
	datId = 0;

    query << "SELECT * FROM " DATA_TBL_TAG_N << fldId << "_" << partId << " WHERE dat_id=0";
    res = select(query);

    unsigned fields=res->countFields();
    string fname="";
    unsigned columns=table->getNumColumns();
    vector <string> columnNames, values;
    vector <CondDBTable::cdb_types> types;
    table->getNames(columnNames);
    table->getTypes(types);
    Assert (columns==fields-1); // Check that columns = fields
      
    for (unsigned t=1; t<fields; t++)
    {
	string fnameTemp=res->getFieldName(t);
	fname+=','+fnameTemp;
	string name, type;
	breakUp (fnameTemp, name, type);
	Assert (name==columnNames[t-1]); //Check columns are coincident
    }

    for (unsigned l=0; l<table->getNumRows(); l++)
    {
	table->getRow (l, values);
	int pos=0;
	
	string dat;
	for (unsigned i=1; i<fields; i++)
	{
	    int n_vals;
	    if (CondDBTable::cdbLongLong<types[i-1])
	    {
		n_vals=atol(values[pos].c_str());
		pos++;
	    }
	    else 
		n_vals=1;
	    vector <string> data;
	    string null;
	    CondDBTable::cdb_types type=types[i-1];
	    table->getNull(i-1, null);
	    for (long c=pos; c<pos+n_vals; c++)
	    {
		if (!compareNull(type, values[c], null) || n_vals>1)
		{
		    string tmp;
		    escapeBinaryString(tmp, values[c]);
		    data.push_back(tmp);
		}
		else 
		    data.push_back("NULL");
	    }
	    pos+=n_vals;
	    
	    if (n_vals==0)
		data.push_back("NULL");

	    int n_type=types[i-1];
	    if (n_type > CondDBTable::cdbLongLong)
	    {
		if (data[0]!="NULL")
		{
		    string tmp1, tmp2;
		    vectorToBlob(&tmp1, data, types[i-1]);
		    escapeBinaryString(tmp2, tmp1);
		    dat+="'"+tmp2+"'"+',';
		}
		else
		    dat+=data[0]+',';
	    }
	    else
	    {
		if (data[0]!="NULL")
		    dat+="'"+data[0]+"',";
		else
		    dat+=data[0]+',';
	    }
	    data.clear();
	}

	dat.erase(dat.size()-1);
	// Insert the data
	insertData(fldId, partId, datId, dat, fname);
	

	values.clear();
    }
    SimpleTime tills, sinces;
    table->getTillTime(0, tills);
    table->getSinceTime(0, sinces);
    CondDBKey since=sinces;
    CondDBKey till=tills;
    
    store(since, till, fldId, partId, datId);
        
    query << "UNLOCK TABLES";
    execute(query);
    delete res;
}

void MySqlOnlineMgr::store(CondDBKey since, CondDBKey till, int folderId, int dbId, int datId)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    // Get the objects overlapping the current object time validity interval plus the respective layer
    query << "SELECT h.*, o.layer FROM " OBJECT_KEY_TBL_HEAD_N << folderId << " AS h, " OBJECT_KEY_TBL_N << folderId 
	  << " AS o WHERE (h.since_t<" << till << " AND h.till_t>" << since << " AND h.obj_id=o.obj_id)";
    MySqlResult *res = select(query);
    
    if (res->countRows()!=0)
    {
	CondDBKey since_t, till_t;
	int layer, tmpLay, objId;
	string objsIds;
	string tmp;
	
	tmp = res->getField(0);
	objsIds = tmp;

	since_t = res->getCondDBKeyField(1);
	till_t = res->getCondDBKeyField(2);
	objId = atoi(tmp.c_str());
	layer = res->getIntField(3);
	
	if ((since_t<since) && (till_t>since) && (till_t<=till))
	{
	    query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET till_t=" << since << " WHERE obj_id=" << objId;
	    execute(query);
	}
	else if ((since_t>=since) && (till_t<=till))
	{
	    query << "DELETE FROM " OBJECT_KEY_TBL_HEAD_N << folderId << " WHERE obj_id=" << objId;
	    execute(query);
	}
	else if ((since_t<till) && (till_t>till) && (since_t>=since))
	{
	    query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET since_t=" << till << " WHERE obj_id=" << objId;
	    execute(query); 
	}
	else if ((since_t<since) && (till_t>till))
	{
	    query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET till_t=" << since << " WHERE obj_id=" << objId;
	    execute(query);
	    query << "INSERT INTO " OBJECT_KEY_TBL_HEAD_N << folderId << " (obj_id, since_t, till_t)\n"
		" VALUES(" << objId << "," << till << "," << till_t << ")";
	    execute(query); 
	}

	while (res->nextRow())
	{
	    tmp = res->getField(0);
	    objsIds += "," + tmp;
	    since_t = res->getCondDBKeyField(1);
	    till_t = res->getCondDBKeyField(2);
	    objId = atoi(tmp.c_str());
	    tmpLay = res->getIntField(3);
	    if (tmpLay>layer) layer = tmpLay;

	    if ((since_t<since) && (till_t>since) && (till_t<=till))
	    {
		query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET till_t=" << since << " WHERE obj_id=" << objId;
		execute(query);
	    }
	    else if ((since_t>=since) && (till_t<=till))
	    {
		query << "DELETE FROM " OBJECT_KEY_TBL_HEAD_N << folderId << " WHERE obj_id=" << objId;
		execute(query);
	    }
	    else if ((since_t<till) && (till_t>till) && (since_t>=since))
	    {
		query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET since_t=" << till << " WHERE obj_id=" << objId;
		execute(query); 
	    }
	    else if ((since_t<since) && (till_t>till))
	    {
		query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET till_t=" << since << " WHERE obj_id=" << objId;
		execute(query);
		query << "INSERT INTO " OBJECT_KEY_TBL_HEAD_N << folderId << " (obj_id, since_t, till_t)\n"
		    " VALUES(" << objId << "," << till << "," << till_t << ")";
		execute(query); 
	    }
	    
	} 
	layer++;
	// Finally insert the object
	insert(since, till, folderId, layer, dbId, datId);
    }
    else
    {
	// Found no objects for the given time interval so let's just insert it in the bottom layer
        // and add objId + Time Validity to head
	insert (since, till, folderId, 0, dbId, datId);
    }
    delete res;
    // release the lock
    query << "UNLOCK TABLES";
    execute(query);
}

void MySqlOnlineMgr::insert(CondDBKey since, CondDBKey till, int folderId, int layer, int tblId, int datId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "INSERT INTO " OBJECT_KEY_TBL_N << folderId << "\n"
	"  (insert_t,since_t,till_t,layer,db_id,dat_id)\n"
	"  VALUES(NOW()," << since << "," << till << "," 
	  << layer << "," << tblId << "," << datId << ")";
    execute(query);
    query << "INSERT INTO " OBJECT_KEY_TBL_HEAD_N << folderId << " (obj_id, since_t, till_t)\n"
	" VALUES(LAST_INSERT_ID()," << since << "," << till << ")";
    execute(query);
}


/**
 * Connection management
 */

void MySqlOnlineMgr::changeDatabase(const string& srvname,
				    const string& username,
				    const string& password,
				    const string& dbname,
				    int dbId)
    throw(CondDBException)
{
    connect(srvname, username, password);

    if(changeDB(dbname)==false)
	THROWEX("MySqlOnlineMgr::changeDatabase(...) failed",0);
    databaseId = dbId;
}

/**
 * Get table schema for structures
 * @param fldId The folder Id
 * @param types A vector containing the types of the columns of the table
 */ 

void MySqlOnlineMgr::getSchema(int fldId, int partId, CondDBTable *table, bool versions)
{
    MYSQLSTREAM query;
    int startCol;
    if (!versions)
    {
	query << "SHOW COLUMNS FROM " DATA_TBL_ONL_N << fldId << "_" << partId;
	startCol = 2;
    }
    else
    {
	query << "SHOW COLUMNS FROM " DATA_TBL_TAG_N << fldId << "_" << partId;
	startCol = 1;
    }
    MySqlResult *res = select(query);
    int fields=res->countRows();
    int columns=table->getNumColumns();
    if (columns!=0)
	cout << "Table already initialized" << endl;
    table->Initialize(fields-startCol+versions);
    res->seek(startCol);
    for (int i=startCol; i<fields; i++)
    {
	string fname = res->getField(0);
	string type = res->getField(1);
	string name;
	CondDBTable::cdb_types colType;
	if (type!="blob")
	{
	    if (type=="float") colType=CondDBTable::cdbFloat;
	    else if (type=="int(11)") colType=CondDBTable::cdbInt;
	    else if (type=="text") colType=CondDBTable::cdbString;
	    else if (type=="double") colType=CondDBTable::cdbDouble;
	    else if (type=="tinyint(4)") colType=CondDBTable::cdbBool;
	    else if (type=="bigint(20)") colType=CondDBTable::cdbLongLong;
	    else cout << "Undefined" << endl;
	    name=fname;
	}
	else 
	{
	    breakUp (fname, name, type);
	    typeNameToEnum(type, colType);
	}
	table->setType(i-startCol, colType);
	table->setName(i-startCol, name);
	res->nextRow();
    }
    if(versions)
    {
	int cols = table->getNumColumns();
	table->setType(cols-1, CondDBTable::cdbInt);
	table->setName(cols-1, "layer");
    }

    vector<CondDBTable::cdb_types> types;
    vector<string> names;
/*
    table->getNames(names);
    table->getTypes(types);

    cout << "names: " << names.size() << endl;
    cout << "types: " << types.size() << endl;
*/
    delete res;
}

// New functions

void MySqlOnlineMgr::browseId (int folderId, int tagId, ICondDBTimePath& path)
	throw(CondDBException)
{
    MYSQLSTREAM query;
    if (tagId == 0)
	query << "SELECT since_t,t_time,obj_id FROM " OBJECT_KEY_TBL_HEAD_N << folderId;
    else
	query << "SELECT since_t,t_time,obj_id FROM " TAG2OBJ_TBL_N << folderId << " WHERE tag_id=" << tagId;
    MySqlResult *res = select(query);
    CondDBTimeStep *step = new CondDBTimeStep ();
    if (res->countRows()!=0)
	do
	{
	    step->setSince(res->getCondDBKeyField(0));
	    step->setTill(res->getCondDBKeyField(1));
	    step->setId(res->getIntField(2));
	    path.push_back(step);
	} while (res->nextRow());
    delete step;
}

void MySqlOnlineMgr::tagId (int folderId, int tagId, const ICondDBTimePath& path)
	throw(CondDBException)
{
    MYSQLSTREAM query;
    
    query << "INSERT INTO " TAG2OBJ_TBL_N << folderId << " (tag_id,since_t,till_t,obj_id) VALUES ";
    
    ICondDBTimeStep *step = new CondDBTimeStep();
    if (path.size()>0)
    {
	step = path[0];
	query << "(" << tagId << "," << step->getSince() << "," << step->getTill() << "," << step->getId() << ")";
    }
    for (unsigned i=1; i<path.size(); i++)
    {
	step = path[i];
	query << "(" << tagId << "," << step->getSince() << "," << step->getTill() << "," << step->getId() << ")";
    }
    execute(query);
    delete step;
}


/**
 * Auxiliary functions
 */

void MySqlOnlineMgr::escapeBinaryString(string& target, const string& source)
    throw(CondDBException)
{
    unsigned int len = source.size();
    if( len==0 ) { target=""; return; }

    char *_target = new char[len*2];
    Assert( _target != 0 );
    
    len = mysql_real_escape_string(mysqlHandle->handle(), _target,
				   source.c_str(), len);
    
    target.assign(_target, len);
    delete[] _target;
}

/**
 * Splits column name into name, unit and type
 */

void MySqlOnlineMgr::breakUp (string fname, string& name, string &type)
{
    string temp=fname;
 
    int sep = temp.find_first_of("$");
    int end = temp.size();
    name = temp.substr(0, sep);
    temp = temp.substr(sep + 1, end);
    
    if (sep==-1)
	temp="";

    //  cout << "name:" << name << endl;

    sep = temp.find_first_of("$");
    end = temp.size();
    type = temp.substr(0, sep);
    
    //cout << "type: " << type << endl;
}

void MySqlOnlineMgr::typeNameToEnum(string type, CondDBTable::cdb_types& colType)
{

    if (type=="FLOAT") colType=CondDBTable::cdbArrayFloat; 
    if (type=="INT") colType=CondDBTable::cdbArrayInt;
    if (type=="DOUBLE") colType=CondDBTable::cdbArrayDouble;
    if (type=="TEXT") colType=CondDBTable::cdbArrayString;
    if (type=="TINYINT") colType=CondDBTable::cdbArrayBool;
    if (type=="BIGINT") colType=CondDBTable::cdbArrayLongLong;

}

bool MySqlOnlineMgr::compareNull(CondDBTable::cdb_types&type, string& value, string& null)
{
    switch (type)
    {
	case CondDBTable::cdbInt: 
	case CondDBTable::cdbArrayInt: 
	{
	    int val=atoi(value.c_str());
	    int nul=atoi(null.c_str());
	    return (val==nul);
	}
	case CondDBTable::cdbFloat: 
	case CondDBTable::cdbArrayFloat: 
	{
	    float val=atof(value.c_str());
	    float nul=atof(null.c_str());
	    return (val==nul);
	}
	case CondDBTable::cdbDouble: 
	case CondDBTable::cdbArrayDouble: 
	{
	    double val=atof(value.c_str());
	    double nul=atof(null.c_str());
	    return (val==nul);
	}
	case CondDBTable::cdbString: 
	case CondDBTable::cdbArrayString: 
	{
	    return (value==null);
	}
	case CondDBTable::cdbLongLong:
	case CondDBTable::cdbArrayLongLong:
	{
	    int64 val=atoi64(value.c_str());
	    int64 nul=atoi64(null.c_str());
	    return (val==nul);
	}
	case CondDBTable::cdbBool:
	case CondDBTable::cdbArrayBool:
	{
	    return (atoi(value.c_str())==-1);
	}
	default: return false;
    }
}






