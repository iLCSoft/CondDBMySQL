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

set folder "/cal/temp"

puts "Testing -  DB initialization"
    
set condDBmgr [CondDBMySQLMgrFactory::createCondDBMgr]
  
# LIGACAO A BASE DE DADOS
$condDBmgr init
$condDBmgr startRead
$condDBmgr openDatabase
$condDBmgr commit

set condDataAccess [$condDBmgr getCondDBDataAccess]
set condFolderMgr [$condDBmgr getCondDBFolderMgr] 
puts "Example -  CondFolder verification"

$condDBmgr startUpdate

puts "Verification of existence of folder : $folder"

if {![$condFolderMgr exist $folder]} {
    puts "CondDB folder does not exist \nProceding to cretion of CondDBFolder $folder"
    $condFolderMgr createCondDBFolder $folder "" "" true
    # add the system name of an existing DB if you wish
    $condDBmgr commit
} else {
    puts "CondDBFolder already exists...proceding!"
    # PARTE DISPENSAVEL: LISTA TODOS OS FOLDERS EXISTENTES NA BD
    # Dump the list of stored CondFolder
    #puts "Example - all stored CondFolder:"
    #$condDBmgr startRead
    #puts "Ainda por implementar! (describe)"
    #$condFolderMgr describe "/"
    #$condDBmgr commit
}

puts "Example -  storage of CondObject in folder: $folder"
    
$condDBmgr startUpdate
    
puts "How many objects do you want to create?"
set num [gets stdin]

puts "Performance:"
puts "$conddb_perf_measurement"
set conddb_perf_measurement 1
puts "$conddb_perf_measurement"
    
set i 0
while { $i<$num } {
    set since [expr $i*100]
    set till [expr ($i+10)*100]
    set condObject [CondDBObjFactory::createCondDBObject \
	    $since $till "sample data value object" "sample description"]	
    $condDataAccess storeCondDBObject $folder $condObject
    puts "Object num $i - time spent $conddb_elapsed_time sec"
    CondDBObjFactory::destroyCondDBObject $condObject
    incr i
}

puts "Testing - Sucessfully created - Object"
$condDBmgr commit
CondDBMySQLMgrFactory::destroyCondDBMgr $condDBmgr







