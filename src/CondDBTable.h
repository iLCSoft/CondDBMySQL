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
/* $Id: CondDBTable.h,v 1.1 2006-11-15 14:04:44 poeschl Exp $ */

#ifndef CONDDBTABLE_H
#define CONDDBTABLE_H

#include "ConditionsDB/ICondDBTable.h"
#include <cstdlib>
#include <climits>
#include <cfloat>
#include<iostream>
#include<vector>
#include<string>

#ifndef WIN32
//# include <ansidecl.h>
#ifdef __GNUC__
# if __GNUC__ < 3
#  include <strstream>
#  define CONDDBTABLESTREAM strstream
#  define USE_OLD_STREAMS
# else
#  include <sstream>
#  define CONDDBTABLESTREAM std::ostringstream 
# endif
#else
#  include <sstream>
#  define CONDDBTABLESTREAM std::ostringstream 
#endif
#endif

/*MySql NULL constants*/
#define NULLINT -INT_MAX
#define NULLFLOAT -FLT_MAX
#define NULLDOUBLE -DBL_MAX

#ifdef _WIN32
#define NULLLONG -9223372036854775808
#else 
#if __GNUC__ < 3
#define NULLLONG -9223372036854775807LL
#elif defined(LONG_LONG_MAX)
#define NULLLONG -LONG_LONG_MAX
#elif defined(__LONG_LONG_MAX__)
/* Mac OS X & CYGWIN defintion */
#define NULLLONG -__LONG_LONG_MAX__
#endif
#endif
#ifdef _WIN32
#define atoi64(x) _atoi64(x)
#else
#define atoi64(x) atoll(x)
#endif

#ifdef WIN32
#  include <sstream>
#  define CONDDBTABLESTREAM std::ostringstream
#endif

#include <fstream>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef _WIN32
  typedef __int64 int64;
   inline std::ostream& operator<<( std::ostream& os , const __int64 ll)
{ 
  static char buf[128];

  sprintf(buf,"%-1.1I64d",ll); // Remove initial zeroes and left-justify
  std::string darg(buf);

  sprintf(buf,"%1.1I64x", ll); // Remove initial zeroes and left-justify
  std::string xarg(buf);

  // Return the desired representation
  int flg = os.flags();
  (flg & std::ios::hex) ? os<<xarg : os<<darg;
  return os;

}
#else
  typedef long long int64;
#endif

#include "ConditionsDB/SimpleTime.h"

using std::vector;
using std::string;



enum CondDBStatus {changed,
		   deleted,
		   inserted,
		   untouched};

class CondDBTable: public ICondDBTable{

 private:

// general members for data storage and retrieval
template <typename T, typename COLUMN>
    int __getNull(T &data, COLUMN *tmpColumn)
	{
	data = tmpColumn->column[0];
	return CDB_SUCCESS;
}

 
template <typename T, typename COLUMN>
    int __getCell(unsigned n_column, unsigned n_row, T &ndata, cdb_types type, COLUMN *tmpColumn)
	{
    
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if ((numRows > n_row) && (conddbtable.size() > n_column))
    {
	if (conddbtable[n_column]->type == type)
	{
	    if (tmpColumn->column.size() <= n_row)
		ndata = tmpColumn->column.back();
	    else
		ndata = tmpColumn->column[n_row+1];
	}
	
	else
	    return CDB_TYPE_ERROR;
    }
    else
	return CDB_RANGE_ERROR;
    
    return CDB_SUCCESS;
}

template <typename T, typename COLUMN>
    int __setCell(unsigned n_column, unsigned n_row, const T &ndata, cdb_types type, COLUMN *tmpColumn)
	{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    if ((numRows > n_row) && (conddbtable.size() > n_column))
    {
	if (conddbtable[n_column]->type == type)
	{
//	    static_cast<COLUMN*>(conddbtable[n_column])->column[n_row+1] = ndata;
	    if (tmpColumn->column.size() ==  (n_row+1))
		tmpColumn->column.push_back(ndata);
	    else
		if (tmpColumn->column.size() <  (n_row+1))
		{
		    T tmp = tmpColumn->column.back();
		    
		    while((n_row)-tmpColumn->column.size())
		    {
			tmpColumn->column.push_back(tmp);
		    }
		    tmpColumn->column.push_back(ndata);
		}
		else
		    // if the cell already has a value
		    tmpColumn->column[n_row+1] = ndata;
	 
	    return CDB_SUCCESS;
	}
	else
	    return  CDB_TYPE_ERROR; 
    }
    else
	return CDB_RANGE_ERROR;
}

template <typename T, typename COLUMN>
int __setColumnData(unsigned n_column, T &data, cdb_types type, COLUMN *tmpColumn)
{
    if (!isInitialized)
	return CDB_NOT_INITIALIZED;
    
    unsigned index = 0;
    if (n_column < conddbtable.size())
    {
	if (conddbtable[n_column]->type == type)
	{
	    while((tmpColumn->column.size() < numRows+1) && (index < data.size()))
	    {
		tmpColumn->column.push_back(data[index]);
		index++;
	    }
	    return index;
	    
	}
	else
	    return CDB_TYPE_ERROR;
    }
    else
	return CDB_RANGE_ERROR;
}



// structures of the columns
    struct CondDBColumn{
	
