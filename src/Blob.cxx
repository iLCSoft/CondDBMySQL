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
/* $Id: Blob.cxx,v 1.2 2009-11-30 10:10:22 meyern Exp $ */

#include "Blob.h"
#include <iostream>
#include <cstring>
#ifndef _WIN32
#include "config.h"
#define atoi64(x) atoll(x)
#else
#define atoi64(x) _atoi64(x)
#endif

template <class T>
void makeBlob(T *array, std::string *Blob, long size)
{
    long sizetmp=size;
    long *itmp=&sizetmp;

    char *numEl=(char*)itmp;
    char *blob=(char*)array;

#ifdef CONDDB_BIGENDIAN
    char tmp;
    
    for (int c=0; c<sizeof(long) ; c++)
    {
	tmp=numEl[c];
	numEl[c]=numEl[sizeof(long)-c-1];
	numEl[sizeof(long)-c-1]=tmp;
    }

    for (int s=0; s<size*sizeof(T); s+=sizeof(T))
    {
	char t[sizeof(T)+1];
	t[4]=0;
	for (int q=0; q<sizeof(T);q++)
	    t[q]=blob[s+sizeof(T)-q-1];
	for (int q=0; q<sizeof(T);q++)
	    blob[s+q]=t[q];
    }
#endif
		       
    *Blob=std::string(numEl, sizeof(long));
    *Blob+=std::string(blob, sizeof(T)*size);

}

void vectorToBlob(std::string *blob, std::vector <std::string>& data, CondDBTable::cdb_types type) 
{
    switch (type)
    {
	case CondDBTable::cdbArrayFloat: 
	{
	    float* arr;
	    arr = (float*) malloc (sizeof(float) * data.size());
	    for (unsigned i=data.size(); i>0;i--)
		arr[data.size()-i]=atof(data[i-1].c_str());
	    makeBlob(arr, blob, data.size());
	    free(arr);
	    break;
	}
	case CondDBTable::cdbArrayInt: 
	{
	    int* arr;
	    arr = (int*) malloc (sizeof(int)*data.size());
	    for (unsigned i=data.size(); i>0;i--)
		arr[data.size()-i]=atoi(data[i-1].c_str());
	    makeBlob(arr, blob, data.size());
	    free(arr);
	    break;
	}
	case CondDBTable::cdbArrayDouble: 
	{
	    double* arr;
	    arr = (double*) malloc (sizeof(double)*data.size());
	    for (unsigned i=data.size(); i>0;i--)
		arr[data.size()-i]=atof(data[i-1].c_str());
	    makeBlob(arr, blob, data.size());
	    free(arr);
	    break;
	}
	case CondDBTable::cdbArrayLongLong:
	{
	    int64* arr;
	    arr = (int64*) malloc (sizeof(int64)*data.size());
	    for (unsigned i=data.size(); i>0;i--)
		arr[data.size()-i]=atoi64(data[i-1].c_str());
	    makeBlob(arr, blob, data.size());
	    free(arr);
	    break;
	}
	case CondDBTable::cdbArrayBool:
	{
	    short int *arr;
	    arr = (short int*) malloc (sizeof(short int)*data.size());
	    for (unsigned i=data.size(); i>0;i--)
		arr[data.size()-i]=atoi(data[i-1].c_str());
	    makeBlob(arr, blob, data.size());
	    free(arr);
	    break;
	}
	case CondDBTable::cdbArrayString:
	{
	    //STRING
	    unsigned size=data.size();
	    long sizetmp=size;
	    long *itmp=&sizetmp;
	    char *numEl=(char*)itmp;
	    
#ifdef CONDDB_BIGENDIAN
	    char tmp;
	    
	    for (int c=0; c<sizeof(long) ; c++)
	    {
		tmp=numEl[c];
		numEl[c]=numEl[sizeof(long)-c-1];
		numEl[sizeof(long)-c-1]=tmp;
	    }
#endif
	    
	    *blob=std::string(numEl, sizeof(long));
	    for (unsigned i=size; i>0; i--)
	    {
		long length=data[i-1].size();
		long lengthtmp=length;
		long *ltemp=&lengthtmp;
		char *blength=(char*)ltemp;
		
#ifdef CONDDB_BIGENDIAN
		char tmp;
		
		for (int c=0; c<sizeof(long) ; c++)
		{
		    tmp=blength[c];
		    blength[c]=blength[sizeof(long)-c-1];
		    blength[sizeof(long)-c-1]=tmp;
		}
		
#endif
		*blob+=std::string(blength, sizeof(long));
		*blob+=data[i-1];
	    }
	    break;
	}
	default : break;
    }
}

