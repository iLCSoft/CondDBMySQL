#include "CondDBPHP.h"

/*debug*/
#include <iostream>

/* True global resources - no need for thread safety here */
static int conddb_rsrc_id; /*condb handler identifier*/
static int manager_rsrc_id; /*manager handler identifier*/
static int dataAccess_rsrc_id; /*dataAccess handler identifier*/
static int dataIterator_rsrc_id; /*dataIterator handler identifier*/

/* compiled function list so Zend knows what's in this module */
 zend_function_entry CondDBPHP_functions[] = {
  PHP_FE(cdb_connect_open, NULL)
  PHP_FE(cdb_get_folders, NULL)
  PHP_FE(cdb_get_tags, NULL)
  PHP_FE(cdb_get_foldersets, NULL)
  PHP_FE(cdb_get_folder, NULL)
  PHP_FE(cdb_get_obj,	NULL)
  PHP_FE(cdb_browse_obj_history, NULL)
  PHP_FE(cdb_browse_tbl_history, NULL)
  PHP_FE(cdb_browse_all_objs, NULL)
  PHP_FE(cdb_browse_obj_tag, NULL)
  PHP_FE(cdb_browse_tbl_tag, NULL)
  PHP_FE(cdb_browse_obj_point, NULL)
  PHP_FE(cdb_browse_tbl_point, NULL)
  PHP_FE(cdb_tbl_schema, NULL)
  PHP_FE(cdb_print_all_obj, NULL)
  PHP_FE(cdb_connect_close, NULL)
  PHP_FE(cdb_get_folder_type, NULL)
  {NULL, NULL, NULL}
};

/* compiled module information */
zend_module_entry CondDBPHP_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  "CondDBPHP",
  CondDBPHP_functions,
  PHP_MINIT(CondDBPHP),
  PHP_MSHUTDOWN(CondDBPHP),
  PHP_RINIT(CondDBPHP),		/* Replace with NULL if there's nothing to do at request start */
  PHP_RSHUTDOWN(CondDBPHP),	/* Replace with NULL if there's nothing to do at request end */
  PHP_MINFO(CondDBPHP), /*place minit here*/
#if ZEND_MODULE_API_NO >= 20010901
  "0.1", /* Replace with version number for your extension */ /*debug*/
#endif
		STANDARD_MODULE_PROPERTIES
	};

extern "C"
{
  /* implement standard "stub" routine to introduce ourselves to Zend */
// debug
//#ifdef COMPILE_DL_CONDDBPHP
  ZEND_GET_MODULE(CondDBPHP)
//#endif
}

/*debug  put auxiliar functions in a new file*/

/*functions for dealing with run/event format*/
unsigned long getRun(CondDBKey m_time)
{ 
  return m_time>>32; 
}

unsigned long getEvent(CondDBKey m_time)
{ 
  return m_time & 0xFFFFFFFF; 
}

CondDBKey toCondDBKey(unsigned long run, unsigned long event) {
  return ( (unsigned long long) run << 32) + event;
}


/*computes an interval and returns valid one.*/
void getInterval(long &from, long &to, int numRows)
{
  //set from
  if (from < 0 || from  > numRows-1) {
    from=0;
  }

  //set to
  if ( from > to || to > numRows-1 ) {
      to= numRows-1;
  }
}


/*transforms object condObject in a zval * object */
/*returns zobject (zval *), or NULL if error */
/*debug instead of null, should throw an exception*/
zval *cond_to_z_object(ICondDBObject* condObject, bool trunc_data= false)
{
  zval *zobject;
  MAKE_STD_ZVAL(zobject);
  std::ostringstream buffer;
  SimpleTime time;

  //init zval as an object
  if (object_init(zobject) == FAILURE)
    return NULL; //debug throw exception here

  //insert object properties

  //since
  time= condObject->validSince();
  buffer << time;
  add_property_string(zobject, "since", (char *)buffer.str().c_str(), 1);
  buffer.str(""); //empty the buffer for later use
  buffer.clear(); //clear fill flag
  //add time/event for time/event display
  add_property_long(zobject, "sinceRun", getRun(time));
  add_property_long(zobject, "sinceEvent", getEvent(time));


  //till
  time= condObject->validTill();
  buffer << time;
  add_property_string(zobject, "till", (char *)buffer.str().c_str(), 1);
  buffer.str(""); //empty the buffer for later use
  buffer.clear(); //clear fill flag
  //add time/event for time/event display
  add_property_long(zobject, "tillRun", getRun(time));
  add_property_long(zobject, "tillEvent", getEvent(time));


  //insertion time
  condObject->insertionTime(time);
  buffer << time;
  add_property_string(zobject, "insTime", (char *)buffer.str().c_str(), 1);

  //layer
  char str[MAX_STRING];
  sprintf(str, "%ld", condObject->layer());
  add_property_string(zobject, "layer", str, 1);

  //description
  string description;
  condObject->description(description);
  sprintf(str, "%s", description.c_str());
  add_property_string(zobject, "description", str, 1);

  //data
  string data;
  condObject->data(data);
  if (trunc_data) { //show only DATA_SAMPLE bytes of data
    if (data.size() > DATA_SAMPLE) { //check if data is smaller than DATA_SAMPLE
      strncpy(str, data.c_str(), DATA_SAMPLE);
      str[DATA_SAMPLE] = '\0';
    }
    else {
      strcpy(str, data.c_str());
      str[data.size()] = '\0';
    }
    add_property_string(zobject, "data", str, 1);
  }
  else { //copy all data
    add_property_string(zobject, "data", (char *)data.c_str(), 1);
  }

  //add some redundant properties to help managing the object in PHP
  time= condObject->validSince();
  add_property_long(zobject, "year", time.year());
  add_property_long(zobject, "month", time.month());
  add_property_long(zobject, "day", time.day());
  add_property_long(zobject, "hour", time.hour());
  add_property_long(zobject, "minute", time.minute());
  add_property_long(zobject, "second", time.second());
  add_property_long(zobject, "nanosec", time.nanosec());
  return zobject;
}

