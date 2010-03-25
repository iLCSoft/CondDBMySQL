// ICondDBFolder.h
//
// $Id: ICondDBFolder.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $
//
#ifndef ICondDBFolder_h
#define ICondDBFolder_h

#include "ConditionsDB/CondDBException.h"

#include <vector>
#include <string>

/**
 * Interface wich allows to get information on a defined Folder or Folderset.
 * This is the interface that provides general information, valid
 * for both Folder and FolderSet.
 */

class ICondDBFolderGeneral {

public:

  /// Virtual destructor
  virtual ~ICondDBFolderGeneral(){};

  /**
   * Returns 
   */  
  virtual std::string getName() const = 0;
  virtual std::string getFullPathName() const = 0;
  virtual std::string getAttributes() const = 0;
  virtual std::string getDescription() const = 0;
};

/**
 * Interface wich allows to get information specific to a Folder.
 */

class ICondDBFolder : public ICondDBFolderGeneral {

public:

  // Folder type according to CondDBMySQL extended API, not used in common API
  // (kept here for simplicity although it belongs logically to CondDBMySQL)
  enum folder_types {BLOBTAG, STRUCT, STRUCTID, BLOB, STRUCTTAG, STRUCTIDTAG};

  /// Virtual destructor
  virtual ~ICondDBFolder(){};

   /**
   * Methods to return information specific to a Folder
   * will be added.
   */  


};

/**
 * Interface wich allows to get information specific to a Folderset.
 */

class ICondDBFolderSet : public ICondDBFolderGeneral {

public:

  /// Virtual destructor
  virtual ~ICondDBFolderSet(){};

  /**
   * Return a vector containing the name of all defined CondFolderSet
   */  
  virtual void
  getContainedFolderSet(std::vector<std::string>& allFolderSet) const = 0;

  /**
   * Return a vector containing the name of all defined CondFolder
   */  
  virtual void
  getContainedFolder(std::vector<std::string>& allFolder) const = 0;
  
};

#endif // ICondDBFolder_h 



