#!/bin/sh
#
# TCL Wrapper for ConditionsDB
# Lisbon ATLAS-DAQ Collaboration
# Ported from C++ to TCL by Jorge Lima 2003/04/15
#
#\
exec tclsh "$0" "$@"

package require conddb 1.0

#
# Define delete proc to mimic C++ delete operator
#
proc delete { obj } {
    rename $obj {}
}

proc printObj { condDBobj } {
    set since [$condDBobj validSince]
    set till [$condDBobj validTill]
    $condDBobj insertionTime insTime
    set layer [$condDBobj layer] 
    $condDBobj data data
    $condDBobj description desc
  
    puts "Interval      : since $since till $till"
    puts "insertionTime : $insTime"
    puts "        layer : $layer"
    puts "         data : $data"
    puts "  description : $desc"
}


proc printAllObjs { iterObjs } {
    set oCondObject [$iterObjs current]
    while { $oCondObject!=0 } {
	printObj $oCondObject
	puts ""
	delete $oCondObject;
	set oCondObject [$iterObjs next]
    }
}

#
# START THE MAIN PROGRAM HERE
#
set folder "/cal/temp"
set point 15

puts "DB initialization"    
set condDBmgr [CondDBMySQLMgrFactory::createCondDBMgr]
	
$condDBmgr init
# condDBmgr init "localhost:test_conddb_02:jmal:online"
	
$condDBmgr startRead
$condDBmgr openDatabase
$condDBmgr commit
	
set condDataAccess [$condDBmgr getCondDBDataAccess]

puts "Example - looking into folder $folder"
puts "Example - find of CondObj at point $point"

$condDBmgr startRead

$condDataAccess findCondDBObject oCondObject $folder $point

if { $oCondObject!=0 } {
    puts "Object found for CondDBKey = $point"
    printObj $oCondObject
    CondDBObjFactory::destroyCondDBObject $oCondObject
} else {
    puts "No object found for CondDBKey = $point"
}

$condDBmgr commit;

# Browsing

# Browsing vertically
puts "\nAll intervals at point: $point"


$condDBmgr startRead
$condDataAccess browseObjectsAtPoint iterObjs $folder $point
printAllObjs $iterObjs
delete $iterObjs;

# Browsing horizontally
puts "\nAll intervals in the HEAD:"

$condDataAccess browseObjectsInTag iterObjs $folder

if { $iterObjs == 0 } {
    puts "iterObjs is NULL"
} else {
    printAllObjs $iterObjs
    delete $iterObjs
}

$condDBmgr commit

CondDBMySQLMgrFactory::destroyCondDBMgr $condDBmgr