/*transforms a vector<string> in a zval array*/
/*returns zobject (zval *), or NULL if error */
/*debug instead of null, should throw an exception*/
zval *cpp_string_to_z_array(vector<string> &array) {
  zval *zarray;
  MAKE_STD_ZVAL(zarray);
  if (array_init(zarray) == FAILURE) {
    return NULL; //debug throw exception here
  }

  for (unsigned int i=0; i<array.size(); i++) {
       add_next_index_string(zarray, (char *)array[i].c_str(), 1);
  }
  return zarray;
}

/*transforms a vector of ICondDBTable::cdb_types in a zval array*/
/*returns zobject (zval *), or NULL if error */
/*debug instead of null, should throw an exception*/
zval *cdb_types_to_z_array(vector<ICondDBTable::cdb_types> &array) {
  zval *zarray;
  MAKE_STD_ZVAL(zarray);
  if (array_init(zarray) == FAILURE) {
    return NULL; //debug throw exception here
  }

  for (unsigned int i=0; i<array.size(); i++) {
       add_next_index_long(zarray, array[i]);
  }
  return zarray;
}

/*transforms the condTable Schema in a zval * table*/
/*returns zobject (zval *), or NULL if error */
/*debug instead of null, should throw an exception*/
zval *cond_to_z_table_schema(ICondDBTable* condTable)
{
  std::ostringstream buffer;
  SimpleTime time;

  //init zval as an object
  zval *ztable;
  MAKE_STD_ZVAL(ztable);
  if (object_init(ztable) == FAILURE)
    return NULL; //debug throw exception here

  //put number of rows and columns
  int ncolumns= condTable->getNumColumns();
  add_property_long(ztable, "ncolumns", (long)ncolumns);
  int nrows= condTable->getNumRows();
  add_property_long(ztable, "nrows", nrows);

  //get types
  vector<ICondDBTable::cdb_types> condTypes;    //A vector for table types
  condTable->getTypes(condTypes);
  //put types in ztable
  zval *types= cdb_types_to_z_array(condTypes); //debug error not catch
  add_property_zval(ztable, "types", types);

  //get names of the columns
  vector<string> condNames;
  condTable->getNames(condNames);
  //put names in ztable
  zval *names= cpp_string_to_z_array(condNames);
  add_property_zval(ztable, "names", names);

  return ztable;
}


