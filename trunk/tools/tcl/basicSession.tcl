#!/bin/sh
#
# TCL Wrapper for ConditionsDB
# Lisbon ATLAS-DAQ Collaboration
# Ported from C++ to TCL by Jorge Lima 2003/04/15
#
#\
exec tclsh "$0" "$@"

package require conddb 1.0
 
set CondDBmgr [CondDBMySQLMgrFactory::createCondDBMgr]

puts "condDBmgr->init"

$CondDBmgr init
# CondDBmgr->init("localhost:test_conddb02:jmal:online");
    
# Create a ConditionsDB

puts "Ok"
$CondDBmgr startUpdate
$CondDBmgr createCondDB
$CondDBmgr commit

CondDBMySQLMgrFactory::destroyCondDBMgr $CondDBmgr

