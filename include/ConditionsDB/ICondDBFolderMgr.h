/* $Id: ICondDBFolderMgr.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef ICondDBFolderMgr_h
#define ICondDBFolderMgr_h

#include "ConditionsDB/ICondDBFolder.h"
#include "ConditionsDB/ICondDBBasicFolderMgr.h"
#include "ConditionsDB/ICondDBTable.h"

/**
 * Extended interface to manage the CondDBFolder and CondDBTable structures.
 */

class ICondDBFolderMgr : virtual public ICondDBBasicFolderMgr {

public:

  /// Destructor.
  virtual ~ICondDBFolderMgr() {}

  // Creators

  /**
   * Create a new CondFolderSet with the specified path and name.
   *
   * Parameters:
   *   fullPathName   : the full path and name of the CondFolderSet
   *   attributes     : system name of an existing objectivity database, where the CondData
   *                    for contained CondFolder can be stored
   *   description    : a generic string; it is stored as such and not used.
   *   parents        : make any missing parent CondFolderSet
   *
   * Example: createCondDBFolderSet("/compass/calo/aaaa/bbb",
   *                                "bbbDB")
   *   will create a new CondFolderSet "bbb" within "/compass/calo/aaaa/" and by default
   *   all contained CondFolder will be stored (the CondData) in the database
   *   with system name = "bbbDB"
   *
   * NOTES:
   * - if attributes is not specified it will be inherited by the parentFolderSet
   * - an objectivity database with system name as specified in attributes must
   *   be previously created with the objectivity tools
   * - if attributes is not specified for the root FolderSet, the database
   *   "conditionsDBGeneral" will be used
   * - if parents=true any missing parent CondFolderSet will be created, with
   *   the same attributes and description
   *   If parents=false and the parent CondFolderSet does not exist, an
   *   exception will be thrown.
   * - the root FolderSet is "/"; this can either be created explicitly,
   *   or implicitly created by the system at the time of the first created
   *   FolderSet (using the same attributes)
   * - double "/" are ignored, e.g. "/a///b/" is equivalent to "/a/b"; but are
   *   best avoided
   */
    virtual void createCondDBFolderSet( const std::string& fullPathName,
  				      std::string  attributes,
  				      std::string  description,
  				      bool         parents,
  				      int ddtype )
     = 0;

  // Redeclare ICondDBBasicFolderMgr method to prevent it from being "hidden"
  virtual void createCondDBFolderSet( const std::string& fullPathName,
				      std::string  attributes  = "",
				      std::string  description = "",
				      bool         parents     = false)
     = 0;

  /**
   * Create folder for structures
   */

  virtual void createCondDBFolder( const std::string& fullPathName,
				   ICondDBTable *table,
				   std::string  attributes  = "",
				   std::string  description = "",
				   bool    parents = false,
				   ICondDBFolder::folder_types ftype = ICondDBFolder::STRUCT)
    //ICondDBExtendedFolder::folder_types ftype = ICondDBExtendedFolder::STRUCT)
       = 0;

  // Redeclare ICondDBBasicFolderMgr method to prevent it from being "hidden"
  virtual void createCondDBFolder( const std::string& fullPathName,
  				   std::string  attributes  = "",
  				   std::string  description = "",
  				   bool         parents     = false)
    = 0;

  /**
   * Returns the folder type
   */

  virtual int getFolderType( const string& fullPathName )
     = 0;

};

#endif // ICondDBFolderMgr_h