/*transforms object condTable in a zval * table */
/*returns zobject (zval *), or NULL if error */
/*debug instead of null, should throw an exception*/
/*from and to are the interval of the rows to return in the object*/
/*pre: from <= to && to <condTable->getNumRows()*/
zval *cond_to_z_table(ICondDBTable* condTable, long from, long to)
{
  std::ostringstream buffer;
  SimpleTime time;

  //get table schema
  zval *ztable= cond_to_z_table_schema(condTable);
  if (ztable == NULL) /*error ocurred*/ /*debug throw exception here*/
     return NULL;

  //get rows
  zval *zrows;
  MAKE_STD_ZVAL(zrows);
  if (array_init(zrows) == FAILURE) {
    return NULL; //debug throw exception here
  }

  //if table has no rows, return
  if (condTable->getNumRows()==0) {
    //put the rows in the table (array will be empty)
    add_property_zval(ztable, "rows", zrows);
    return ztable;
  }

  //one by one
  vector<ICondDBTable::cdb_types> condTypes;    //A vector for table types
  condTable->getTypes(condTypes);
  for (long i=from; i<= to; i++) {
    zval *zrow;
    MAKE_STD_ZVAL(zrow);
    if (object_init(zrow) == FAILURE)
      return NULL; //debug throw exception here
 
    //row number
    add_property_long(zrow, "row_number", i);

    //since
    buffer.str(""); //empty the buffer for later use
    buffer.clear(); //clear fill flag
    condTable->getSinceTime(i, time);
    buffer << time;
    add_property_string(zrow, "since", (char *)buffer.str().c_str(), 1);
    // run/event information stored in condDBKey
    add_property_long(zrow, "sinceRun", getRun(time));
    add_property_long(zrow, "sinceEvent", getEvent(time));


   //till
    buffer.str(""); //empty the buffer for later use
    buffer.clear(); //clear fill flag
    condTable->getTillTime(i, time);
    buffer << time;
    add_property_string(zrow, "till", (char *)buffer.str().c_str(), 1);
    // run/event information stored in condDBKey
    add_property_long(zrow, "tillRun", getRun(time));
    add_property_long(zrow, "tillEvent", getEvent(time));

    //add values that are kept in the an array called cells
    zval *cells;
    MAKE_STD_ZVAL(cells);
    if (array_init(cells) == FAILURE)
      return NULL; //debug throw exception here

//     put every column cell in row
     vector <string> row;
     condTable->getRow(i, row);
     for (int j=0, ncolumns= condTable->getNumColumns(), arrayIndex=0; j<ncolumns; j++) { //put cells one by one
      
       char *value= (char *)(row[arrayIndex++].c_str());
       switch(condTypes[j]) {

	 //not an array or bool, return strings
       case ICondDBTable::cdbNull :
	 add_next_index_string(cells, value, 1);
	 break;

       case ICondDBTable::cdbInt :
	 if (atoi(value)==NULLINT)
	   add_next_index_string(cells, "NULL", 1);
	 else
	   add_next_index_string(cells, value, 1);
	 break;

       case ICondDBTable::cdbDouble :
	 buffer.str("");
	 buffer.clear();
	 buffer << NULLDOUBLE;
	 if (strcmp(value, buffer.str().c_str())==0)
	   add_next_index_string(cells, "NULL", 1);
	 else
	   add_next_index_string(cells, value, 1);
	 break;

       case ICondDBTable::cdbFloat :
	 buffer.str("");
	 buffer.clear();
	 buffer << NULLFLOAT;
	 if (strcmp(value, buffer.str().c_str())==0)
	   add_next_index_string(cells, "NULL", 1);
	 else
	   add_next_index_string(cells, value, 1);
	 break;

       case ICondDBTable::cdbLongLong :
	 if (atoll(value)==NULLLONG)
	   add_next_index_string(cells, "NULL", 1);
	 else
	   add_next_index_string(cells, value, 1);
	 break;
	 
       case ICondDBTable::cdbString :
	 add_next_index_string(cells, value, 1);
	 break;

       case ICondDBTable::cdbBool :
	 printf("TEMOS Bool\n"); //debug
	 add_next_index_bool(cells, atoi(value));
	 break;

	 //now we treat the arays
       case ICondDBTable::cdbArrayInt :
       case ICondDBTable::cdbArrayFloat :
       case ICondDBTable::cdbArrayString :
       case ICondDBTable::cdbArrayDouble :
       case ICondDBTable::cdbArrayLongLong :
       case ICondDBTable::cdbArrayBool :
	 //reconstruct array from string
	 zval *arrayCell;
	 MAKE_STD_ZVAL(arrayCell);
	 if (array_init(arrayCell) == FAILURE)
	   return NULL; //debug throw exception here

	 for(int k=atoi(value); k>0; k--) {  //first position has the size of the array
	   char *pos_val= (char *)(row[arrayIndex++].c_str());
	     if (condTypes[j]== ICondDBTable::cdbArrayBool) //do we have an array of booleans?
	       add_next_index_bool(arrayCell, atoi(pos_val) );
	     else if (condTypes[j]== ICondDBTable::cdbArrayInt) { //array of ints
	       if (atoi(pos_val)==NULLINT)
		 add_next_index_string(arrayCell, "NULL", 1);
	       else
		 add_next_index_string(arrayCell, pos_val, 1);
	     }
	     else if (condTypes[j]== ICondDBTable::cdbArrayDouble) { //array of doubles
	       buffer.str("");
	       buffer.clear();
	       buffer << NULLDOUBLE;
	       if (strcmp(pos_val, buffer.str().c_str())==0)
		 add_next_index_string(arrayCell, "NULL", 1);
	       else
		 add_next_index_string(arrayCell, pos_val, 1);
	     }
	     else if (condTypes[j]== ICondDBTable::cdbArrayFloat) { //array of floats
	       buffer.str("");
	       buffer.clear();
	       buffer << NULLFLOAT;
	       if (strcmp(pos_val, buffer.str().c_str())==0)
		 add_next_index_string(arrayCell, "NULL", 1);
	       else
		 add_next_index_string(arrayCell, pos_val, 1);
	     }
	     else if (condTypes[j]== ICondDBTable::cdbArrayLongLong) { //array of longs
	       if (atoll(pos_val)==NULLLONG)
		 add_next_index_string(arrayCell, "NULL", 1);
	       else
		 add_next_index_string(arrayCell, pos_val, 1);
	     }
	     else //array of strings
	       add_next_index_string(arrayCell, pos_val, 1);
	   }
	   add_next_index_zval(cells, arrayCell);
	   break;

       default:
	 return NULL; //debug throw exception here
       }
     }

    //add cells property to current row
    add_property_zval(zrow, "cells", cells);
    
    //add current row to the rows property of ztable
    add_next_index_zval(zrows, zrow);
  }

  //put the rows in the table
  add_property_zval(ztable, "rows", zrows);
  return ztable;
}



