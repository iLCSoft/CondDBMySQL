/* $Id: CondDBMySQLObjectFactory.h,v 1.1 2006-11-15 14:03:15 poeschl Exp $ */
#ifndef CondDBMySQLObjectFactory_h
#define CondDBMySQLObjectFactory_h


#include "ConditionsDB/ICondDBObject.h"
#include "ConditionsDB/ICondDBStreamable.h"


/**
 * Factory method of ICondDBObject
 */

class CondDBMySQLObjectFactory {

public:

  /// Creates an instance of ICondDBObject.
  static ICondDBObject* createCondDBObject(const CondDBKey& since,
					   const CondDBKey& till,
					   const std::string& data,
					   const std::string& description)
    ;

  static ICondDBObject* createCondDBObject(const CondDBKey& since,
					   const CondDBKey& till,
					   const ICondDBStreamable &obj,
					   const std::string& description)
    ;


  /// Destroys the instance of ICondDBObject.
  static void destroyCondDBObject(ICondDBObject* CondObj)
    ;
};

#endif // CondDBMySQLObjectFactory_h
