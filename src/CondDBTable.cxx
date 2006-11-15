/*
 Copyright (C) 1999-2003 FreeDevices.org  geral@freedevices.org

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $Id: CondDBTable.cxx,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

#include "CondDBTable.h"

using std::cerr;
using std::endl;

CondDBTable::CondDBTable(unsigned n_columns, int n_rows):numRows(n_rows)
{
    numRows=0;
    isInitialized = false;
    unsigned i;
    
    for (i=0;i<n_columns; i++)
	conddbtable.push_back(new CondDBColumn);
    
    for (i = 0; i<conddbtable.size(); i++ )
    {
	conddbtable[i]->initialized = false; 
	conddbtable[i]->type = cdbNull;
    }
    
    if (n_rows)
    {
	resize(n_rows);
	status = vector<CondDBStatus>(n_rows,untouched);
    }
}



CondDBTable::~CondDBTable()
{
    for (unsigned i=0; i< conddbtable.size(); i++)
    {
	delete conddbtable[i];
    }
}

void CondDBTable::Initialize(unsigned n_columns, int n_rows) 
{
    unsigned i;

    if (conddbtable.size() > 0 || isInitialized == true)
	return;
    
    for (i=0;i<n_columns; i++)
	conddbtable.push_back(new CondDBColumn);
    
    for (i = 0; i<conddbtable.size(); i++ )
    {
	conddbtable[i]->initialized = false; 
	conddbtable[i]->type = cdbNull;
    }

    if (n_rows)
    {
	resize(n_rows);
	status = vector<CondDBStatus>(n_rows,untouched);
    }
}

int CondDBTable::getNames(vector<string> &names) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    for (unsigned i = 0; i < conddbtable.size(); i++)
    {
	names.push_back(conddbtable[i]->name);
    }

    return 0;
}

int CondDBTable::getTypes(vector<cdb_types> &types) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    for (unsigned i = 0; i < conddbtable.size(); i++)
    {
	types.push_back(conddbtable[i]->type);
    }
    return 0;
}


// when the argument is a string there must be done a spetial treatment
int CondDBTable::getNull(unsigned n_column, string &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;

    if (conddbtable.size() > n_column)
    {
	switch (conddbtable[n_column]->type)
	{
	    case cdbString:
	    {
		CondDBColumnString *tmpColumn = static_cast<CondDBColumnString*>(conddbtable[n_column]);
		return __getNull(null, tmpColumn);
	    }
	    case cdbArrayString:
	    {
		vector<string> tmp;
		CondDBColumnArrayString *tmpColumn = static_cast<CondDBColumnArrayString*>(conddbtable[n_column]);
		 __getNull(tmp, tmpColumn);
		 null = tmp[0]; 
		 return CDB_SUCCESS;
	    }
	    case cdbFloat:
	    {
		float tmp;
		CondDBColumnFloat *tmpColumn = static_cast<CondDBColumnFloat*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null = ToString(tmp);
		return CDB_SUCCESS;
	    }
	    case cdbArrayFloat:
	    {
		vector<float> tmp;
		CondDBColumnArrayFloat *tmpColumn = static_cast<CondDBColumnArrayFloat*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null = ToString(tmp[0]);
		return CDB_SUCCESS;
	    }
	    case cdbInt:
	    {
		long int tmp;
		CondDBColumnInt *tmpColumn = static_cast<CondDBColumnInt*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null = ToString(tmp);
		return CDB_SUCCESS;
	    }
	    case cdbArrayInt:
	    {
		vector<long int> tmp;
		CondDBColumnArrayInt *tmpColumn = static_cast<CondDBColumnArrayInt*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null = ToString(tmp[0]);
		return CDB_SUCCESS;
	    }
	    case cdbDouble:
	    {
		double tmp;
		CondDBColumnDouble *tmpColumn = static_cast<CondDBColumnDouble*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null = ToString(tmp);
		return CDB_SUCCESS;
	    }
	    case cdbArrayDouble:
	    {
		vector<double> tmp;
		CondDBColumnArrayDouble *tmpColumn = static_cast<CondDBColumnArrayDouble*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null = ToString(tmp[0]);
		return CDB_SUCCESS;
	    }
	    case cdbLongLong:
	    {
		int64 tmp;
		CondDBColumnLong *tmpColumn = static_cast<CondDBColumnLong*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null = ToString(tmp);
		return CDB_SUCCESS;
	    }
	    case cdbArrayLongLong:
	    {
		vector<int64> tmp;
		CondDBColumnArrayLong *tmpColumn = static_cast<CondDBColumnArrayLong*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null = ToString(tmp[0]);
		return CDB_SUCCESS;
	    }
	    case cdbBool:
	    case cdbArrayBool:
	    {
		null=ToString(-1);
		return CDB_SUCCESS;
	    }
	    default:
		return CDB_TYPE_ERROR;
	}
    }
    else
	return CDB_RANGE_ERROR; 
}

int CondDBTable::getNull(unsigned n_column, float &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbFloat)
	{
	    CondDBColumnFloat *tmpColumn = static_cast<CondDBColumnFloat*>(conddbtable[n_column]);
	    return __getNull(null, tmpColumn);
	}
	else if ( conddbtable[n_column]->type == cdbArrayFloat)
	    {
		vector<float> tmp;
		CondDBColumnArrayFloat *tmpColumn = static_cast<CondDBColumnArrayFloat*>(conddbtable[n_column]);
		__getNull(tmp, tmpColumn);
		null=tmp[0];
		return CDB_SUCCESS;
	    }
	else
	    return CDB_TYPE_ERROR;
    else
	return CDB_RANGE_ERROR; 
}

int CondDBTable::getNull(unsigned n_column, long int &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbInt)
	{
	    CondDBColumnInt *tmpColumn = static_cast<CondDBColumnInt*>(conddbtable[n_column]);
	    return __getNull(null, tmpColumn);
	}
	else if (conddbtable[n_column]->type == cdbArrayInt)
	{
	    vector<long int> tmp; 
	    CondDBColumnArrayInt *tmpColumn = static_cast<CondDBColumnArrayInt*>(conddbtable[n_column]);
	    __getNull(tmp, tmpColumn);
	    null = tmp[0];
	    return CDB_SUCCESS;
	}
	else
	    return CDB_TYPE_ERROR;
    else
	return CDB_RANGE_ERROR; 
}

int CondDBTable::getNull(unsigned n_column, double &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbDouble)
	{
	    CondDBColumnDouble *tmpColumn = static_cast<CondDBColumnDouble*>(conddbtable[n_column]);
	    return __getNull(null, tmpColumn);
	}
	else if (conddbtable[n_column]->type == cdbArrayDouble)
	{
	    vector<double> tmp;
	    CondDBColumnArrayDouble *tmpColumn = static_cast<CondDBColumnArrayDouble*>(conddbtable[n_column]);
	    __getNull(tmp, tmpColumn);
	    null = tmp[0];
	    return CDB_SUCCESS;
	}
	else
	    return CDB_TYPE_ERROR;
    else
	return CDB_RANGE_ERROR; 
}

int CondDBTable::getNull(unsigned n_column, int64 &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbLongLong)
	{
	    CondDBColumnLong *tmpColumn = static_cast<CondDBColumnLong*>(conddbtable[n_column]);
	    return __getNull(null, tmpColumn);
	}
	else if ( conddbtable[n_column]->type == cdbArrayLongLong)
	{
	    vector<int64> tmp;
	    CondDBColumnArrayLong *tmpColumn = static_cast<CondDBColumnArrayLong*>(conddbtable[n_column]);
	    __getNull(tmp, tmpColumn);
	    null = tmp[0];
	    return CDB_SUCCESS;
	}
	else
	    return CDB_TYPE_ERROR;
    else
	return CDB_RANGE_ERROR; 
}


int CondDBTable::setName(unsigned n_column, const string& name) 
{
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->name.size() == 0)
	{
	    conddbtable[n_column]->name = name;
	    if (conddbtable[n_column]->type != cdbNull) 
	    {	
		conddbtable[n_column]->initialized = true;
		verifyInitialization();
	    }
	}
	else
	    return CDB_TYPE_ERROR; 
    else
	return CDB_RANGE_ERROR;
    return CDB_SUCCESS;
}

int CondDBTable::setType(unsigned n_column, cdb_types type) 
{
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbNull)
	{
	    conddbtable[n_column]->type = type;
	    
	    switch (type)
	    {
		case cdbBool:
		{
		    CondDBColumnBool * tmpColumn = new CondDBColumnBool;
		    tmpColumn->type = conddbtable[n_column]->type;
		    tmpColumn->column.push_back(-1);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}   
		case cdbInt:
		{
		    CondDBColumnInt * tmpColumn = new CondDBColumnInt;
		    tmpColumn->type = conddbtable[n_column]->type;
		    tmpColumn->column.push_back(NULLINT);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}   
		case cdbLongLong:
		{
		    CondDBColumnLong * tmpColumn = new CondDBColumnLong;
		    tmpColumn->type = conddbtable[n_column]->type;
		    tmpColumn->column.push_back(NULLLONG);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}   
		
		case cdbFloat:
		{
		    CondDBColumnFloat * tmpColumn = new CondDBColumnFloat;
		    tmpColumn->type = conddbtable[n_column]->type;
		    tmpColumn->column.push_back(NULLFLOAT);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		case cdbString:
		{
		    CondDBColumnString * tmpColumn = new CondDBColumnString;
		    tmpColumn->type = conddbtable[n_column]->type;
		    tmpColumn->column.push_back("NULL");
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		case cdbDouble:
		{
		    CondDBColumnDouble * tmpColumn = new CondDBColumnDouble;
		    tmpColumn->type = conddbtable[n_column]->type;
		    tmpColumn->column.push_back(NULLDOUBLE);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		case cdbArrayInt:
		{		 
		    CondDBColumnArrayInt * tmpColumn = new CondDBColumnArrayInt;
		    tmpColumn->type = conddbtable[n_column]->type;
		    vector<long int> tmp;
		    tmp.push_back(NULLINT);
		    tmpColumn->column.push_back(tmp);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		case cdbArrayLongLong:
		{		 
		    CondDBColumnArrayLong * tmpColumn = new CondDBColumnArrayLong;
		    tmpColumn->type = conddbtable[n_column]->type;
		    vector<int64> tmp;
		    tmp.push_back(NULLLONG);
		    tmpColumn->column.push_back(tmp);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		case cdbArrayBool:
		{		 
		    CondDBColumnArrayBool * tmpColumn = new CondDBColumnArrayBool;
		    tmpColumn->type = conddbtable[n_column]->type;
		    vector<short int> tmp;
		    tmp.push_back(-1);
		    tmpColumn->column.push_back(tmp);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		case cdbArrayFloat:
		{
		    CondDBColumnArrayFloat * tmpColumn = new CondDBColumnArrayFloat;
		    tmpColumn->type = conddbtable[n_column]->type;
		    vector<float> tmp;
		    tmp.push_back(NULLFLOAT);
		    tmpColumn->column.push_back(tmp);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		case cdbArrayString:
		{
		    CondDBColumnArrayString * tmpColumn = new CondDBColumnArrayString;
		    tmpColumn->type = conddbtable[n_column]->type;
		    vector<string> tmp;
		    tmp.push_back("NULL");
		    tmpColumn->column.push_back(tmp);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		case cdbArrayDouble:
		{
		    CondDBColumnArrayDouble * tmpColumn = new CondDBColumnArrayDouble;
		    tmpColumn->type = conddbtable[n_column]->type;
		    vector<double> tmp;
		    tmp.push_back(NULLDOUBLE);
		    tmpColumn->column.push_back(tmp);
		    if ((conddbtable[n_column]->name.size()))
			tmpColumn->name = conddbtable[n_column]->name;
		    conddbtable[n_column] = tmpColumn;
		    break;
		}
		default : break;
	    }
	    
	    if (conddbtable[n_column]->name.size() != 0)
	    {
		conddbtable[n_column]->initialized = true;
		verifyInitialization();
	    }
	}
	else
	    return CDB_TYPE_ERROR; 
    else
	return CDB_RANGE_ERROR; 
    return CDB_SUCCESS;
    
}

int CondDBTable::setNull(unsigned n_column, const string &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	switch (conddbtable[n_column]->type)
	{
	    case cdbString:
	    {
		static_cast<CondDBColumnString*>(conddbtable[n_column])->column[0] = null;
		return CDB_SUCCESS;
	    }
	    case cdbArrayString:
	    {
		vector<string> tmp(1,null);
		static_cast<CondDBColumnArrayString*>(conddbtable[n_column])->column[0] = tmp;
		return CDB_SUCCESS;
	    }
	    case cdbFloat:
	    {
		static_cast<CondDBColumnFloat*>(conddbtable[n_column])->column[0] = atof(null.c_str());
		return CDB_SUCCESS;
	    }
	    case cdbArrayFloat:
	    {
		vector<float> tmp(1,atof(null.c_str()));
		static_cast<CondDBColumnArrayFloat*>(conddbtable[n_column])->column[0] = tmp;
		return CDB_SUCCESS;
	    }
	    case cdbInt:
	    {
		static_cast<CondDBColumnInt*>(conddbtable[n_column])->column[0] = atol(null.c_str());
		return CDB_SUCCESS;
	    }
	    case cdbArrayInt:
	    {
		vector<long int> tmp(1,atol(null.c_str()));
		static_cast<CondDBColumnArrayInt*>(conddbtable[n_column])->column[0] = tmp;
		return CDB_SUCCESS;
	    }
	    case cdbBool:
	    case cdbArrayBool:
	    {
		return CDB_SUCCESS;
	    }
	    case cdbLongLong:
	    {
		static_cast<CondDBColumnString*>(conddbtable[n_column])->column[0] = atoi64(null.c_str());
		return CDB_SUCCESS;
	    }
	    case cdbArrayLongLong:
	    {
		vector<int64> tmp(1,atoi64(null.c_str()));
		static_cast<CondDBColumnArrayLong*>(conddbtable[n_column])->column[0] = tmp;
		return CDB_SUCCESS;
	    }
	    case cdbDouble:
	    {
		static_cast<CondDBColumnDouble*>(conddbtable[n_column])->column[0] = atof(null.c_str());
		return CDB_SUCCESS;
	    }
	    case cdbArrayDouble:
	    {
		vector<double> tmp(1,atof(null.c_str()));
		static_cast<CondDBColumnArrayDouble*>(conddbtable[n_column])->column[0] = tmp;
		return CDB_SUCCESS;
	    }
	    default:
		return CDB_TYPE_ERROR;
	}
    else
	return CDB_RANGE_ERROR;
}

int CondDBTable::setNull(unsigned n_column, const long int &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbInt)
	{
	    static_cast<CondDBColumnInt*>(conddbtable[n_column])->column[0] = null;
	    return CDB_SUCCESS;
	}
	else if (conddbtable[n_column]->type == cdbArrayInt)
	{
	    vector<long int> tmp(1,null);
	    static_cast<CondDBColumnArrayInt*>(conddbtable[n_column])->column[0] = tmp;
	    return CDB_SUCCESS;
	}  
	else
	    return CDB_TYPE_ERROR;
    else
	return CDB_RANGE_ERROR;
}

int CondDBTable::setNull(unsigned n_column, const int64 &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbLongLong )
	{
	    static_cast<CondDBColumnLong*>(conddbtable[n_column])->column[0] = null;
	    return CDB_SUCCESS;
	}
	else if (conddbtable[n_column]->type == cdbArrayLongLong)
	{
	    vector<int64> tmp(1,null);
	    static_cast<CondDBColumnArrayLong*>(conddbtable[n_column])->column[0] = tmp;
	    return CDB_SUCCESS;
	}
	else
	    return CDB_TYPE_ERROR;
    else
	return CDB_RANGE_ERROR;
}

int CondDBTable::setNull(unsigned n_column, const float &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbFloat )
	{
	    static_cast<CondDBColumnFloat*>(conddbtable[n_column])->column[0] = null;
	    return CDB_SUCCESS;
	}
	else if (conddbtable[n_column]->type == cdbArrayFloat)
	{
	    vector<float>  tmp(1,null);
	    static_cast<CondDBColumnArrayFloat*>(conddbtable[n_column])->column[0] = tmp;
	    return CDB_SUCCESS;
	}
	else
	    return CDB_TYPE_ERROR;
    else
	return CDB_RANGE_ERROR;
}

int CondDBTable::setNull(unsigned n_column, const double &null)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (conddbtable.size() > n_column)
	if (conddbtable[n_column]->type == cdbDouble || conddbtable[n_column]->type == cdbArrayDouble)
	{
	    static_cast<CondDBColumnDouble*>(conddbtable[n_column])->column[0] = null;
	    return CDB_SUCCESS;
	}
	else if (conddbtable[n_column]->type == cdbArrayDouble)
	{
	    vector<double> tmp(1, null);
	    static_cast<CondDBColumnArrayDouble*>(conddbtable[n_column])->column[0] = tmp;
	    return CDB_SUCCESS;
	}
	else
	    return CDB_TYPE_ERROR;
    else
	return CDB_RANGE_ERROR;
}


// getCell - getting the value of a cell.

int CondDBTable::getCell(unsigned n_column, unsigned n_row, long int &ndata) 
{
    CondDBColumnInt *tmpColumn = static_cast<CondDBColumnInt*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbInt, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, short int &ndata) 
{
    CondDBColumnBool *tmpColumn = static_cast<CondDBColumnBool*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbBool, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, int64 &ndata) 
{
    CondDBColumnLong *tmpColumn = static_cast<CondDBColumnLong*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbLongLong, tmpColumn); 
}


int CondDBTable::getCell(unsigned n_column, unsigned n_row, float &ndata) 
{
    CondDBColumnFloat *tmpColumn = static_cast<CondDBColumnFloat*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbFloat, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, string &ndata) 
{
    CondDBColumnString *tmpColumn = static_cast<CondDBColumnString*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbString, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, double &ndata) 
{
    CondDBColumnDouble *tmpColumn = static_cast<CondDBColumnDouble*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbDouble, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, vector<long int> &ndata) 
{
    CondDBColumnArrayInt *tmpColumn = static_cast<CondDBColumnArrayInt*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbArrayInt, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, vector<short int> &ndata) 
{ 
    CondDBColumnArrayBool *tmpColumn = static_cast<CondDBColumnArrayBool*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbArrayBool, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, vector<int64> &ndata) 
{ 
    CondDBColumnArrayLong *tmpColumn = static_cast<CondDBColumnArrayLong*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbArrayLongLong, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, vector<float> &ndata) 
{ 
    CondDBColumnArrayFloat *tmpColumn = static_cast<CondDBColumnArrayFloat*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbArrayFloat, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, vector<string> &ndata) 
{ 
    CondDBColumnArrayString *tmpColumn = static_cast<CondDBColumnArrayString*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbArrayString, tmpColumn); 
}

int CondDBTable::getCell(unsigned n_column, unsigned n_row, vector<double> &ndata) 
{ 
    CondDBColumnArrayDouble *tmpColumn = static_cast<CondDBColumnArrayDouble*>(conddbtable[n_column]);
    return __getCell(n_column, n_row, ndata, cdbArrayDouble, tmpColumn); 
}

///////////////////
// data insertion members 
///////////////////

// public members
int CondDBTable::setCell(unsigned n_column, unsigned n_row, const long int ndata) 
{
    CondDBColumnInt *tmpColumn = static_cast<CondDBColumnInt*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbInt, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const short int ndata) 
{
    CondDBColumnBool *tmpColumn = static_cast<CondDBColumnBool*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbBool, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const int64 ndata) 
{
    CondDBColumnLong *tmpColumn = static_cast<CondDBColumnLong*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbLongLong, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row,const float ndata) 
{
    CondDBColumnFloat *tmpColumn = static_cast<CondDBColumnFloat*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbFloat, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const string ndata) 
{
    
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if ((numRows > n_row) && (conddbtable.size() > n_column))     
    {
	if (conddbtable[n_column]->type == cdbString)
	{
	    CondDBColumnString *tmpColumn = static_cast<CondDBColumnString*>(conddbtable[n_column]);
	    __setCell(n_column, n_row, ndata, cdbString, tmpColumn);
	}
	else
	{
	    switch(conddbtable[n_column]->type)
	    {
		case cdbInt:
		{
		    if (ndata=="NULL")
		    {
			CondDBColumnInt *tmpColumn = static_cast<CondDBColumnInt*>(conddbtable[n_column]);
			__setCell(n_column, n_row, tmpColumn->column[0], cdbInt, tmpColumn);
		    }
		    else
		    {
			CondDBColumnInt *tmpColumn = static_cast<CondDBColumnInt*>(conddbtable[n_column]);
			__setCell(n_column, n_row, atol(ndata.c_str()), cdbInt, tmpColumn);
		    }
		    break;
		}
		case cdbBool:
		{
		    if (ndata=="NULL")
		    {
			CondDBColumnBool *tmpColumn = static_cast<CondDBColumnBool*>(conddbtable[n_column]);
			__setCell(n_column,n_row, tmpColumn->column[0], cdbBool, tmpColumn);
			
		    }
		    else
		    {
			CondDBColumnBool *tmpColumn = static_cast<CondDBColumnBool*>(conddbtable[n_column]);
			__setCell(n_column, n_row, atoi(ndata.c_str()), cdbBool, tmpColumn);
		    }
		    break;
		}
		case cdbLongLong:
		{
		    if (ndata=="NULL")
		    {
			CondDBColumnLong *tmpColumn = static_cast<CondDBColumnLong*>(conddbtable[n_column]);
			__setCell(n_column,n_row, tmpColumn->column[0], cdbLongLong, tmpColumn);
		    }
		    else
		    {
			CondDBColumnLong *tmpColumn = static_cast<CondDBColumnLong*>(conddbtable[n_column]);
			__setCell(n_column,n_row, atoi64(ndata.c_str()), cdbLongLong, tmpColumn);
		    }
		    break;
		}
		case cdbFloat:
		{
		    if (ndata=="NULL")
		    {
			CondDBColumnFloat *tmpColumn = static_cast<CondDBColumnFloat*>(conddbtable[n_column]);
			__setCell(n_column,n_row, tmpColumn->column[0], cdbFloat, tmpColumn);
		    }
		    else
		    {
			CondDBColumnFloat *tmpColumn = static_cast<CondDBColumnFloat*>(conddbtable[n_column]);
			__setCell(n_column,n_row, strtod(ndata.c_str(), (char **)NULL), cdbFloat, tmpColumn);
		    }
		    break;
		}
		case cdbDouble:
		{
		    if (ndata=="NULL")
		    {
			CondDBColumnDouble *tmpColumn = static_cast<CondDBColumnDouble*>(conddbtable[n_column]);
			__setCell(n_column,n_row, tmpColumn->column[0],cdbDouble, tmpColumn);
		    }
		    else
		    {
			CondDBColumnDouble *tmpColumn = static_cast<CondDBColumnDouble*>(conddbtable[n_column]);
			__setCell(n_column,n_row,strtod(ndata.c_str(), (char **)NULL),cdbDouble, tmpColumn);
		    }
		    break;
		}
		//if the column is of the type array of *type* one should use the method with vector of strings
		default:
		    return CDB_STRTOARRAY_ERROR;
	    }
	}
    }
    else
	return CDB_RANGE_ERROR; 
    return CDB_SUCCESS;
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const double ndata) 
{
    CondDBColumnDouble *tmpColumn = static_cast<CondDBColumnDouble*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbDouble, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const vector<short int> &ndata) 
{
    CondDBColumnArrayBool *tmpColumn = static_cast<CondDBColumnArrayBool*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbArrayBool, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const vector<long int> &ndata) 
{
    CondDBColumnArrayInt *tmpColumn = static_cast<CondDBColumnArrayInt*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbArrayInt, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const vector<int64> &ndata) 
{
    CondDBColumnArrayLong *tmpColumn = static_cast<CondDBColumnArrayLong*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbArrayLongLong, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const vector<float> &ndata) 
{
    CondDBColumnArrayFloat *tmpColumn = static_cast<CondDBColumnArrayFloat*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbArrayFloat, tmpColumn);
}

int CondDBTable::setCell(unsigned n_column, unsigned n_row, const vector<double> &ndata) 
{
    CondDBColumnArrayDouble *tmpColumn = static_cast<CondDBColumnArrayDouble*>(conddbtable[n_column]);
    return __setCell(n_column, n_row, ndata, cdbArrayDouble, tmpColumn);
}


// nbarros: necessario refazer!!!
int CondDBTable::setCell(unsigned n_column, unsigned n_row, const vector<string> &ndata) 
{

    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if ((numRows > n_row) && (conddbtable.size() > n_column))
    {
	if (conddbtable[n_column]->type == cdbArrayString)
	{
	    CondDBColumnArrayString *tmpColumn = static_cast<CondDBColumnArrayString*>(conddbtable[n_column]);
	    return __setCell(n_column, n_row, ndata, cdbArrayString, tmpColumn);
//	    static_cast<CondDBColumnArrayString*>(conddbtable[n_column])->column[n_row+1] = ndata;
	}
	else
	{
	    switch(conddbtable[n_column]->type)
	    {
		case cdbArrayInt:
		{
		    vector<long int> tmp;
		    for (unsigned i=0; i<ndata.size(); i++)
		    {
			if (ndata[i]=="NULL")
			{
			    long int null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atol(ndata[i].c_str()));
		    }
		    CondDBColumnArrayInt *tmpColumn = static_cast<CondDBColumnArrayInt*>(conddbtable[n_column]);
		    return __setCell(n_column, n_row, tmp, cdbArrayInt, tmpColumn);
		    break;
		}
		case cdbArrayBool:
		{
		    vector<short int> tmp;
		    for (unsigned i=0; i<ndata.size(); i++)
		    {
			if (ndata[i]=="NULL")
			{
			    tmp.push_back(-1);
			}
			else
			    tmp.push_back(atoi(ndata[i].c_str()));
		    }
		    CondDBColumnArrayBool *tmpColumn = static_cast<CondDBColumnArrayBool*>(conddbtable[n_column]);
		    return __setCell(n_column, n_row, tmp, cdbArrayBool, tmpColumn);
		    break;
		}
		case cdbArrayLongLong:
		{
		    vector<int64> tmp;
		    for (unsigned i=0; i<ndata.size(); i++)
		    {
			if (ndata[i]=="NULL")
			{
			    int64 null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else

			    tmp.push_back(atoi64(ndata[i].c_str()));
		    }
		    CondDBColumnArrayLong *tmpColumn = static_cast<CondDBColumnArrayLong*>(conddbtable[n_column]);
		    return __setCell(n_column, n_row, tmp, cdbArrayLongLong, tmpColumn);
		    break;
		}
		case cdbArrayFloat:
		{
		    vector<float> tmp;
		    for (unsigned i=0; i<ndata.size(); i++)
		    {
			if (ndata[i]=="NULL")
			{
			    float null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atof(ndata[i].c_str()));
		    }
	 	    CondDBColumnArrayFloat *tmpColumn = static_cast<CondDBColumnArrayFloat*>(conddbtable[n_column]);
		    return __setCell(n_column, n_row, tmp, cdbArrayFloat, tmpColumn);
		    break;
		}
		case cdbArrayDouble:
		{
		    vector<double> tmp;
		    for (unsigned i=0; i<ndata.size(); i++)
		    {
			if (ndata[i]=="NULL")
			{
			    double null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(strtod(ndata[i].c_str(), (char**)NULL));
		    }
		    CondDBColumnArrayDouble *tmpColumn = static_cast<CondDBColumnArrayDouble*>(conddbtable[n_column]);
		    return __setCell(n_column, n_row, tmp, cdbArrayDouble, tmpColumn);
		    break;
		}
		//if the column is of the type array of *type* one should use the method with vector of strings
		default:
		    return CDB_STRTOARRAY_ERROR;
	    }
	}
    }
    else
	return CDB_RANGE_ERROR;
    return CDB_SUCCESS;
}

///////////////////
// setData members - append data to columns.
//
// Return the number of elements of the vector passed that have been appended to the column
// or an error code.
///////////////////


// data type specific members (public)
int CondDBTable::setColumndata(unsigned n_column,const vector<long int> &data) 
{
    CondDBColumnInt *tmpColumn = static_cast<CondDBColumnInt*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbInt, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<int64> &data) 
{
    CondDBColumnLong *tmpColumn = static_cast<CondDBColumnLong*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbLongLong, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<short int> &data) 
{
    CondDBColumnBool *tmpColumn = static_cast<CondDBColumnBool*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbBool, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<float> &data) 
{
    CondDBColumnFloat *tmpColumn = static_cast<CondDBColumnFloat*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbFloat, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<string> &data) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    unsigned index = 0;
    if (n_column < conddbtable.size())
    {
	if (conddbtable[n_column]->type == cdbString)
	{
	    CondDBColumnString* tmpColumn = static_cast<CondDBColumnString*>(conddbtable[n_column]);
	    while((tmpColumn->column.size() < numRows+1) && (index < data.size()))
	    {
		tmpColumn->column.push_back(data[index]);
		index++;
	    }
	    return index;
	}
	else
	{
// If the data type is not string we can convert the string to the corresponding type
	    switch(conddbtable[n_column]->type)
	    {
//if the column data type is some kind of array it will be assumed that all values belong to a single cell thus returning 1(cell)
		case cdbArrayInt:
		{
		    vector<long int> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    long int null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atol(data[i].c_str()));
		    }
		    index= setColumndata(n_column, tmp);
		    break;
		}
		case cdbArrayBool:
		{
		    vector<short int> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    short int null = -1;
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atoi(data[i].c_str()));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
		case cdbArrayLongLong:
		{
		    vector<int64> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    int64 null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atoi64(data[i].c_str()));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
		case cdbArrayFloat:
		{
		    vector<float> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    float null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atof(data[i].c_str()));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
		case cdbArrayDouble:
		{
		    vector<double> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    double null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(strtod(data[i].c_str(), (char**)NULL));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
//If the column data type is not any array it will be inserted a value in each row!
		case cdbInt:
		{
		    vector<long int> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    long int null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atol(data[i].c_str()));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
		case cdbBool:
		{
		    vector<short int> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    // for bool the null value is -1 (not editable by user)
			    short int null = -1;
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atoi(data[i].c_str()));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
		case cdbLongLong:
		{
		    vector<int64> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    int64 null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atoi64(data[i].c_str()));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
		case cdbFloat:
		{
		    vector<float> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    float null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(atof(data[i].c_str()));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
		case cdbDouble:
		{
		    vector<double> tmp;
		    for (unsigned i=0; i<data.size(); i++)
		    {
			if (data[i]=="NULL")
			{
			    double null;
			    getNull(n_column, null);
			    tmp.push_back(null);
			}
			else
			    tmp.push_back(strtod(data[i].c_str(), (char**)NULL));
		    }
		    index = setColumndata(n_column, tmp);
		    break;
		}
		default:
		    return CDB_STRTOARRAY_ERROR;
	    }
	    
	    return index;
	}
    }
    else
	return CDB_RANGE_ERROR;
}

int CondDBTable::setColumndata(unsigned n_column, const vector<double> &data) 
{
    CondDBColumnDouble *tmpColumn = static_cast<CondDBColumnDouble*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbDouble, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<vector<long int> > &data) 
{
    CondDBColumnArrayInt *tmpColumn = static_cast<CondDBColumnArrayInt*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbArrayInt, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<vector<int64> > &data) 
{
    CondDBColumnArrayLong *tmpColumn = static_cast<CondDBColumnArrayLong*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbArrayLongLong, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<vector<short int> > &data) 
{
    CondDBColumnArrayBool *tmpColumn = static_cast<CondDBColumnArrayBool*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbArrayBool, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<vector<float> > &data) 
{
    CondDBColumnArrayFloat *tmpColumn = static_cast<CondDBColumnArrayFloat*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbArrayFloat, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<vector<double> > &data) 
{
    CondDBColumnArrayDouble *tmpColumn = static_cast<CondDBColumnArrayDouble*>(conddbtable[n_column]);
    return __setColumnData(n_column, data, cdbArrayDouble, tmpColumn);
}

int CondDBTable::setColumndata(unsigned n_column, const vector<vector<string> > &data) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    unsigned index = 0;
    if (n_column < conddbtable.size())
    {
	if (conddbtable[n_column]->type == cdbArrayString)
	{
	    CondDBColumnArrayString* tmpColumn = static_cast<CondDBColumnArrayString*>(conddbtable[n_column]);
	    while((tmpColumn->column.size() < numRows+1) && (index < data.size()))
	    {
		tmpColumn->column.push_back(data[index]);
		index++;
	    }
	    return index;
	}
	else
	{
// If the data type is not string we can convert the string to the corresponding type
	    switch(conddbtable[n_column]->type)
	    {
//if the column data type is an array of any type each inner vector refers to a vector on a cell
		case cdbArrayInt:
		{
		    vector<vector<long int> > aux;
		    for (unsigned j=0; j<data.size(); j++)
		    {
			vector<long int> tmp;
			for (unsigned i=0; i<data[j].size(); i++)
			{
			    if (data[j][i]=="NULL")
			    {
				long int null;
				getNull(n_column, null);
				tmp.push_back(null);
			    }
			    else
				tmp.push_back(atol(data[j][i].c_str()));
			}
			aux.push_back(tmp);
		    }
		    index = setColumndata(n_column, aux);
		    break;
		}
		case cdbArrayLongLong:
		{
		    vector<vector<int64> > aux;
		    for (unsigned j=0; j<data.size(); j++)
		    {
			vector<int64> tmp;
			for (unsigned i=0; i<data[j].size(); i++)
			{
			    if (data[j][i]=="NULL")
			    {
				int64 null;
				getNull(n_column, null);
				tmp.push_back(null);
			    }
			    else
				tmp.push_back(atoi64(data[j][i].c_str()));
			}
			aux.push_back(tmp);
		    }
		    index = setColumndata(n_column, aux);
		    break;
		}
		case cdbArrayBool:
		{
		    vector<vector<short int> > aux;
		    for (unsigned j=0; j<data.size(); j++)
		    {
			vector<short int> tmp;
			for (unsigned i=0; i<data[j].size(); i++)
			{
			    if (data[j][i]=="NULL")
			    {
				tmp.push_back(-1);
			    }
			    else
				tmp.push_back(atoi(data[j][i].c_str()));
			}
			aux.push_back(tmp);
		    }
		    index = setColumndata(n_column, aux);
		    break;
		}
		case cdbArrayFloat:
		{
		    vector<vector<float> > aux;
		    for (unsigned j=0; j<data.size(); j++)
		    {
			vector<float> tmp;
			for (unsigned i=0; i<data[j].size(); i++)
			{
			    if (data[j][i]=="NULL")
			    {
				float null;
				getNull(n_column, null);
				tmp.push_back(null);
			    }
			    else
				tmp.push_back(atof(data[j][i].c_str()));
			}
			aux.push_back(tmp);
		    }
		    index = setColumndata(n_column, aux);
		    break;
		}
		case cdbArrayDouble:
		{
		    vector<vector<double> > aux;
		    for (unsigned j=0; j<data.size(); j++)
		    {
			vector<double> tmp;
			for (unsigned i=0; i<data[j].size(); i++)
			{
			    if (data[j][i]=="NULL")
			    {
				double null;
				getNull(n_column, null);
				tmp.push_back(null);
			    }
			    else
				tmp.push_back(strtod(data[j][i].c_str(), (char**)NULL));
			}
			aux.push_back(tmp);
		    }
		    index = setColumndata(n_column, aux);
		    break;
		}
		default:
		    return CDB_STRTOARRAY_ERROR;
	    }
	    return index;
	}
    }
    else
	return CDB_RANGE_ERROR;
    
}

///////////////////////////////////////////////////////////
// resize() - realocates space for table.
//
// The number of rows is the number of new rows to allocate
///////////////////////////////////////////////////////////

void CondDBTable::resize(int num_rows) 
{
    numRows += num_rows;
}

//////////////////////////////////////////////////////////////////
// getRow() - return a vector of strings with the values of a row
//
//  it returns an error code *TO BE IMPLEMENTED*
//////////////////////////////////////////////////////////////////
int CondDBTable::getRow(unsigned n_row, vector<string> &values) 
{
    if (n_row < numRows)
    {
	for (unsigned i =0; i<conddbtable.size(); i++)
	{
	    switch(conddbtable[i]->type)
	    {
		case cdbInt:
		{
		    long int tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp));
		    break;
		}
		case cdbBool:
		{
		    short int tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp));
		    break;
		}
		case cdbLongLong:
		{
		    int64 tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp));
		    break;
		}
		case cdbFloat:
		{
		    float tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp));
		    break;

		}
		case cdbString:
		{
		    string tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp));
		    break;
		    
		}
		case cdbDouble:
		{
		    double tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp));
		    break;
		    
		}
		case cdbArrayInt:
		{
		    vector<long int> tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp.size()));
		    for (unsigned int t=0; t<tmp.size(); t++)
			values.push_back(ToString(tmp[t]));
		    break;
		}
		case cdbArrayBool:
		{
		    vector<short int> tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp.size()));
		    for (unsigned int t=0; t<tmp.size(); t++)
			values.push_back(ToString(tmp[t]));
		    break;
		}
		case cdbArrayLongLong:
		{
		    vector<int64> tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp.size()));
		    for (unsigned int t=0; t<tmp.size(); t++)
			values.push_back(ToString(tmp[t]));
		    break;
		}
		case cdbArrayFloat:
		{
		    vector<float> tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp.size()));
		    for (unsigned int t=0; t<tmp.size(); t++)
			values.push_back(ToString(tmp[t]));
		    break;
		}
		case cdbArrayString:
		{
		    vector<string> tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp.size()));
		    for (unsigned int t=0; t<tmp.size(); t++)
			values.push_back(ToString(tmp[t]));
		    break;
		}
		case cdbArrayDouble:
		{
		    vector<double> tmp;
		    getCell(i,n_row, tmp);
		    values.push_back(ToString(tmp.size()));
		    for (unsigned int t=0; t<tmp.size(); t++)
			values.push_back(ToString(tmp[t]));
		    break;
		}
		default: return CDB_TYPE_ERROR;
	    }    
	}
	
    }
    else
	return CDB_RANGE_ERROR;
// sucess!
    return CDB_SUCCESS;
}

// verifies if all columns are intialized
void CondDBTable::verifyInitialization()
{
    bool aux = true;
    
    for (unsigned i=0; i< conddbtable.size(); i++)
    {
	if (!(conddbtable[i]->initialized))
	{
	    aux = false;
	    break;
	}
    }

    if (aux)
	isInitialized = true;
}


int CondDBTable::setSinceTime(const SimpleTime &since) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (!(ti.size() < numRows) )
	return CDB_RANGE_ERROR;
    
    ti.push_back(since);
    
    return CDB_SUCCESS;
}

int CondDBTable::setSinceTime(const vector<SimpleTime> &since) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (!(ti.size() < numRows) )
	return CDB_RANGE_ERROR;
    
    unsigned index = 0;
    
    while((ti.size() != numRows) && (index < since.size()))
    {
	ti.push_back(since[index]);
	    index++;
    }
    
    return index;
}


int CondDBTable::changeSinceTime(unsigned n_row,const SimpleTime &since) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (n_row >= numRows)
	return CDB_RANGE_ERROR;
    
    if (n_row == ti.size())
	ti.push_back(since);
    
    if (n_row > ti.size())
    {
	while (n_row - ti.size())
	{
	    ti.push_back(ti.back());
	}
	ti.push_back(since);
    }//return CDB_ALLOC_ERROR;
    else
	ti[n_row] = since;
    
    return CDB_SUCCESS;
}

int CondDBTable::changeTillTime(unsigned n_row,const SimpleTime &till) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (n_row >= numRows)
	    return CDB_RANGE_ERROR;
    
    if (n_row == tf.size())
	tf.push_back(till);
    
    if (n_row > tf.size())
    {
	while (n_row - tf.size())
	{
	    tf.push_back(tf.back());
	}
	tf.push_back(till);
    }//return CDB_ALLOC_ERROR;
    else
	tf[n_row] = till;

/*    if (n_row >= tf.size())
	return CDB_ALLOC_ERROR;
    
    tf[n_row] = till;
*/  
    return CDB_SUCCESS;
}

int CondDBTable::setTillTime(const SimpleTime &till) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (!(tf.size() < numRows) )
	return CDB_RANGE_ERROR;
    
    tf.push_back(till);

    return CDB_SUCCESS;
}

int CondDBTable::setTillTime(const vector<SimpleTime> &till) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (!(tf.size() < numRows) )
	return CDB_RANGE_ERROR;
    
    unsigned index = 0;
    
    while((tf.size() != numRows) && (index < till.size()))
    {
	tf.push_back(till[index]);
	index++;
    }
    
    return index;
}

int CondDBTable::getSinceTime(unsigned n_row, SimpleTime &since) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (n_row > numRows)
	return CDB_RANGE_ERROR;
    
    if (n_row > ti.size())
	return ti.back();//CDB_ALLOC_ERROR;
    
    since = ti[n_row];
    return CDB_SUCCESS;
}

int CondDBTable::getTillTime(unsigned n_row, SimpleTime &till) 
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if (n_row > numRows)
	return CDB_RANGE_ERROR;
    
    if (n_row > tf.size())
	return tf.back(); //CDB_RANGE_ERROR;
    
    till = tf[n_row];
    return CDB_SUCCESS;
}
