#ifndef _H_CONDDBPHP
#define _H_CONDDBPHP

/* extern zend_module_entry CondDBPHP_module_entry; */
#define phpext_CondDBPHP_ptr &CondDBPHP_module_entry

#ifdef PHP_WIN32
#define PHP_CONDDBPHP_API __declspec(dllexport)
#else
#define PHP_CONDDBPHP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* include standard header */
#include "php.h"
#include <zend.h>
/*C++ files*/
#include <sstream>
/*conditions headers*/
#include "ConditionsDB/CondDBMySQLMgrFactory.h"
#include "ConditionsDB/CondDBObjFactory.h"
#include "CondDBTable.h"
/* bind specific files*/
#include "defines.h"

/* #include "CondDBCPP.h" */

/*handler to the conditions database*/
typedef struct _conddb_handler {
  ICondDBMgr *manager;
  ICondDBDataAccess* dataAccess;
  ICondDBDataIterator* iterator;
  ICondDBFolderMgr *fol_mgr;
  ICondDBTagMgr *tag_mgr;
  //  CondDBException *error; /*not used yet*/
} conddb, *conddb_ptr;
#define CONDDB_HAND_NAME "Condtions Database Connection Handler"


/*debug*/
/* void cdbclose(zend_rsrc_list_entry *rsrc TSRMLS_DC); */

/* declaration of standard functions*/
PHP_MINIT_FUNCTION(CondDBPHP);
PHP_MSHUTDOWN_FUNCTION(CondDBPHP);
PHP_RINIT_FUNCTION(CondDBPHP);
PHP_RSHUTDOWN_FUNCTION(CondDBPHP);
PHP_MINFO_FUNCTION(CondDBPHP);
/* declaration of functions to be exported */
ZEND_FUNCTION(cdb_connect_open);
ZEND_FUNCTION(cdb_get_folders);
ZEND_FUNCTION(cdb_get_tags);
ZEND_FUNCTION(cdb_get_foldersets);
ZEND_FUNCTION(cdb_get_folder);
PHP_FUNCTION(cdb_get_obj);
ZEND_FUNCTION(cdb_browse_obj_history);
ZEND_FUNCTION(cdb_browse_tbl_history);
ZEND_FUNCTION(cdb_browse_all_objs);
ZEND_FUNCTION(cdb_browse_obj_tag);
ZEND_FUNCTION(cdb_browse_tbl_tag);
ZEND_FUNCTION(cdb_browse_obj_point);
ZEND_FUNCTION(cdb_browse_tbl_point);
ZEND_FUNCTION(cdb_tbl_schema);
ZEND_FUNCTION(cdb_print_all_obj);
PHP_FUNCTION(cdb_get_folder_type);
PHP_FUNCTION(cdb_get_folder_type);
PHP_FUNCTION(cdb_connect_close);

//destruction handler for the conddb resource
void conddb_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC);

#ifdef ZTS
#define CONDDBPHP_G(v) TSRMG(CondDBPHP_globals_id, CondDBPHP_CondDBPHP_globals *, v)
#else
#define CONDDBPHP_G(v) (CondDBPHP_globals.v)
#endif

#endif