/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(CondDBPHP)
{
  /* If you have INI entries, uncomment these lines 
        ZEND_INIT_MODULE_GLOBALS(CondDBPHP, php_CondDBPHP_init_globals, NULL);
        REGISTER_INI_ENTRIES();
  */
  
  // register conddb resource (handler)
  conddb_rsrc_id= zend_register_list_destructors_ex( conddb_destruction_handler
						, NULL, CONDDB_HAND_NAME, 
						module_number);
  return SUCCESS;
}
/* }}} */

/* destruction handler for conddb */
void conddb_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC) {

  conddb_ptr resource;
  resource= (conddb_ptr) rsrc->ptr;

  //we do not need to destroy:
  //resource->dataAccess
  //resource->fol_mgr
  //resource->tag_mgr

  if (resource->iterator!=NULL)
    delete resource->iterator;
  
  if (resource->manager != NULL) {
    	CondDBMySQLMgrFactory::destroyCondDBMgr(resource->manager);
  }

}

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(CondDBPHP)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(CondDBPHP)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(CondDBPHP)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(CondDBPHP)
{
// 	php_info_print_table_start();
// 	php_info_print_table_header(2, "CondDBPHP support", "enabled");
// 	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* implement function that is meant to be made available to PHP */

/* {{{ proto int cdb_connect_open(string host, string dbname, string user, string pass)
   open connection to the database*/
PHP_FUNCTION(cdb_connect_open)
{
  char *host = NULL;
  char *dbname = NULL;
  char *user = NULL;
  char *pass = NULL;
  int argc = ZEND_NUM_ARGS();
  int manager_id = -1;
  int host_len;
  int dbname_len;
  int user_len;
  int pass_len;

  if (zend_parse_parameters(argc TSRMLS_CC, "ssss", &host, &host_len, &dbname, &dbname_len, &user, &user_len, &pass, &pass_len
			    ) == FAILURE) 
    return;

  int rscid;
  string dbini=string(host)+":"+string(dbname)+":"+string(user) +":"+string(pass);
  conddb_ptr resource= (conddb_ptr)malloc(sizeof(conddb));

  /*get manager*/
  resource->manager= CondDBMySQLMgrFactory::createCondDBMgr();

  try {
    resource->manager->init(dbini); /*init database*/
    resource->manager->openDatabase(); /*connect to database*/
  }
  catch(CondDBException &e) {
    CondDBMySQLMgrFactory::destroyCondDBMgr( resource->manager );
    free(resource);
    RETURN_LONG(-1);
  }

  resource->dataAccess= resource->manager->getCondDBDataAccess(); /*get dataAccess*/

  /*set default values for the rest of the handler's attributes*/
  resource->fol_mgr=NULL;
  resource->tag_mgr=NULL;
  resource->iterator= NULL;
  /*register resource and return identifier*/
  rscid= ZEND_REGISTER_RESOURCE(return_value, resource, conddb_rsrc_id);
  RETVAL_RESOURCE(rscid);
}

/* {{{ proto array cdb_get_folders(resource manager, string folderset)
   gets all folders inside a giver folderset. Returns array of strings */
PHP_FUNCTION(cdb_get_folders)
{
	char *folderset = NULL;
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	int folderset_len;
	zval *manager = NULL;
	conddb_ptr resource;
	array_init(return_value);

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &manager, &folderset, &folderset_len) == FAILURE) 
		return;

	if (manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &manager, manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	if (resource->fol_mgr==NULL) { //folderManager not set yet
	  resource->fol_mgr = resource->manager->getCondDBFolderMgr(); /*set it*/
	}

	try {
	  ICondDBFolderSet *parent_folder;
	  resource->fol_mgr->getCondDBFolderSet(folderset, parent_folder); //get folder from path
	  vector <string> sub_folders;
	  parent_folder->getContainedFolder(sub_folders);

	  for (unsigned i=0; i<sub_folders.size(); i++) {
	    add_next_index_string( return_value, (char *)sub_folders[i].c_str(), 1);
	  }
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}
}
/* }}} */

/* {{{ proto array cdb_get_foldersets(resource manager, string folderset)
   gets all foldersets inside a giver folderset */
PHP_FUNCTION(cdb_get_foldersets)
{
	char *folderset = NULL;
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	int folderset_len;
	zval *manager = NULL;
	conddb_ptr resource;
	array_init(return_value);

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &manager, &folderset, &folderset_len) == FAILURE) 
		return;

	if (manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &manager, manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	if (resource->fol_mgr==NULL) { //folderManager not set yet
	  resource->fol_mgr = resource->manager->getCondDBFolderMgr(); /*set it*/
	}

	try {
	  ICondDBFolderSet *parent_folder;
	  resource->fol_mgr->getCondDBFolderSet(folderset, parent_folder); //get folder from path
	  vector <string> sub_folders;
	  parent_folder->getContainedFolderSet(sub_folders);

	  for (unsigned i=0; i<sub_folders.size(); i++) {
	    add_next_index_string( return_value, (char *)sub_folders[i].c_str(), 1);
	  }
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}
}
/* }}} */

/* {{{ proto object cdb_get_folder(resource fld_manager, string path)
   get folder object*/
