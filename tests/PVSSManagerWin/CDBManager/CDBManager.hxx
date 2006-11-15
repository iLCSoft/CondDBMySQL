#ifndef  CDBMANAGER_H
#define  CDBMANAGER_H

#include   <Manager.hxx>        // include/Manager
#include   <DpIdentifier.hxx>   // include/Basics
#include   <string>
#include   <vector>

class CDBManager : public Manager
{
  public:
    // Default constructor
    CDBManager();

    // Run the Manager
    void run();

    // handle incoming hotlinks by group
	std::vector < std::vector<std::string> > handleHotLinkforConnect (const DpHLGroup &group);
	std::vector<std::string> handleHotLinkforContainer (const DpHLGroup &group);
	std::string handleHotLinkforConfig (const DpHLGroup &group);
    bool handleHotLinkforStart(const DpHLGroup &group);
	bool handleHotLinkforStatus(const DpHLGroup &group);
	std::string handleHotLinkParent(const DpHLGroup &group);

	static PVSSboolean doExit;


  private:
    // our exit flag. The signal handler will set it to PVSS_TRUE
  	
    // callback from signal handler
    virtual void signalHandler(int sig);

    // private Variables
    DpIdentifier  dpHolder;   //DP which contains the list of dps 
    DpIdentifier  configDPId;
};

#endif
