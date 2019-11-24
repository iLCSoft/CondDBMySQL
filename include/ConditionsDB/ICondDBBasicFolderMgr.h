/* $Id: ICondDBBasicFolderMgr.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef ICondDBBasicFolderMgr_h
#define ICondDBBasicFolderMgr_h

#include "ConditionsDB/CondDBException.h"
#include "ConditionsDB/ICondDBFolder.h"

#include <vector>
#include <string>

// Forward declaration
class ICondDBFolderMgr;

/**
 * Interface to manage the basic CondDBFolder structure.
 */

class ICondDBBasicFolderMgr {

public:

  /// Destructor.
  virtual ~ICondDBBasicFolderMgr() {}

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
				      std::string  attributes  = "",
				      std::string  description = "",
				      bool         parents     = false)
     = 0;

  /**
   * Create a new CondFolder within the specified path and name.
   *
   * Parameters:
   *   fullPathName   : the full path and name of the CondFolder
   *   attributes     : system name of an existing objectivity database, where the CondData
   *                    for this CondFolder can be stored
   *   description    : a generic string; it is stored as such and not used.
   *   parents        : make any missing parent CondFolderSet
   *
   * Example: createCondDBFolder("/compass/calo/magnField",
   *                             "caloDB")
   *   will create the new CondFolder "magnField" within "/compass/calo/"
   *   and the CondData for it will be stored  in the database with system name = "caloDB"
   *
   * NOTES:
   * - if attributes is not specified it will be inherited by the parentFolderSet
   * - an objectivity database with system name as specified in attributes must
   *   be previously created with the objectivity tools
   * - if parents=true any missing parent CondFolderSet will be created, with
   *   the same attributes and description
   *   If parents=false and the parent CondFolderSet does not exist, an
   *   exception will be thrown.
   * - double "/" are ignored, e.g. "/a///b/" is equivalent to "/a/b"; but are
   *   best avoided
   */
  virtual void createCondDBFolder( const std::string& fullPathName,
				   std::string  attributes  = "",
				   std::string  description = "",
				   bool         parents     = false)
     = 0;


  /// Return true if a CondFolderSet or a CondFolder exists already with the given name
  virtual bool exist( const std::string& fullName ) const
     = 0;

  /**
   * Delete the CondDBFolder or CondDBFolderSet with the specified name.
   *
   * NOTES:
   * - deleting a CondDBFolder that contains CondDBObject will result
   *   in the deletion of all the information stored
   * - in order to delete a CondDBFolderSet it has to be empty; delete
   *   all contained CondDBFolder or CondDBFolderSet beforehand.
   *   This is imposed for safety reasons.
   */
  virtual void deleteFolder( const std::string& fullPathName )
     = 0;


  /**
   * Return the CondDBFolderSet with the specified name.
   *
   * An exception is thrown if no CondDBFolderSet exist with that name.
   *
   * Note: the object pointed by ICondDBFolderSet* has to be deleted by the
   * user at the end of its usage.
   */
  virtual void getCondDBFolderSet( const std::string& fullPathName,
				   ICondDBFolderSet*& folderSet) const
     = 0;

  /**
   * Return the CondDBFolder with the specified name.
   *
   * An exception is thrown if no CondDBFolder exist with that name.
   *
   * NOTES:
   * - the object pointed by ICondDBFolder* has to be deleted by the
   *   user at the end of its usage.
   */
  virtual void getCondDBFolder( const std::string& fullPathName,
				ICondDBFolder*& folder) const
     = 0;


  /**
   * This is the equivalent of the command "ls" on UNIX.
   * fullName will be like "/compass/calo/temperature" and can be a
   * CondFolderSet or a CondFolder.
   * To describe the whole structure (from the root) pass fullName="" or ="/".
   *
   * This will print the properties and/or the contained CondFolderSet/CondFolder
   * according to the options passed. Accepted options:
   *      options = "l"  : provide a long description (including all attributes)
   *      options = "R"  : describe sub CondFolderSet recursively
   *      options = "lR" : provide a long and recursive description
   */
  virtual void describe(const std::string& fullName,
			std::string options = "") const
     = 0;

  /**
   * This methods returns a vector containing the full name of all
   * defined CondFolder
   */
  virtual void getAllCondDBFolder( std::vector<std::string>& allCondFolder ) const
     = 0;

  /**
   * This methods returns a vector containing the full name of all
   * defined CondFolderSet
   */
  virtual void getAllCondDBFolderSet( std::vector<std::string>& allCondFolderSet ) const
     = 0;

  /**
   * This methods returns a vector containing the full name of all
   * CondFolder beneath fullPathName.
   *
   * NOTES:
   * - if fullPathName is a CondDBFolder, folderList will only contain fullPathName
   * - if fullPathName is a CondDBFolderSet, folderList will contain a complete list
   */
  virtual void getAllCondDBFolderBeneath( const std::string& fullPathName,
					  std::vector<std::string>& folderList ) const
     = 0;

};

#endif // ICondDBBasicFolderMgr_h
