//                         -*- Mode: C++ -*- 
// CondDBVector.h --
// created on Thu Feb  1 18:29:34 2001
// by         Dirk Duellmann 
//
// $Id: CondDBVector.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $
//
#ifndef CondDBVector_h
#define CondDBVector_h

#include "ConditionsDB/ICondDBStreamable.h"
#include <vector>

template <class T>
class CondDBVector: public std::vector<T>, public ICondDBStreamable {
public:
  virtual ~CondDBVector() 
    {}
  void storeToDB(std::string &s) const
    { 
      // check for platform and do byte swapping
      s = std::string(reinterpret_cast<const char *>(& (this->operator[](0))), this->size()*sizeof (T)); 
    }

  void retrieveFromDB(const std::string &s)
    {
      size_t elems   = s.size() / sizeof (T);
      const T *beg = reinterpret_cast<const T *> (s.c_str());
      this->resize(elems);
      copy(beg,beg+elems,this->begin());
    }
};

#endif // CondDBVector_h

