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
/* $Id: MySqlObjectMgr.cxx,v 1.3 2009-11-23 11:56:33 meyern Exp $ */

// $HEAD 10
//
// File: "MySqlObjectMgr.cxx"
//
// Created at Wed Aug 21 12:03:10 WEST 2002
// by Jorge Lima, portuguese TDAQ group, to the atlas collaborations.
//
// Based on the CERN IT Objectivity Implementation.
//



#include <string>
#include "Debug.h"
#include "ConditionsDB/CondDBKey.h"
#include "CondDBObject.h"

#include "MySqlObjectMgr.h"
#include "MySqlTypes.h"
#include "boot-sql.h"

MySqlObjectMgr::MySqlObjectMgr(MySqlDBMgr *dbMgr,
			       MySqlHandle *mysqlH)
    : MySqlConnection(mysqlH)
{
    databaseMgr = dbMgr;
}

MySqlObjectMgr::MySqlObjectMgr(MySqlDBMgr *dbMgr)
    : MySqlConnection()
{
    databaseMgr = dbMgr;
}

MySqlObjectMgr::~MySqlObjectMgr()
{
    if (next != 0) delete next;
}

// Object management functions


/**
 * Find a Conditions Object in the folder identified by 'folderId', crossing the
 * time given by point and tagged with the tag whose id is 'tagId' 
 */

MySqlResult *MySqlObjectMgr::find(CondDBKey point, int folderId, int tagId)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    if ( tagId == 0 ) 
    { //means we are looking at HEAD
	query << "SELECT h.obj_id, o.insert_t, h.since_t, h.till_t, o.layer, o.db_id, o.dat_id FROM " OBJECT_KEY_TBL_HEAD_N << folderId << " AS h, " << OBJECT_KEY_TBL_N << folderId << " AS o WHERE (" << point << " >=  h.since_t AND " << point << " < h.till_t AND h.obj_id=o.obj_id)";
    }
    else { // otherwise look for tagged objects
	// check if folder has ever been tagged to avoid unecessary queries
	query << "SELECT * FROM " TAG2OBJ_TBL_N << folderId
	      << "\n  WHERE " << tagId << "=tag_id LIMIT 1";

	MySqlResult *res = select(query);
    
	// No tags exist so why do this?
	if (res->countRows() == 0)
	{
	    cerr << "No objects tagged for this folder" << endl;
	}
	else
	{
	    query << "SELECT t.obj_id, o.insert_t, t.since_t, t.till_t, o.layer, o.db_id, o.dat_id FROM " OBJECT_KEY_TBL_N << folderId << " AS o,\n" TAG2OBJ_TBL_N << folderId << " AS t WHERE (" << point << " >= t.since_t AND " << point << " < t.till_t) AND t.tag_id=" << tagId << " AND t.obj_id=o.obj_id";
	}
	delete res;
    }
    return select (query);
}

/**
 * Browse all Conditions Objects in the folder identified by 'folderId'.
 */

MySqlResult *MySqlObjectMgr::browseAll(int folderId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT * FROM " OBJECT_KEY_TBL_N << folderId;
    return select(query);
}


/**
 * Browse all Conditions Objects in the folder identified by 'folderId' and
 * tagged with the tag whose id is 'tagId'.
 */

MySqlResult *MySqlObjectMgr::browseTagged(int folderId, int tagId)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    if ( tagId == 0 ) { //means we are looking at HEAD
	query << "SELECT h.obj_id, o.insert_t, h.since_t, h.till_t, o.layer, o.db_id, o.dat_id FROM " OBJECT_KEY_TBL_HEAD_N << folderId << " AS h, " << OBJECT_KEY_TBL_N << folderId << " AS o WHERE h.obj_id=o.obj_id";
    }
    else if(tagId==-1){
	return NULL;
    }
    else {
	// check if folder has ever been tagged
	query << "SELECT * FROM " TAG2OBJ_TBL_N << folderId << "\n"
	    "  WHERE " << "tag_id =" << tagId << " LIMIT 1";
	
	MySqlResult *res = select(query);
	
	if (res->countRows() == 0)
	{
	    cerr << "No objects tagged for this folder" << endl;
	}
	else
	{
	    query << "SELECT t.obj_id, o.insert_t, t.since_t, t.till_t, o.layer, o.db_id, o.dat_id FROM " TAG2OBJ_TBL_N 
		  << folderId << " AS t, " << OBJECT_KEY_TBL_N << folderId 
		  << " AS o WHERE t.obj_id=o.obj_id AND t.tag_id=" << tagId;
	}
	delete res;
    }

    return select (query);
}
    
    
/**
 * Browse all Conditions Objects in the folder identified by 'folderId' and crossing
 * the time given by 'point'.
 */

