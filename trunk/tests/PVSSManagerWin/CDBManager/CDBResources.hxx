#ifndef  CDBRESOURCES_H
#define  CDBRESOURCES_H


#include  <Resources.hxx>

class  CDBResources : public Resources
{
  public:
    // These functions initializes the manager
    static  void  init(int &argc, char *argv[]);  

    // Read the config section
    static  PVSSboolean  readSection();

  public:
    // Get the name of our Datapoint
    static const CharString & getDpName() {return dpName;}
    static const CharString getDbInit() {return dbInit;}
    static const CharString getParentFol() {return parentf;}
    static const CharString getConfigDP() {return configDP;}
  private:
    // The name of our Datapoint
    static CharString dpName;
    static CharString dbInit;
    static CharString parentf;
    static CharString configDP;
    
    DpIdentifier  dpHolder;      // dp ID
    DpIdentifier  configDPId;
    
};

#endif