PHP_FUNCTION(cdb_get_folder)
{
	char *path = NULL;
	int argc = ZEND_NUM_ARGS();
	int fld_manager_id = -1;
	int path_len;
	zval *fld_manager = NULL;
	conddb_ptr resource;
	object_init(return_value);

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &fld_manager, &path, &path_len) == FAILURE) 
		return;

	if (fld_manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &fld_manager, fld_manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	if (resource->fol_mgr==NULL) { //folderManager not set yet
	  resource->fol_mgr = resource->manager->getCondDBFolderMgr(); /*set it*/
	}

	try {
	  ICondDBFolder *cdb_folder;
	  resource->fol_mgr->getCondDBFolder(path, cdb_folder); //get folder

	  /*add path property*/
	  add_property_string(return_value, "path", path, true);

	  /*add description*/
	  string tmp_str= cdb_folder->getDescription();
	  if (tmp_str.size() == 0) //no description
	    add_property_string(return_value, "description", "none", true);
	  else //save description
	    add_property_string(return_value, "description", (char *)tmp_str.c_str(), true);

	  //add atributes
	  tmp_str= cdb_folder->getAttributes();
	  if (tmp_str.size() == 0) //no description
	    add_property_string(return_value, "attributes", "none", true);
	  else //save description
	    add_property_string(return_value, "attributes", (char *)tmp_str.c_str(), true);

	  //add type
	  add_property_long(return_value, "type", resource->fol_mgr->getFolderType(path));
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}
}
/* }}} */


/* {{{ proto object cdb_get_tags(resource tag_manager, string folder)
   get folder object*/
PHP_FUNCTION(cdb_get_tags)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();
	int tag_manager_id = -1;
	int folder_len;
	zval *tag_manager = NULL;
	conddb_ptr resource;
	array_init(return_value);

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &tag_manager, &folder, &folder_len) == FAILURE) 
		return;

	if (tag_manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &tag_manager, tag_manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	if (resource->tag_mgr==NULL) { //tagManager not set yet
	  resource->tag_mgr = resource->manager->getCondDBTagMgr(); /*set it*/
	}

	vector <string> tags;
	try {
	  resource->tag_mgr->getCondDBTag(folder, tags); //get tags from folder
	  for (unsigned i=0; i<tags.size(); i++) {
	    add_next_index_string( return_value, (char *)tags[i].c_str(), 1);
	  }
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}
}
/* }}} */


/* {{{ proto int cdb_get_folder_type(resource manager, string folder)
   returns int meaning the type of the folder */
PHP_FUNCTION(cdb_get_folder_type)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	int folder_len;
	zval *manager = NULL;
	conddb_ptr resource;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &manager, &folder, &folder_len) == FAILURE) 
		return;

	if (manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &manager, manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	if (resource->fol_mgr==NULL) { //folderManager not set yet
	  resource->fol_mgr = resource->manager->getCondDBFolderMgr(); /*set it*/
	}

	try {
	  RETURN_LONG(resource->fol_mgr->getFolderType(folder)); //get folder type
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}
}
/* }}} */

/* {{{ proto object cdb_get_obj(resource dataAccess, string folder, int year, int month, int day, int hour, int min, int sec, int nanosec [, string tag])
   //get object from conditions */
/*debug not tested*/
PHP_FUNCTION(cdb_get_obj)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();
	int dataAccess_id = -1;
	int folder_len;
	long year;
	long month;
	long day;
	long hour;
	long min;
	long sec;
	long nanosec;
	char *tag;
	zval *dataAccess = NULL;
	conddb_ptr resource;

	/*retrieve function arguments*/
	/* see if it satisfies our minimal request (2 arguments) */
	/* and our maximal acceptance (4 arguments) */
	if(argc < 9 || argc > 10)
	  WRONG_PARAM_COUNT;

	//get the first 8 arguments
	if (zend_parse_parameters(9, "rslllllll", &dataAccess, &folder, &folder_len,
				  &year, &month, &day, &hour, &min, &sec, &nanosec) == FAILURE)
		return;

	//get optional parameter
	if (argc == 10) {
	  zval **parameter_array[argc];
	  zend_get_parameters_array_ex(argc, parameter_array); //this function will never fail
	  convert_to_string_ex(parameter_array[argc-1]); //make sure last argument becomes a string
	  tag= Z_STRVAL(**parameter_array[argc-1]);
	}
	else 
	  tag=DEFAULT_TAG;

	//fetch ressource
	if (dataAccess) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &dataAccess, dataAccess_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	/*get the object*/
	try {
	  ICondDBObject* condObject = NULL;
	  /*debug*/
// 	  printf("Folder: %s, year: %li, day %li, month: %li, hour: %li, min: %li, sec: %li, nanosec: %li, tag: %s\n", 
// 		 folder, year, month, day, hour, min, sec, nanosec, tag);
	  /*debug, no tag for now*/
	  resource->dataAccess->findCondDBObject(condObject, folder, 
						 SimpleTime(year, month, day, hour, min, sec, nanosec)/*, tag*/);

	  /*return the object*/
	  if (condObject!=NULL) { //found any object?
	    *return_value= *cond_to_z_object(condObject);
	    delete condObject;
	  }
	  else
	    RETURN_NULL();
	}
	catch(CondDBException &e) {
	  RETURN_LONG(-1);
	}
}
/* }}} */