MySqlResult *MySqlObjectMgr::browseAtPoint(CondDBKey point, int folderId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT * FROM " OBJECT_KEY_TBL_N << folderId << " WHERE (" << point << " BETWEEN since_t AND till_t)";
    return select(query);
}

/**
 * Browse all Conditions Objects in the folder identified by 'folderId' and
 * touching the time interval given by 'begin' and 'end'.
 */

MySqlResult *MySqlObjectMgr::browseInInterval(CondDBKey begin, CondDBKey end, int folderId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "SELECT * FROM " OBJECT_KEY_TBL_N << folderId << " WHERE ( since_t BETWEEN " << begin << " AND " << end << " ) OR ( till_t BETWEEN " << begin << " AND " << end << " )";
    return select(query);
}

/**
 * Browse all objects in an interval defined by two times
 * 
 * @since The beggining of time interval
 * @till  is the end of the interval
 * @folderId is the id of the folder to search
 * @return a MySqlResult with the query results
 */

MySqlResult *MySqlObjectMgr::browseHistory(int folderId, CondDBKey since, CondDBKey till, int tagId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    MySqlResult *res;

    if ( tagId == 0 ) 
      { //means we are looking at HEAD
	query << "SELECT h.obj_id, o.insert_t, h.since_t, h.till_t, o.layer, o.db_id, o.dat_id FROM " OBJECT_KEY_TBL_HEAD_N 
	      << folderId << " AS h, " << OBJECT_KEY_TBL_N 
	      << folderId << " AS o WHERE h.since_t <" 
	      << till     << " AND h.till_t >" 
	      << since    << " AND h.obj_id=o.obj_id";
    }
    else if(tagId==-1){
	return NULL;
    }
    else {
	// check if folder has ever been tagged
	query << "SELECT * FROM " TAG2OBJ_TBL_N << folderId << "\n"
	    "  WHERE " << "tag_id =" << tagId << " LIMIT 1";
	
	res = select(query);
	
	if (res->countRows() == 0){
	    cerr << "No objects tagged for this folder" << endl;
	}
	else{
	    query << "SELECT t.obj_id, o.insert_t, t.since_t, t.till_t, o.layer, o.db_id, o.dat_id FROM " TAG2OBJ_TBL_N 
		  << folderId << " AS t, " << OBJECT_KEY_TBL_N 
		  << folderId << " AS o WHERE t.since_t <" 
		  << till     << " AND t.till_t >" 
		  << since    << " AND t.tag_id=" 
		  << tagId    << " AND t.obj_id=o.obj_id";
	}
	delete res;
    }
    return select(query);
}

/**
 * Private method: blindly insert an object on objects table
 */

void MySqlObjectMgr::insert(CondDBKey since, CondDBKey till,
			   int folderId, int layer, int tblId, int datId)
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
 * Create the object tables for a given folder and the initial objects to
 * cover the bottom of the folder with an empty object from -inf to +inf.
 */

