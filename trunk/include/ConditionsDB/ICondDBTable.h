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
/* $Id: ICondDBTable.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $ */

#ifndef ICONDDBTABLE_H
#define ICONDDBTABLE_H

#include<vector>
#include<string>
#include "ConditionsDB/SimpleTime.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef _WIN32
  typedef __int64 int64;
#else
  typedef long long int64;
#endif

using std::vector;
using std::string;

class ICondDBTable{

 

 public:

    enum cdb_types {cdbNull=0,
		    cdbBool,
	            cdbInt, 
		    cdbFloat, 
		    cdbString,
		    cdbDouble,
		    cdbLongLong,
		    cdbArrayBool,
		    cdbArrayFloat, 
		    cdbArrayInt,
		    cdbArrayString,
		    cdbArrayDouble,
		    cdbArrayLongLong
    };
    
 
    
    enum CondDBErrorCode {CDB_NOT_INITIALIZED = -1,
			  CDB_RANGE_ERROR = -2,
			  CDB_TYPE_ERROR = -3,
			  CDB_SUCCESS = -4,
			  CDB_ALLOC_ERROR = -5,
			  CDB_STRTOARRAY_ERROR = -6};


/**
 * Object destructor
*/
  virtual ~ICondDBTable() {}
    
/**
 * This method gets a vector containing the names of all columns in the table
 * 
 * @param names is the reference to the vector returned
 * @return an error code 
*/
  virtual int getNames(vector<string> &names) = 0;
    
/**
 * This method gets a vector containing the data types of all columns in the table
 * 
 * @param types is the reference to the vector returned
 * @return an error code 
*/
  virtual int getTypes(vector<cdb_types> &types) = 0;

/**
 * Get Null value for column
 */

   virtual int getNull(unsigned n_column, float &null) = 0;
   virtual int getNull(unsigned n_column, long int &null) = 0;
   virtual int getNull(unsigned n_column, int64 &null) = 0;
   virtual int getNull(unsigned n_column, double &null) = 0;
   virtual int getNull(unsigned n_column, string &null) = 0; 

/**
 * This method defines the name for a specified column
 * 
 * @param n_column is the column number
 * @param name is a string with the name
 * @return an error code 
*/
  virtual int setName(unsigned n_column, const string& name) = 0;

/**
 * This method defines the data type for a specified column
 * 
 * @param n_column is the column number
 * @param type is a the type
 * @return an error code 
*/
  virtual int setType(unsigned n_column, cdb_types type) = 0;

/**
 * Set Null value for column
 */