	bool initialized;
	string name;
	cdb_types type;
	
    };
    
    struct CondDBColumnBool:CondDBColumn
    {
	vector<short int> column;	
    };

    struct CondDBColumnLong:CondDBColumn
    {
	vector<int64> column;
    };
    

    struct CondDBColumnInt:CondDBColumn
    {
	vector<long int> column;
    };

    struct CondDBColumnFloat:CondDBColumn
    {
	vector<float> column;    
    };
    
    struct CondDBColumnString:CondDBColumn
    {
	vector<string> column;    
    };

    struct CondDBColumnDouble:CondDBColumn
    {
	vector<double> column;    
    };

    struct CondDBColumnArrayBool:CondDBColumn
    {
	vector<vector<short int> > column;	
    };

    struct CondDBColumnArrayLong:CondDBColumn
    {
	vector<vector<int64> > column;    
    };

    struct CondDBColumnArrayInt:CondDBColumn
    {
	vector<vector<long int> > column;    
    };

    struct CondDBColumnArrayFloat:CondDBColumn
    {
	vector<vector<float> > column;    
    };

    struct CondDBColumnArrayString:CondDBColumn
    {
	vector<vector<string> > column;    
    };

    struct CondDBColumnArrayDouble:CondDBColumn
    {
	vector<vector<double> > column;    
    };

 public:

/**
 * Default constructor. 
 * 
 * @param n_columns is the number of columns of the table
 * @param n_rows is the number of rows that must be reserved (OPTIONAL)
*/
   CondDBTable(){isInitialized = false; numRows=0;}
    
/**
 * Default constructor. 
 * 
 * @param n_columns is the number of columns of the table
 * @param n_rows is the number of rows that must be reserved (OPTIONAL)
*/
   CondDBTable(unsigned n_columns, int n_rows = 0);
    
/**
 * Object destructor
*/
   virtual ~CondDBTable();
    
/**
 * This method gets a vector containing the names of all columns in the table
 * 
 * @param names is the reference to the vector returned
 * @return an error code 
*/
   virtual int getNames(vector<string> &names) ;
    
/**
 * This method gets a vector containing the data types of all columns in the table
 * 
 * @param types is the reference to the vector returned
 * @return an error code 
*/
   virtual int getTypes(vector<cdb_types> &types) ;

/**
 * Get Null value for column
 */

   virtual int getNull(unsigned n_column, float &null);
   virtual int getNull(unsigned n_column, long int &null);
   virtual int getNull(unsigned n_column, int64 &null);
   virtual int getNull(unsigned n_column, double &null);
   virtual int getNull(unsigned n_column, string &null);   

/**
 * This method defines the name for a specified column
 * 
 * @param n_column is the column number
 * @param name is a string with the name
 * @return an error code 
*/
   virtual int setName(unsigned n_column, const string& name);

/**
 * This method defines the data type for a specified column
 * 
 * @param n_column is the column number
 * @param type is a the type
 * @return an error code 
*/
   virtual int setType(unsigned n_column, cdb_types type);

/**
 * Set Null value for column
 */