void MySqlObjectMgr::createTables(int folderId)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    query << "CREATE TABLE " OBJECT_KEY_TBL_N << folderId << " " OBJECT_KEY_TBL_SCHEMA;
    execute(query);
    query << "CREATE TABLE " TAG2OBJ_TBL_N << folderId << " " TAG2OBJ_TBL_SCHEMA;
    execute(query);
    query << "CREATE TABLE " PARTITION_TBL_N << folderId << " " PARTITION_TBL_SCHEMA;
    execute(query);
    query << "CREATE TABLE " OBJECT_KEY_TBL_HEAD_N << folderId << " " OBJECT_KEY_TBL_HEAD_SCHEMA;
    execute(query);

    SimpleTime minf, pinf; minf.setMinusInf(); pinf.setPlusInf();
    
    int part_id = createPartition(minf, pinf, folderId, getDatabaseId());

    // Default data table is in the same database as object_key
    query << "CREATE TABLE " DATA_TBL_NN << folderId << "_" 
	  << part_id << " " DATA_TBL_SCHEMA;
    execute(query);

    // Insert the floor object
    // insert(minf, pinf, folderId, 0 /*layer*/, getDatabaseId(), 0 /*dat_id*/);

}

/**
 * Create a partition (time partition). Its like a folder but based on time
 * instead of category. This eases the scalling over time. 
 */

int MySqlObjectMgr::createPartition(CondDBKey since, CondDBKey till,
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
 * Return the databaseId for the corrent opened connection
 */

int MySqlObjectMgr::getDatabaseId()
{
    return databaseId;
}

/**
 * Return the partition id for the time partition that contains
 * the time given by 'point'.
 */

int MySqlObjectMgr::getPartitionId(CondDBKey point, int folderId)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    query << "SELECT part_id FROM " PARTITION_TBL_N << folderId << "\n"
	"  WHERE (" << point << " BETWEEN since_t AND till_t)\n";

    MySqlResult *res = select(query);
    
    if ( res->countRows() == 0 ) {
	THROWEX("No partition has been created yet!",0);
    }

    int partitionId = res->getIntField(0);
    delete res;
    return partitionId;
}

/**
 * Destroy the object tables associated with the folder given by folderId 
 */

void MySqlObjectMgr::deleteTables(int folderId,int folderType)
    throw(CondDBException)
{
    // should iterate through all objects and delete the corresponding data?
    MYSQLSTREAM query;
    query << "DROP TABLE " TAG2OBJ_TBL_N << folderId;
    execute(query);
    query << "DROP TABLE " OBJECT_KEY_TBL_N << folderId;
    execute(query);
    query << "DROP TABLE " PARTITION_TBL_N << folderId;
    execute(query);
    query << "DROP TABLE " OBJECT_KEY_TBL_HEAD_N << folderId;
    execute(query);
	// we should drop all tables associated with this folder, not just this one
    
    if (folderType == ICondDBFolder::STRUCTTAG)
	query << "DROP TABLE " DATA_TBL_TAG_N << folderId << "_1";
    else
	if (folderType == ICondDBFolder::BLOB || folderType == ICondDBFolder::BLOBTAG)
	    query << "DROP TABLE " DATA_TBL_NN << folderId << "_1";
	else
	    cerr << "Wrong folder type" << endl;
    
    execute(query);
}


/**
 * Store an object and fill the gaps resulting from partial object overlaping
 */
