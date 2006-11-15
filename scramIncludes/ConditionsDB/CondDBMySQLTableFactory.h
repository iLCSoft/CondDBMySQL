#ifndef CONDDBMYSQLTABLEFACTORY_HH
#define CONDDBMYSQLTABLEFACTORY_HH

#include "ConditionsDB/ICondDBTable.h"
#include "ConditionsDB/ICondDBMgr.h"

/**
 * Factory method of ICondDBTable for implementation 
 */
class CondDBMySQLTableFactory {

public:

    /// creates an instance of ICondDBTable
    static ICondDBTable* createCondDBTable()
	throw(CondDBException);
    
    /// creates an instance of ICondDBTable
    static ICondDBTable* createCondDBTable(int numColumns, int numRows = 0)
	throw(CondDBException);
    
    static void destroyCondDBTable(ICondDBTable* CondDBTable)
	throw(CondDBException);
};

#endif /* CONDDBMYSQLTABLEFACTORY_HH */

