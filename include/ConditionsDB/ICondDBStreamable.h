//                         -*- Mode: C++ -*- 
// ICondDBStreamable.h --
// created on Thu Feb  1 18:29:01 2001
// by         Dirk Duellmann 
//
// $Id: ICondDBStreamable.h,v 1.1 2006-11-15 14:02:19 poeschl Exp $
//
#ifndef ICondDBStreamable_h
#define ICondDBStreamable_h

#include <string>

class ICondDBStreamable {
public:
  /// Virtual destructor
  virtual ~ICondDBStreamable(){};

  virtual void storeToDB(std::string &s) const = 0;
  virtual void retrieveFromDB(const std::string &s) = 0;
};

#endif // ICondDBStreamable_h

