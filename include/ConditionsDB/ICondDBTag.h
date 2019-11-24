/* $Id: ICondDBTag.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef ICondDBTag_h
#define ICondDBTag_h

#include "ConditionsDB/CondDBException.h"

#include <string>
#include <vector>

//#include "HepUtilities/SimpleTime.h"
#include "ConditionsDB/SimpleTime.h"

/**
 * Interface wich allows to get information on a defined tag.
 */

class ICondDBTag {

public:

  /// Virtual destructor
  virtual ~ICondDBTag(){};

  /// Return the name of the tag
  virtual std::string getName() const = 0;
  /// Return the description of the tag
  virtual std::string getDescription() const = 0;
  /// Return the creation time of the tag
  virtual void getCreationTime(SimpleTime& creationTime) const = 0;

  /**
   * This method allows to change the name of a CondDBTag
   * Nothing else than the name will be affected by this change.
   * This method requires un update transaction.
   */
  virtual void changeName( const std::string& newName)
     = 0;

  /**
   * This methods returns a vector containing the full name of all
   * CondDBFolder or CondDBFolderSet to which this tag has been applied,
   * and each application time.
   */
  virtual void
  getTaggedFolder(std::vector<std::string>& allFolder,
		  std::vector<SimpleTime>& allApplicationTime) const = 0;
};

#endif // ICondDBTag_h