// Corrected bug in update of head table
void MySqlObjectMgr::store(CondDBKey since, CondDBKey till,
			   int folderId, int dbId, int datId)
    throw(CondDBException)
{
    MYSQLSTREAM query;

    // lock tables
    query << "LOCK TABLES " OBJECT_KEY_TBL_N << folderId
	  << " AS o WRITE, " OBJECT_KEY_TBL_N << folderId << " WRITE," OBJECT_KEY_TBL_HEAD_N << folderId 
       	  << " AS h WRITE, " OBJECT_KEY_TBL_HEAD_N << folderId << " WRITE, " TAG2OBJ_TBL_N << folderId << " WRITE";
    execute(query);

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
	    query << " AND since_t=" << since_t << " AND till_t=" << till_t;
	    execute(query);
	}
	else if ((since_t>=since) && (till_t<=till))
	{
	    query << "DELETE FROM " OBJECT_KEY_TBL_HEAD_N << folderId << " WHERE obj_id=" << objId;
	    query << " AND since_t=" << since_t << " AND till_t=" << till_t;
	    execute(query);
	}
	else if ((since_t<till) && (till_t>till) && (since_t>=since))
	{
	    query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET since_t=" << till << " WHERE obj_id=" << objId;
	    query << " AND since_t=" << since_t << " AND till_t=" << till_t;
	    execute(query); 
	}
	else if ((since_t<since) && (till_t>till))
	{
	    query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET till_t=" << since << " WHERE obj_id=" << objId;
	    query << " AND since_t=" << since_t << " AND till_t=" << till_t;
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
		query << " AND since_t=" << since_t << " AND till_t=" << till_t;
		execute(query);
	    }
	    else if ((since_t>=since) && (till_t<=till))
	    {
		query << "DELETE FROM " OBJECT_KEY_TBL_HEAD_N << folderId << " WHERE obj_id=" << objId;
		query << " AND since_t=" << since_t << " AND till_t=" << till_t;
		execute(query);
	    }
	    else if ((since_t<till) && (till_t>till) && (since_t>=since))
	    {
		query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET since_t=" << till << " WHERE obj_id=" << objId;
		query << " AND since_t=" << since_t << " AND till_t=" << till_t;
		execute(query); 
	    }
	    else if ((since_t<since) && (till_t>till))
	    {
		query << "UPDATE " OBJECT_KEY_TBL_HEAD_N << folderId << " SET till_t=" << since << " WHERE obj_id=" << objId;
		query << " AND since_t=" << since_t << " AND till_t=" << till_t;
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

// New functions

void MySqlObjectMgr::browseId (int folderId, int tagId, ICondDBTimePath& path)
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

void MySqlObjectMgr::tagId (int folderId, int tagId, const ICondDBTimePath& path)
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

//
// Object specific tagging and untagging
//

/**
 * Tag with the 'newTagId' the objects within this folder that belong to the
 * head or have been previously tagged with the tag identified by 'oldTagId'.
 * This method must be used in conjunction with the method 
 * of the MySqlFolderMgr with the same name.
 */
void MySqlObjectMgr::addTag(int folderId, int newTagId, int oldTagId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    // Determine which objects need to be tagged
    if ( oldTagId==0 ) { // the object at head will be tagged
	query << "SELECT obj_id, since_t, till_t FROM " OBJECT_KEY_TBL_HEAD_N << folderId;
    }
    else { // the objects tagged with 'oldTagId' will be tagged
	query << "SELECT obj_id FROM " TAG2OBJ_TBL_N << folderId
	      << "\n  WHERE tag_id=" << oldTagId;
    }
    MySqlResult *res = select(query);
    // Now tag the objects
    if ( res->countRows()!=0 )
	//Deleting objects that are allready tagged. If we don't delete them they will be stored in 
	//the tag2obk_tbl_# table again and again....
	query << "DELETE FROM " TAG2OBJ_TBL_N << folderId << " WHERE tag_id = " << newTagId;
	execute(query);
	do {
	    
	    query << "INSERT INTO " TAG2OBJ_TBL_N << folderId 
		  << "\n (tag_id,obj_id, since_t, till_t)\n   VALUES(" 
		  << newTagId << "," << res->getField(0) << "," << res->getCondDBKeyField(1) << "," << res->getCondDBKeyField(2) << ")";
	    execute(query);
	} while (res->nextRow());

    delete res;
}

/**
 * Remove the tag identified by 'tagId' from the objects in this folder.
 * This method must be used in conjunction with the method 
 * of the MySqlFolderMgr with the same name.
 */
void MySqlObjectMgr::removeTag(int folderId, int tagId)
    throw(CondDBException)
{
    MYSQLSTREAM query;
    query << "DELETE FROM " TAG2OBJ_TBL_N << folderId
	  << "\n  WHERE tag_id=" << tagId;
    execute(query);
}

//
// Connection management
//
void MySqlObjectMgr::changeDatabase(const string& srvname,
				    const string& username,
				    const string& password,
				    const string& dbname,
				    int dbId)
    throw(CondDBException)
{
    connect(srvname, username, password);

    if(changeDB(dbname)==false)
	THROWEX("MySqlObjectMgr::changeDatabase(...) failed",0);
    databaseId = dbId;
}


// THE END