/* {{{ proto int cdb_browse_all_objs(resource manager, string folder)
   puts iterator for all objects in resource */
PHP_FUNCTION(cdb_browse_all_objs)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	int folder_len;
	zval *manager = NULL;
	conddb_ptr resource;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &manager, &folder, &folder_len) == FAILURE) 
		return;

	if (manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &manager, manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	try {
	  if (resource->iterator != NULL) {
	    /*debug libertar iterador*/
	    resource->iterator= NULL;
	  }
	  resource->dataAccess->browseAllObjects(resource->iterator, folder);
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}
}
/* }}} */

/* {{{ proto int cdb_browse_obj_tag(resource manager, string folder, string tag)
   puts tag iterator in the resource */
PHP_FUNCTION(cdb_browse_obj_tag)
{
	char *folder = NULL;
	char *tag = NULL;
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	int folder_len;
	int tag_len;
	zval *manager = NULL;
	conddb_ptr resource;

	if (zend_parse_parameters(argc TSRMLS_CC, "rss", &manager, &folder, &folder_len, &tag, &tag_len) == FAILURE) 
		return;

	if (manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &manager, manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}
	try {
	  if (resource->iterator != NULL) {
	    /*debug libertar iterador*/
	    resource->iterator= NULL;
	  }
	  resource->dataAccess->browseObjectsInTag(resource->iterator, folder, tag);
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto int cdb_browse_tbl_tag(resource dataAccess, string folder, string tag [, int from, int to [, string where]])
   returns a table browsed with the given tag */
PHP_FUNCTION(cdb_browse_tbl_tag)
{
	char *folder = NULL;
	char *tag = NULL;
	int argc = ZEND_NUM_ARGS();
	int dataAccess_id = -1;
	int folder_len;
	int tag_len;
	zval *dataAccess = NULL;
	conddb_ptr resource;

	/*retrieve function arguments*/
	/* see if it satisfies our minimal request (3 arguments) */
	/* or has 'from' and 'to' */
	/* and after that may have 'where' */

	if(! (argc == 3 || argc == 5 || argc==6))
	  WRONG_PARAM_COUNT;

	//get the first 3 arguments
	if (zend_parse_parameters(3, "rss", &dataAccess, &folder, &folder_len, &tag, &tag_len) == FAILURE) 
		return;

	if (dataAccess) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &dataAccess, dataAccess_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	  ICondDBTable *condTable;
	  condTable= new CondDBTable();
	  long from, to;

	try {
	  if (argc >= 5) { //get optional parameters, 'from', 'to' and where
	    zval **parameter_array[argc]; //create an array
	    zend_get_parameters_array_ex(argc, parameter_array); //get args to array
	    convert_to_long_ex(parameter_array[3]); //cast 'from' to integer
	    from= Z_LVAL(**parameter_array[3]); //get 'from' from zval
	    convert_to_long_ex(parameter_array[4]); //cast 'to' to integer
	    to= Z_LVAL(**parameter_array[4]); //get 'to' from zval
	    

	    if (argc == 6) { //get optional parameter, "where" clause
	      convert_to_string_ex(parameter_array[argc-1]); //cast 'where' to string
	      char *where_clause= Z_STRVAL(**parameter_array[argc-1]); //get 'where' from zval
	      resource->dataAccess->browseObjectsInTag(condTable, folder, tag, where_clause);
	    }
	    else { //no 'where' clause, force it to show all rows
	      resource->dataAccess->browseObjectsInTag(condTable, folder, tag);
	    }
	    //validate [from, to]
	    getInterval(from, to, condTable->getNumRows());
	  }
	  else { //no 'from', 'to', and 'where'
	    //get data from conditions
	    resource->dataAccess->browseObjectsInTag(condTable, folder, tag);
	    /*set from and to get full table*/
	    from=0;
	    to=condTable->getNumRows()-1;
	  }
	}
	catch(CondDBException &e) {
	  delete condTable;
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}

	//put table into a return value
	*return_value= *cond_to_z_table(condTable, from, to);
	delete condTable;
}
/* }}} */

/* {{{ proto object cdb_tbl_schema(resource dataAccess, string folder)
   returns a table containing the schema */
PHP_FUNCTION(cdb_tbl_schema)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();

	int dataAccess_id = -1;
	int folder_len;
	zval *dataAccess = NULL;
	conddb_ptr resource;

	if (zend_parse_parameters(argc TSRMLS_CC, "rs", &dataAccess, &folder, &folder_len) == FAILURE) 
		return;

	if (dataAccess) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &dataAccess, dataAccess_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	ICondDBTable *condTable;
	condTable= new CondDBTable();
	try {
	  resource->dataAccess->getTableSchema(folder, condTable);
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  delete condTable;
	  RETURN_LONG(-1);
	}
	*return_value= *cond_to_z_table_schema(condTable);
	delete condTable;
}
/* }}} */



/* {{{ proto int cdb_browse_obj_point(resource manager, string folder, int year, int month, int day, int hour, int min, int sec)
   puts point iterator in the resource */
PHP_FUNCTION(cdb_browse_obj_point)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	int folder_len;
	long year;
	long month;
	long day;
	long hour;
	long min;
	long sec;
	zval *manager = NULL;
	conddb_ptr resource;


	if (zend_parse_parameters(argc TSRMLS_CC, "rsllllll", &manager, &folder, &folder_len, &year, &month, &day, &hour, &min, &sec) == FAILURE) 
		return;

	if (manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &manager, manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	if (resource->iterator != NULL) {
	  /*debug libertar iterador*/
	  resource->iterator= NULL;
	}
	SimpleTime point = SimpleTime(year, month, day, hour, min, sec);
	
	try {
	  resource->dataAccess->browseObjectsAtPoint(resource->iterator, folder, point);
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_NULL();
	}
}
/* }}} */


/* {{{ proto int cdb_browse_tbl_point(resource dataAccess, string folder, int year, int month, int day, int hour, int min, int sec[int from, int to [, string where]])
   returns a table browsed at the given point */
PHP_FUNCTION(cdb_browse_tbl_point)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();
	int dataAccess_id = -1;
	int folder_len;
	long year;
	long month;
	long day;
	long hour;
	long min;
	long sec;
	zval *dataAccess = NULL;
	conddb_ptr resource;

	/*retrieve function arguments*/
	/* see if it satisfies our minimal request (8 arguments) */
	/* or has 'from' and 'to' */
	/* and after that may have 'where' */
	if(! (argc == 8 || argc == 10 || argc == 11))
	  WRONG_PARAM_COUNT;


	if (zend_parse_parameters(8, "rsllllll", &dataAccess, &folder, &folder_len,
				  &year, &month, &day, &hour, &min, &sec) == FAILURE) 
	  return;

	if (dataAccess)
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &dataAccess, dataAccess_id, CONDDB_HAND_NAME, conddb_rsrc_id);

	ICondDBTable *condTable;
	condTable= new CondDBTable();
	SimpleTime point = SimpleTime(year, month, day, hour, min, sec);
	long from, to; /*values to limit number of rows to return*/

	try {
	  if (argc >= 10) { //get optional parameters, 'from', 'to' and where
	    zval **parameter_array[argc]; //create an array
	    zend_get_parameters_array_ex(argc, parameter_array); //get args to array
	    convert_to_long_ex(parameter_array[8]); //cast 'from' to integer
	    from= Z_LVAL(**parameter_array[8]); //get 'from' from zval
	    convert_to_long_ex(parameter_array[9]); //cast 'to' to integer
	    to= Z_LVAL(**parameter_array[9]); //get 'to' from zval
	    
 	    if (argc == 11) { //get optional parameter, "where" clause
 	      convert_to_string_ex(parameter_array[argc-1]); //cast 'where' to string
 	      char *where_clause= Z_STRVAL(**parameter_array[argc-1]); //get 'where' from zval
	      resource->dataAccess->browseObjectsAtPoint(condTable, folder, point, where_clause);
 	    }
	    else { //no 'where' clause, but [from, to]
	      resource->dataAccess->browseObjectsAtPoint(condTable, folder, point);
	    }
	    //validate [from, to]
	    getInterval(from, to, condTable->getNumRows());
	  }
	  else { //no 'from', 'to', and 'where'
	    //get data from conditions
	    resource->dataAccess->browseObjectsAtPoint(condTable, folder, point);
	    /*set from and to get full table*/
	    from=0;
	    to=condTable->getNumRows()-1;
	  }
	}
	catch(CondDBException &e) {
	  delete condTable;
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}
	
	//put table into a return value
	*return_value= *cond_to_z_table(condTable, from, to);
	//free table and return
	delete condTable;
}
/* }}} */


