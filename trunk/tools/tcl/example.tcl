#! /bin/sh
#\
exec tclsh "$0" "$@"


load ./tclconddb.so conddb


# basicSession

set mgr [CondDBMgrFactory::create]

$mgr init

$mgr startUpdate
$mgr createCondDB
$mgr commit
	
CondDBMgrFactory::destroy $mgr

# storeData

set mgr [CondDBMgrFactory::create]

puts "Init with default parameters"

set folder "/LHCb/ATLAS/MuonCal/Align01"
$mgr init
# $mgr init "localhost:test_conddb_02:jmal:online"
    
$mgr startRead
$mgr openDatabase
$mgr commit

set dataAccess [$mgr getDataMgr]
set folderMgr [$mgr getFolderMgr] 

$mgr startUpdate    
$folderMgr createCondDBFolder $folder "" "" true 
$mgr commit
    
puts "Example -  storage of CondObject in folder $folder"
    
$mgr startUpdate
    
# First object
    
array set cObj {
    since 3
    till  10
    data  "sample data value \n\nobject 3-10"
    desc  "sample description"
}


$dataAccess storeCondDBObject $folder cObj
unset cObj

# Second object
	
array set cObj {
    since 10
    till  20
    data  "sample data value object 10-20"
    desc  "sample description"
}

$dataAccess storeCondDBObject $folder cObj
unset cObj
    
$mgr commit
    
CondDBMgrFactory::destroy $mgr
    










