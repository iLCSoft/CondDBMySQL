/* $Id: ICondDBMgr.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef ICondDBMgr_h
#define ICondDBMgr_h


#include "ConditionsDB/CondDBException.h"
#include "ConditionsDB/ICondDBBasicDataAccess.h"
#include "ConditionsDB/ICondDBBasicFolderMgr.h"
#include "ConditionsDB/ICondDBLogMgr.h"
#include "ConditionsDB/ICondDBTagMgr.h"

// Forward declaration
class ICondDBDataAccess;
class ICondDBFolderMgr;

/**
 * Interface for managing the conditions database as a whole
 * (open, close, transaction management).
 * Many instancies of this interface can be present within
 * a single application.
 */

class ICondDBMgr {

public:

  virtual ~ICondDBMgr() {}

  /**
   * Initialize ICondDBMgr.
   *
   * This method must always be invoked at first (no start transaction has to
   * be invoked before).
   *
   * databaseInfo is an optional string; you can pass in this string the following
   * optional information: databaseInfo="bootFilePath,generalDBName"
   * - bootFilePath:
   *
   *   use the same format as in objectivity: host::path (note: path includes the file name);
   *   if databaseInfo is not passed, the value of the OO_FD_BOOT environment variable will be used;
   *   if host is not passed, local host will be used
   * - generalDBName:
   *
   *   name of the database general to be used by the ConditionsDB; a user can create
   *   this database beforehand, and the ConditionsDB will use it. If a database with
   *   this name does not exist, the ConditionsDB will create it when the method
   *   createCondDB will be invoked.
   *   If such name is not passed, a default name "conditionsDBGeneral" will be used.
   *
   * NOTE: is a single parameter is passed in databaseInfo, this will be assumed
   * to be the bootFilePath. If you want to pass only the generalDBName pass a string
   * like databaseInfo=",generalDBName" .
   */
  virtual void init(std::string databaseInfo = "")
     = 0;

  /** Create the structures required by the conditions DB (e.g database "conditionsDBGeneral")
   *
   * This method must be invoked after the init, and after a startUpdate. It will
   * be invoked only once, at the beginning.
   *
   * condDBInfo = "host::path" where the database "conditionsDBGeneral" has to be created
   * by the conditionsDB. (note: path includes the file name)
   * - if condDBInfo is not passed, the database will be created where
   *   the bootFile resides.
   * - if host is not passed (but only the path), the host of the bootFile will be used
   */
  virtual void createCondDB(std::string condDBInfo = "")
     = 0;

  /** Tell whether the method createCondDB has already
   * been successfully invoked.
   * This method must be invoked after the init.
   */
  virtual bool isCondDBcreated() const
     = 0;

  /** Open the conditions DB.
   *
   * This method must be invoked after the init, and after a startRead.
   * It assumes that the method createCondDB has been invoked already.
   * No objects will be created in this method, but it must be invoked
   * in order to set up the ConditionsDB.
   */
  virtual void openDatabase()
     = 0;

  /// Start an update transaction.
  virtual void startUpdate()
     = 0;

  /// Start a read transaction.
  virtual void  startRead()
     = 0;

  /// Abort the current transaction.
  virtual void  abort()
     = 0;

  /// Commit the current transaction.
  virtual void  commit()
     = 0;

  /// Obtain an ICondDBBasicDataAccess interface.
  virtual ICondDBBasicDataAccess* getCondDBBasicDataAccess()
     = 0;

  /// Obtain an ICondDBDataAccess interface.
  virtual ICondDBDataAccess* getCondDBDataAccess()
     = 0;

  /// Obtain an ICondDBBasicFolderMgr interface.
  virtual ICondDBBasicFolderMgr* getCondDBBasicFolderMgr()
     = 0;

  /// Obtain an ICondDBFolderMgr interface.
  virtual ICondDBFolderMgr* getCondDBFolderMgr()
     = 0;

  /// Obtain an ICondDBTagMgr interface.
  virtual ICondDBTagMgr* getCondDBTagMgr()
     = 0;

  /* Not yet implemented
     /// Obtain an ICondDBLogMgr interface.
  virtual ICondDBLogMgr* getCondDBLogMgr()
     = 0;
  */
};

#endif // ICondDBMgr_h
