#include  <CDBManager.hxx>
#include  <CDBResources.hxx>
#include  <signal.h>
#include <string>

int main(int argc, char *argv[])
{
  // Let Manager handle SIGINT and SIGTERM (Ctrl+C, kill)
  // Manager::sigHdl will call virtual function Manager::signalHandler 
	signal(SIGINT,  Manager::sigHdl);
	signal(SIGTERM, Manager::sigHdl);

  // Initialize Resources, i.e. 
  //  - interpret commandline arguments
  //  - interpret config file
	CDBResources::init(argc, argv);

  // Are we called with -helpDbg or -help ?
  if (CDBResources::getHelpDbgFlag())
  {
    CDBResources::printHelpDbg();
    return 0;
  }

  if (CDBResources::getHelpFlag())
  {
    CDBResources::printHelp();
    return 0;
  }

  // Now run our demo manager
	CDBManager *cdbManager = new CDBManager;
	cdbManager->run();

  // Exit gracefully :) 
  // Call Manager::exit instead of ::exit, so we can clean up
	
	Manager::exit(0);
	
  // Just make the compilers happy...
	return 0;
}