  virtual int setNull(unsigned n_column, const float &null) = 0;
  virtual int setNull(unsigned n_column, const long int &null) = 0;
  virtual int setNull(unsigned n_column, const int64 &null) = 0;
  virtual int setNull(unsigned n_column, const double &null) = 0;
  virtual int setNull(unsigned n_column, const string &null) = 0;

    
/**
 * This method initializes a table created by the default constructor
 * If the table had the number of columns already defined this does nothing
 *
 * @param n_columns is the number of columns of this table
 * @param n_rows is the number of rows to allocate (OPTIONAL)
*/
  virtual void Initialize(unsigned n_columns, int n_rows=0) = 0;

/**
 *  This method returns a vector of strings with allelements in a row
 *  In case the cell has an array the first string has the number of elements (following strings) belonging to the array
 *
 * @param rowNumber is the number of the row
 * @param values is the string that will be returned with the row elements
 * @return an error code that is to be defined yet
 */
  virtual int getRow(unsigned rowNumber, vector<string> &values) = 0;

///////////////////////////////////////////////////////////
// The following methods get the value of a specified cell
///////////////////////////////////////////////////////////

/**
 * This method gets a value from a cell in a column of bools
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, short int &ndata) = 0;

/**
 * This method gets a value from a cell in a column of long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, long int &ndata) = 0;

/**
 * This method gets a value from a cell in a column of long long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, int64 &ndata) = 0;

/**
 * This method gets a value from a cell in a column of floats
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, float &ndata) = 0;

/**
 * This method gets a value from a cell in a column of strings
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, string &ndata) = 0;

/**
 * This method gets a value from a cell in a column of doubles
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, double &ndata) = 0;

/**
 * This method gets a value from a cell in a column of vectors of bools
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, vector<short int> &ndata) = 0;

/**
 * This method gets a value from a cell in a column of vectors of long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, vector<long int> &ndata) = 0;

/**
 * This method gets a value from a cell in a column of vectors of long long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, vector<int64> &ndata) = 0;

/**
 * This method gets a value from a cell in a column of vectors of floats
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, vector<float> &ndata) = 0;

/**
 * This method gets a value from a cell in a column of vector of strings
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, vector<string> &ndata) = 0;

/**
 * This method gets a value from a cell in a column of vector of doubles
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the reference to the valriable where the value will be returned 
 * @return an error code 
*/
  virtual int getCell(unsigned n_column, unsigned n_row, vector<double> &ndata) = 0;



//! set data methods

/**
 * These methods allow to chage a value in a specified cell
 */

/**
 * This method changes a value from a cell in a column of bools
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const short int ndata) = 0;

/**
 * This method changes a value from a cell in a column of long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const long int ndata) = 0;

/**
 * This method changes a value from a cell in a column of long long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const int64 ndata) = 0;

/**
 * This method changes a value from a cell in a column of floats
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const float ndata) = 0;

/**
 * This method changes a value from a cell in a column of strings
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const string ndata) = 0;

/**
 * This method changes a value from a cell in a column of doubles
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new value to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const double ndata) = 0;

/**
 * This method changes a value from a cell in a column of vectors of bools
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const vector<short int> &ndata) = 0;

/**
 * This method changes a value from a cell in a column of vectors of long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const vector<long int> &ndata) = 0;

/**
 * This method changes a value from a cell in a column of vectors of long long ints
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const vector<int64> &ndata) = 0;

/**
 * This method changes a value from a cell in a column of vectors of floats
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const vector<float> &ndata) = 0;
    
/**
 * This method changes a value from a cell in a column of vectors of strings
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const vector<string> &ndata) = 0;

/**
 * This method changes a value from a cell in a column of vectors of doubles
 * 
 * @param n_column is the column number
 * @param n_row is the row number
 * @param ndata is the new array to be assigned
 * @return an error code 
*/
  virtual int setCell(unsigned n_column, unsigned n_row, const vector<double> &ndata) = 0;

/**
 * Reserves more rows on the table. This method must be called at each time that 
 * one wants to insert new data on the table reserving only the number of rows that will be used! 
 *
 * @param num_rows is the amount of rows that must be reserved in the table
 */
  virtual void resize(int num_rows) = 0;

/**
 * Returns the number of rows *RESERVED* in the table.
 * The user must know if all rows reserved are already assigned with values
 *
 * @return number of rows reserved.
 */
  virtual unsigned getNumRows() = 0;

/**
 * Returns the number of columns in the table.
 * The user must know if all rows reserved are already assigned with values
 *
 * @return number of rows reserved.
 */
  virtual unsigned getNumColumns() = 0;


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
  virtual int setSinceTime(const SimpleTime &since) = 0;

/**
 * Inserts in the column of the begin of time interval the value of time passed in the vector.
 *
 * @param since is the vector of values to append to the end of the column
 * @return error code.
 * @see setColumndata
 */
  virtual int setSinceTime(const vector<SimpleTime> &since) = 0;

/**
 * Changes the value of time in the now specified. If the row has not a value yet returns an error code (CDB_ALLOC_ERROR)
 *
 * @param n_row is the number of the row which since time must be chaged
 * @param since is the new value to replace
 * @return error code.
 * @see changeColumndata
 */
  virtual int changeSinceTime(unsigned n_row, const SimpleTime &since) = 0;

/**
 * Changes the value of time in the now specified. If the row has not a value yet returns an error code (CDB_ALLOC_ERROR)
 *
 * @param n_row is the number of the row which since time must be chaged
 * @param till is the new value to replace
 * @return error code.
 * @see changeColumndata
 */
  virtual int changeTillTime(unsigned n_row, const SimpleTime &till) = 0;
    
/**
 * Inserts in the column of the end of time interval the value of time.
 *
 * @param till is the value to append to the end of the column
 * @return error code.
 * @see setColumndata
 */
  virtual int setTillTime(const SimpleTime &till) = 0;
    
/**
 * Inserts in the column of the begin of time interval the value of time passed in the vector.
 *
 * @param till is the vector of values to append to the end of the column
 * @return error code.
 * @see setColumndata
 */
  virtual int setTillTime(const vector<SimpleTime> &till) = 0;

/**
 * Get the value of time from a specified row
 *
 * @param n_row is the number of row to get the value
 * @param since is the reference to the variable where the value will be returned
 * @return error code.
 */
    virtual int getSinceTime(unsigned n_row, SimpleTime &since) = 0;
    
/**
 * Get the value of time from a specified row
 *
 * @param n_row is the number of row to get the value
 * @param till is the reference to the variable where the value will be returned
 * @return error code.
 */
    virtual int getTillTime(unsigned n_row, SimpleTime &till) = 0;
/**
 * Generates a user class based on the structure of current table.
 *
 * @param name is the name that should be given to the class and corresponding files
 * @param path is the path where the files should be stored (defaults to CONDDBROOT/src/userclass)
 * @return error code.
 */

//    virtual int generateClass(string name, string path) ;
  
    
};

#endif /*ICONDDBTABLE_H*/



