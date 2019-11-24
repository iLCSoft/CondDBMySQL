/* $Id: ICondDBTagMgr.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */
#ifndef ICondDBTagMgr_h
#define ICondDBTagMgr_h

#include "ConditionsDB/ICondDBTag.h"
#include "ConditionsDB/CondDBException.h"

#include <string>
#include <vector>


/**
 * Interface to manage the CondDBTag.
 */

class ICondDBTagMgr {

public:

  /// Destructor.
  virtual ~ICondDBTagMgr() {}


  /**
   * Create a new CondDBTag with the specified name and description.
   *
   * Parameters:
   *   name           : a unique string that identifies the CondDBTag
   *   description    : a generic string; it is stored as such and not used.
   *
   */
  virtual void createCondDBTag( const std::string& name,
				const std::string  description = "")
     = 0;

  /**
   * Delete the specified CondDBTag.
   *
   * Parameters:
   *   name           : name of the CondDBTag to delete
   *
   * NOTES:
   * - the deletion is possible only if the tag is not associated to
   *   any CondDBFolder; otherwise the untag operation has to be invoked
   *   beforehand
   */
  virtual void deleteCondDBTag( const std::string& name )
     = 0;

  /**
   * Return the CondDBGTag with the specified name.
   *
   * An exception is thrown if no CondDBTag exist with that name.
   *
   * NOTES:
   * - the object pointed by ICondDBTag* has to be deleted by the
   *   user at the end of its usage.
   */
  virtual void getCondDBTag( const std::string& tagName,
			     ICondDBTag*& tag) const
     = 0;

  /**
   * Return the tag names associated to a folder
   * @param folder The name of the folder to search for tags
   * @param tagNames The names of the tags associated to the given folder
   * @return True if tag names found, false if not
   */
  virtual bool getCondDBTag (const std::string& folder, std::vector<std::string>& tagNames)
        = 0;

  /**
   * This methods returns a vector containing the full name of all
   * defined CondDBTag
   */
  virtual void getAllCondDBTag( std::vector<std::string>& allTag ) const
     = 0;


  /**
   * This methods allows to apply a defined CondDBTag to a CondDBFolder or CondDBFolderSet.
   *
   * Parameters:
   *   folderName   : name of the CondDBFolder or CondDBFolderSet to which
   *                  to apply the tag
   *   tagName      : name of the tag to apply
   *   usingTagName : name of the tag (already applied to that folder) to take
   *                  as a reference (i.e. to copy from). If not passed (i.e. usingTagName = "")
   *                  the current HEAD will be used.
   *
   * NOTES:
   * - a CondDBTag has to be previously created with a name equal to tagName
   * - a CondDBTag can be applied only once to the same CondDBFolder or CondDBFolderSet
   * - a CondDBTag can be applied to one or more CondDBFolder or CondDBFolderSet
   * - when applied to a CondDBFolderSet, the tag is actually applied to all CondDBFolder
   *   beneath
   * - if usingTagName is not applied in that folder an exception will be thrown.
   * - after this operation the two tags tagName and usingTagName will evolve independently
   *   (e.g. usingTagName can be deleted)
   * - specifying folderName="/" the whole tree of CondDBFolder will be tagged
   */
  virtual void tag( const std::string& folderName,
		    const std::string& tagName,
		    std::string usingTagName = "")
     = 0;

  /**
   * This methods checks if a folder "folderName" is already tagged with the
   * tag "tagName"
   */
  virtual bool isTagged( const std::string& folderName,
                         const std::string& tagName)
     = 0;

  /**
   * This methods allows to remove the defined CondDBTag from the CondDBFolder or
   * CondDBFolderSet to which it had been previously associated.
   *
   * Parameters:
   *   folderName : name of the CondDBFolder or CondDBFolderSet to untag
   *   tagName    : name of the tag to remove
   *
   * NOTES:
   * - when applied to a CondDBFolderSet, the tag is actually removed from all CondDBFolder
   *   underneath
   */
  virtual void untag( const std::string& folderName,
		      const std::string& tagName)
     = 0;


};

#endif // ICondDBTagMgr_h
