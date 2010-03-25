
#include "ConditionsDB/CondDBMySQLTableFactory.h"
#include "CondDBTable.h"


ICondDBTable*
CondDBMySQLTableFactory::createCondDBTable()
    throw(CondDBException)
{
    return new CondDBTable;
}

ICondDBTable*
CondDBMySQLTableFactory::createCondDBTable(int numColumns, int numRows)
    throw(CondDBException)
{
    return new CondDBTable(numColumns, numRows);
}

void CondDBMySQLTableFactory::destroyCondDBTable(ICondDBTable* CondDBTable)
    throw(CondDBException)
{
    delete CondDBTable;
}


























