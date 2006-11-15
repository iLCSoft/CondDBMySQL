// Our Resources administration
#include  <CDBResources.hxx>
#include  <ErrHdl.hxx>

// Our static Variable
CharString  CDBResources::dpName  = "";
CharString  CDBResources::parentf = "";
CharString  CDBResources::dbInit     = "";
CharString  CDBResources::configDP   = "condDBManager";


// Wrapper to read config file
void  CDBResources::init(int &argc, char *argv[])
{
  begin(argc, argv);
  while ( readSection() || generalSection() )
    ;
  end(argc, argv);
}


// Read the config file.
// Our section is [cdb], 
// our keyword is "dpName"
PVSSboolean  CDBResources::readSection()
{
  // Is it our section ? 
  // This will test for [cdb]
  if (!isSection("cdb"))
    return PVSS_FALSE;

  // Read next entry
  getNextEntry();

  // Loop thru section
  while ( (cfgState != CFG_SECT_START) &&  // Not next section
          (cfgState != CFG_EOF) )          // End of config file
  {
      // Compare keyword with "dpName"
      if (!keyWord.icmp("dpName"))             // It matches
      {
	  cfgStream >> dpName;                   // read the value
	  //cout << "Dpname: " << dpName << endl;
      }
      else if (!keyWord.icmp("parentf"))
      {
	  cfgStream >> parentf;
	  //cout << "parentf: " << parentf << endl;
      }
      else if (!keyWord.icmp("dbInit"))
      {
	  cfgStream >> dbInit;
	  //cout << "dbInit: " << dbInit << endl;
      }
      else if (!keyWord.icmp("configDP"))
      {
	  cfgStream >> configDP;
	  //cout << "configDP: " << configDP << endl;
      }
      else if (!readGeneralKeyWords())         // keywords handled in Resources
      {
	  ErrHdl::error(ErrClass::PRIO_WARNING,     // not that bad
			ErrClass::ERR_PARAM,        // wrong parametrization
			ErrClass::ILLEGAL_KEYWORD,  // illegal Keyword in Res.
			keyWord);
	  
	  // Signal error, so we stop later
	  cfgError = PVSS_TRUE;
      }
      
    getNextEntry();
  }

  // So the loop will stop at the end of the file
  return cfgState != CFG_EOF;
}