   virtual int setNull(unsigned n_column, const float &null);
   virtual int setNull(unsigned n_column, const long int &null);
   virtual int setNull(unsigned n_column, const int64 &null);
   virtual int setNull(unsigned n_column, const double &null);
   virtual int setNull(unsigned n_column, const string &null);

/**
 * This method initializes a table created by the default constructor
 * If the table had the number of columns already defined this does nothing
 *
 * @param n_columns is the number of columns of this table
 * @param n_rows is the number of rows to allocate (OPTIONAL)
*/
    virtual void Initialize(unsigned n_columns, int n_rows=0);


/**
 *  This method returns a vector of strings with allelements in a row
 *  In case the cell has an array the first string has the number of elements (following strings) belonging to the array
 *
 * @param rowNumber is the number of the row
 * @param values is the string that will be returned with the row elements
 * @return an error code that is to be defined yet
 */
    virtual int getRow(unsigned rowNumber, vector<string> &values) ;

///////////////////////////////////////////////////////////
// The following methods get the value of a specified cell
//////////////////////////////////////////////////////////

/**
 * This method gets a value from a cell in a column of long ints (int in MySQL)
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, short int &ndata) ;

/**
 * This method gets a value from a cell in a column of long ints (int in MySQL)
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, long int &ndata) ;

/**
 * This method gets a value from a cell in a column of long long ints (bigint in MySQL)
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, int64 &ndata) ;

/**
 * This method gets a value from a cell in a column of floats
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, float &ndata) ;

/**
 * This method gets a value from a cell in a column of strings
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, string &ndata) ;

/**
 * This method gets a value from a cell in a column of doubles
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, double &ndata) ;

/**
 * This method gets a value from a cell in a column of vectors of bools
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, vector<short int> &ndata) ;

/**
 * This method gets a value from a cell in a column of vectors of long ints (int in MySQL - 4bytes)
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, vector<long int> &ndata) ;

/**
 * This method gets a value from a cell in a column of vectors of long long ints (bigint in MySQL - 8bytes)
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, vector<int64> &ndata) ;

/**
 * This method gets a value from a cell in a column of vectors of floats
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, vector<float> &ndata) ;

/**
 * This method gets a value from a cell in a column of vector of strings
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, vector<string> &ndata) ;

/**
 * This method gets a value from a cell in a column of vector of doubles
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
    virtual int getCell(unsigned n_column, unsigned n_row, vector<double> &ndata) ;



//! set data methods

/**
 * These methods allow to chage a value in a specified cell
 */

/**
 * This method changes a value from a cell in a column of long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const short int ndata);

/**
 * This method changes a value from a cell in a column of long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const long int ndata);

/**
 * This method changes a value from a cell in a column of long long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const int64 ndata);

/**
 * This method changes a value from a cell in a column of floats
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const float ndata);

/**
 * This method changes a value from a cell in a column of strings
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const string ndata);

/**
 * This method changes a value from a cell in a column of doubles
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const double ndata);

/**
 * This method changes a value from a cell in a column of vectors of bools
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const vector<short int> &ndata);

/**
 * This method changes a value from a cell in a column of vectors of long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const vector<long int> &ndata);

/**
 * This method changes a value from a cell in a column of vectors of long long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const vector<int64> &ndata);

/**
 * This method changes a value from a cell in a column of vectors of floats
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const vector<float> &ndata);

/**
 * This method changes a value from a cell in a column of vectors of strings
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const vector<string> &ndata);

/**
 * This method changes a value from a cell in a column of vectors of doubles
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
    virtual int setCell(unsigned n_column, unsigned n_row, const vector<double> &ndata);


/**
 * The following methods allow to insert in the columns a group of values.
 * The values are passed in a vector.
 * In case that there are not enougth rows reserved to insert the values there
 * will be only inserted data until all rows are filled and return the number of values
 * from the vector passed.
 */

/**
 * Insert a subset in a column of bools
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<short int> &data);

/**
 * Insert a subset in a column of long ints
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<long int> &data);

/**
 * Insert a subset in a column of long long ints
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<int64> &data);

/**
 * Insert a subset in a column of floats
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<float> &data);

/**
 * Insert a subset in a column of strings
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<string> &data);

/**
 * Insert a subset in a column of doubles
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<double> &data);

/**
 * Insert a subset in a column of vectors of bools
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<vector<short int> > &data);

/**
 * Insert a subset in a column of vectors of long ints
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<vector<long int> > &data);

/**
 * Insert a subset in a column of vectors of long long ints
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<vector<int64> > &data);

/**
 * Insert a subset in a column of vectors of floats
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<vector<float> > &data);

/**
 * Insert a subset in a column of vectors of strings
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<vector<string> > &data);

/**
 * Insert a subset in a column of vectors of doubles
 *
 * @param n_column Refers to the column number
 * @param data refers to the values to be appended in the column
 * @return the number of values inserted 
 * @see getNames(); getTypes()
 */
    virtual int setColumndata(unsigned n_column, const vector<vector<double> > &data);


/**
 * Reserves more rows on the table. This method must be called at each time that 
 * one wants to insert new data on the table reserving only the number of rows that will be used! 
 *
 * @param num_rows is the amount of rows that must be reserved in the table
 */
    virtual void resize(int num_rows);

/**
 * Returns the number of rows *RESERVED* in the table.
 * The user must know if all rows reserved are already assigned with values
 *
 * @return number of rows reserved.
 */
    virtual unsigned getNumRows()  {return numRows;}

/**
 * Returns the number of columns in the table.
 * The user must know if all rows reserved are already assigned with values
 *
 * @return number of rows reserved.
 */
    virtual unsigned getNumColumns()  {return conddbtable.size();}


///////////////////////////////////////
// Time handling methods - not for now
///////////////////////////////////////

/**
 * Inserts in the column of the begin of time interval the value of time.
 *
 * @param since is the value to append to the end of the column
 * @return error code.
 * @see setColumndata
 */
    virtual int setSinceTime(const SimpleTime &since);

/**
 * Inserts in the column of the begin of time interval the value of time passed in the vector.
 *
 * @param since is the vector of values to append to the end of the column
 * @return error code.
 * @see setColumndata
 */
    virtual int setSinceTime(const vector<SimpleTime> &since);

/**
 * Changes the value of time in the now specified. If the row has not a value yet returns an error code (CDB_ALLOC_ERROR)
 *
 * @param n_row is the number of the row which since time must be chaged
 * @param since is the new value to replace
 * @return error code.
 * @see changeColumndata
 */
    virtual int changeSinceTime(unsigned n_row, const SimpleTime &since);

/**
 * Changes the value of time in the now specified. If the row has not a value yet returns an error code (CDB_ALLOC_ERROR)
 *
 * @param n_row is the number of the row which since time must be chaged
 * @param till is the new value to replace
 * @return error code.
 * @see changeColumndata
 */
    virtual int changeTillTime(unsigned n_row,const  SimpleTime &till);
    
/**
 * Inserts in the column of the end of time interval the value of time.
 *
 * @param till is the value to append to the end of the column
 * @return error code.
 * @see setColumndata
 */
    virtual int setTillTime(const SimpleTime &till);

/**
 * Inserts in the column of the begin of time interval the value of time passed in the vector.
 *
 * @param till is the vector of values to append to the end of the column
 * @return error code.
 * @see setColumndata
 */
    virtual int setTillTime(const vector<SimpleTime> &till);

/**
 * Get the value of time from a specified row
 *
 * @param n_row is the number of row to get the value
 * @param since is the reference to the variable where the value will be returned
 * @return error code.
 */
    virtual int getSinceTime(unsigned n_row, SimpleTime &since) ;
    
/**
 * Get the value of time from a specified row
 *
 * @param n_row is the number of row to get the value
 * @param till is the reference to the variable where the value will be returned
 * @return error code.
 */
    virtual int getTillTime(unsigned n_row, SimpleTime &till) ;


/**
 * Generates a user class based on the structure of current table.
 *
 * @param name is the name that should be given to the class and corresponding files
 * @param path is the path where the files should be stored (defaults to CONDDBROOT/src/userclass)
 * @return error code.
 */

//    virtual int generateClass(string name, string path) ;
  
 private:

/**
 * Verifies if all columns are initialized
 **/
    void verifyInitialization();

/**
 * Auxiliary function to convert any value to string
 *
 * @param t is the value to be converted
 * @return a string with the value passed.
 **/
    template <class TYPE> 
	std::string ToString( const TYPE & t ) {
	CONDDBTABLESTREAM os;
	os << t;
#ifdef USE_OLD_STREAMS
	string tmpstr(os.str(),os.pcount());
	return tmpstr;
#else
        return os.str();
#endif	
    }
    
    bool isInitialized;
    vector<CondDBColumn*> conddbtable;
    unsigned numRows;

    vector<SimpleTime> ti;
    vector<SimpleTime> tf;
    
    vector<CondDBStatus> status;
    
};

#endif /*CONDDBTABLE_H*/