template <class T>
void makeVector (T *val, std::vector <std::string> &data, long size)
{
    for(long i=0; i<size; i++)
    { 
	MYSQLSTREAM os;
	os << val[i];
	data.insert(data.begin(),os.str());
    }
}


void blobToVector (char *Blob, std::vector <std::string>& data, CondDBTable::cdb_types type)
{
    char *nEl;
    nEl = (char*) malloc (sizeof(long));
    memcpy (nEl, Blob, sizeof(long));

#ifdef CONDDB_BIGENDIAN
    char temp;
    
    for (int c=0; c<sizeof(long) ; c++)
    {
	temp=nEl[c];
	nEl[c]=nEl[sizeof(long)-c-1];
	nEl[sizeof(long)-c-1]=temp;
    }

#endif
    
    long *tmp=(long*)nEl;
    unsigned size=tmp[0];
    free(nEl);
    Blob +=sizeof(long);

#ifdef CONDDB_BIGENDIAN
 
    int swapsize;
    switch (type)
    {
	case CondDBTable::cdbArrayFloat: { swapsize=sizeof(float); break; }
	case CondDBTable::cdbArrayInt: { swapsize=sizeof(int); break; }
	case CondDBTable::cdbArrayDouble: { swapsize=sizeof(double); break; }
	case CondDBTable::cdbArrayLongLong: { swapsize=sizeof(int64); break;}
	case CondDBTable::cdbArrayBool: { swapsize=sizeof(short); break;}
    }
    if (type != CondDBTable::cdbArrayString)
    {
	for (int s=0; s<size*swapsize; s+=swapsize)
	{
	    char *t = new char(swapsize);
	    for (int q=0; q<swapsize;q++)
		t[q]=Blob[s+swapsize-q-1];
	    for (int q=0; q<swapsize;q++)
		Blob[s+q]=t[q];
	}
    }
#endif
    switch (type)
    {
	case CondDBTable::cdbArrayFloat:
	{
	    float* arr;
	    arr = (float*) Blob;
	    makeVector ( arr, data, size);
	    break;
	}
	case CondDBTable::cdbArrayInt:
	{
	    int* arr;
	    arr = (int*) Blob;
	    makeVector ( arr, data, size);
	    break;
	}
	case CondDBTable::cdbArrayDouble:
	{
	    double* arr;
	    arr = (double*) Blob;
	    makeVector ( arr, data, size);
	    break;
	}
	case CondDBTable::cdbArrayLongLong:
	{
	    int64 *arr;
	    arr = (int64*) Blob;
	    makeVector (arr, data, size);
	    break;
	}
	case CondDBTable::cdbArrayBool:
	{
	    short int* arr;
	    arr = (short int*) Blob;
	    makeVector ( arr, data, size);
	    break;
	}
	case CondDBTable::cdbArrayString:
	{
	    for (unsigned i=0; i<size; i++)
	    {
		char *blength = (char*)malloc(sizeof(long));
		memcpy (blength, Blob, sizeof(long));
		
#ifdef CONDDB_BIGENDIAN
		char temp;
		
		for (int c=0; c<sizeof(long) ; c++)
		{
		    temp=blength[c];
		    blength[c]=blength[sizeof(long)-c-1];
		    blength[sizeof(long)-c-1]=temp;
		}
#endif
		Blob+=sizeof(long);
		long *ltemp=(long*)blength;
		long length=ltemp[0];
		free(blength);
		char *line = (char*)malloc(length);
		memcpy (line, Blob, length);
		data.insert(data.begin(),std::string(line, length));
		free(line);
		Blob+=length;
	    }
	}
	default : break;
    }
}
