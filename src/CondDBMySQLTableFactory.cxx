
#include "ConditionsDB/CondDBMySQLTableFactory.h"
#include "CondDBTable.h"


ICondDBTable*
CondDBMySQLTableFactory::createCondDBTable()

{
    return new CondDBTable;
}

ICondDBTable*
CondDBMySQLTableFactory::createCondDBTable(int numColumns, int numRows)

{
    return new CondDBTable(numColumns, numRows);
}

void CondDBMySQLTableFactory::destroyCondDBTable(ICondDBTable* CondDBTable)

{
    delete CondDBTable;
}
