/* $Id: CondDBObjFactory.h,v 1.1 2006-11-15 14:03:15 poeschl Exp $ */
#ifndef CondDBObjFactory_h
#define CondDBObjFactory_h

#include "ConditionsDB/CondDBMySQLObjectFactory.h"

/**
 * API wrapper for obsolete CondDBObjFactory to CondDBMySQLObjectFactory
 */

class CondDBObjFactory {

public:

  /// Creates an instance of ICondDBObject.
  static ICondDBObject* createCondDBObject(const CondDBKey& since,
                                           const CondDBKey& till,
                                           const std::string& data,
                                           const std::string& description)
     {
    return CondDBMySQLObjectFactory::createCondDBObject
      (since, till, data, description);
  };

  static ICondDBObject* createCondDBObject(const CondDBKey& since,
                                           const CondDBKey& till,
                                           const ICondDBStreamable &obj,
                                           const std::string& description)
     {
    return CondDBMySQLObjectFactory::createCondDBObject
      (since, till, obj, description);
  };

  /// Destroys the instance of ICondDBObject.
  static void destroyCondDBObject(ICondDBObject* CondObj)
     {
    CondDBMySQLObjectFactory::destroyCondDBObject( CondObj );
  };

};

#endif // CondDBObjFactory_h