/* {{{ proto int cdb_browse_obj_history(resource manager, string folder, int since_year, int since_month, int since_day, int since_hour, int since_min, int since_sec, int till_year, int till_month, int till_day, int till_hour, int till_min, int till_second)
   puts history iterator in the resource */
PHP_FUNCTION(cdb_browse_obj_history)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	int folder_len;
	long since_year;
	long since_month;
	long since_day;
	long since_hour;
	long since_min;
	long since_sec;
	long till_year;
	long till_month;
	long till_day;
	long till_hour;
	long till_min;
	long till_sec;
	zval *manager = NULL;
	conddb_ptr resource;

	if (zend_parse_parameters(argc TSRMLS_CC, "rsllllllllllll", &manager, &folder, &folder_len, &since_year, &since_month, &since_day, &since_hour, &since_min, &since_sec, &till_year, &till_month, &till_day, &till_hour, &till_min, &till_sec) == FAILURE) 
		return;

	if (manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &manager, manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	if (resource->iterator != NULL) {
	  resource->iterator= NULL;
	}

	SimpleTime since = SimpleTime(since_year, since_month, since_day, since_hour, since_min, since_sec);
	SimpleTime till = SimpleTime(till_year, till_month, till_day, till_hour, till_min, till_sec);

	try {
	  resource->dataAccess->browseHistory(resource->iterator, folder, since, till);
	}
	catch(CondDBException &e) {
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_NULL();
	}
}
/* }}} */


