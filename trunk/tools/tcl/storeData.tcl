#!/bin/sh
#
# TCL Wrapper for ConditionsDB
# Lisbon ATLAS-DAQ Collaboration
# Ported from C++ to TCL by Jorge Lima 2003/04/15
#
#\
exec tclsh "$0" "$@"

package require conddb 1.0

# Basic example of data storage.

# this requires the previous run of example basicSession

set folder "/cal/temp"

puts "Example -  DB initialization"
    
set condDBmgr [CondDBMySQLMgrFactory::createCondDBMgr]
  
$condDBmgr init
# $condDBmgr init "localhost:test_conddb_02:jmal:online"
$condDBmgr startRead
$condDBmgr openDatabase
$condDBmgr commit

set condDataAccess [$condDBmgr getCondDBDataAccess]
set condFolderMgr [$condDBmgr getCondDBFolderMgr]
 
puts "Example -  CondFolder creation"

$condDBmgr startUpdate
$condFolderMgr createCondDBFolder $folder "" "" true 
$condDBmgr commit
    
puts "Example -  storage of CondObject in folder $folder"
    
$condDBmgr startUpdate
    
set condObject [CondDBObjFactory::createCondDBObject \
	3 10 "sample data value \n\nobject 3-10" "sample description"]

$condDataAccess storeCondDBObject $folder $condObject
	
CondDBObjFactory::destroyCondDBObject $condObject
	
set condObject [CondDBObjFactory::createCondDBObject \
	10 20 "sample data value object 10-20" "sample description"]

$condDataAccess storeCondDBObject $folder $condObject

CondDBObjFactory::destroyCondDBObject $condObject
    
$condDBmgr commit
    
CondDBMySQLMgrFactory::destroyCondDBMgr $condDBmgr
    