/* {{{ proto int cdb_browse_tbl_history(resource dataAccess, string folder, int since_year, int since_month, int since_day, int since_hour, int since_min, int since_sec, int till_year, int till_month, int till_day, int till_hour, int till_min, int till_second [, int from, int to[, string where]])
   returns a table browsed with history option */
PHP_FUNCTION(cdb_browse_tbl_history)
{
	char *folder = NULL;
	int argc = ZEND_NUM_ARGS();
	int dataAccess_id = -1;
	int folder_len;
	long since_year;
	long since_month;
	long since_day;
	long since_hour;
	long since_min;
	long since_sec;
	long till_year;
	long till_month;
	long till_day;
	long till_hour;
	long till_min;
	long till_sec;
	zval *dataAccess = NULL;
	conddb_ptr resource;

	/*retrieve function arguments*/
	/* see if it satisfies our minimal request (14 arguments) */
	/* or has 'from' and 'to' */
	/* and after that may have 'where' */
	if(! (argc == 14 || argc == 16 || argc==17))
	  WRONG_PARAM_COUNT;

	if (zend_parse_parameters(14, "rsllllllllllll", &dataAccess, &folder, &folder_len, 
				  &since_year, &since_month, &since_day, &since_hour, 
				  &since_min, &since_sec, &till_year, &till_month, &till_day, 
				  &till_hour, &till_min, &till_sec) == FAILURE) 
		return;

	if (dataAccess) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &dataAccess, dataAccess_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	ICondDBTable *condTable;
	condTable= new CondDBTable();
	SimpleTime since = SimpleTime(since_year, since_month, since_day, since_hour, since_min, since_sec);
	SimpleTime till = SimpleTime(till_year, till_month, till_day, till_hour, till_min, till_sec);

	long from, to;

	try {
	  if (argc >= 16) { //get optional parameters, 'from', 'to' and where
	    zval **parameter_array[argc]; //create an array
	    zend_get_parameters_array_ex(argc, parameter_array); //get args to array
	    convert_to_long_ex(parameter_array[14]); //cast 'from' to integer
	    from= Z_LVAL(**parameter_array[14]); //get 'from' from zval
	    convert_to_long_ex(parameter_array[15]); //cast 'to' to integer
	    to= Z_LVAL(**parameter_array[15]); //get 'to' from zval
	    
	    if (argc == 17) { //get optional parameter, "where" clause
	      convert_to_string_ex(parameter_array[argc-1]); //cast 'where' to string
	      char *where_clause= Z_STRVAL(**parameter_array[argc-1]); //get 'where' from zval
	      resource->dataAccess->browseHistory(folder, condTable, since, till, "", where_clause);
	    }
	    else { //no 'where' clause, but [from, to]
	      resource->dataAccess->browseHistory(folder, condTable, since, till, "");
	    }
	    //validate [from, to]
	    getInterval(from, to, condTable->getNumRows());
	  }
	  else { //no 'from', 'to', and 'where'
	    //get data from conditions	    
	    resource->dataAccess->browseHistory(folder, condTable, since, till);
	    /*set from and to get full table*/
	    from=0;
	    to=condTable->getNumRows()-1;
	  }
	}
	catch(CondDBException &e) {
	  delete condTable;
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}

	//put table into a return value
	*return_value= *cond_to_z_table(condTable, from, to);
	//free table and return
	delete condTable;
}
/* }}} */

/* {{{ proto array cdb_print_all_obj(resource manager)
   returns array of objects, using iterator on resource */
PHP_FUNCTION(cdb_print_all_obj)
{
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	zval *manager = NULL;
	conddb_ptr resource;
	array_init(return_value); //array to return

	if (zend_parse_parameters(argc TSRMLS_CC, "r", &manager) == FAILURE) 
	  return;

	if (manager) {
	  ZEND_FETCH_RESOURCE(resource, conddb_ptr, &manager, manager_id, CONDDB_HAND_NAME, conddb_rsrc_id);
	}

	if (resource->iterator == NULL)
	  RETURN_NULL();

	ICondDBObject* cond_object = NULL;
	zval *zobject; //pointer to object to put in return_array

	try {
	    //build array
	    if ((cond_object=resource->iterator->current()) != NULL) {
	      do {
		zobject= cond_to_z_object(cond_object, true);
		if (zobject!=NULL) { //debug cond_to_z_object should throw an exception
		  add_next_index_zval(return_value, zobject);
		  delete cond_object;
		}
		//		resource->iterator->current(); /*debug is this needed???
	      } while( (cond_object=resource->iterator->next()) != NULL );
	    }
	    else
	      RETURN_NULL();
	  }
	catch(CondDBException &e) {
	  delete resource->iterator;
	  resource->iterator= NULL;
	  //	  resource->error= &e; //save exception for error handling
	  RETURN_LONG(-1);
	}
	delete resource->iterator;
	resource->iterator= NULL;
}
/* }}} */



/* {{{ proto array cdb_connect_close(resource manager)
   returns array of objects, using iterator on resource */
ZEND_FUNCTION(cdb_connect_close)
{
	int argc = ZEND_NUM_ARGS();
	int manager_id = -1;
	zval *res = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "z", &res) == FAILURE) 
	  return;

	zend_list_delete( Z_RESVAL_P(res));
}
