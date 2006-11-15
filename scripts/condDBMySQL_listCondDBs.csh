#!/bin/csh -f
if ( "$3" == "" || "$4" != "" ) then
  echo Usage: $0 username password host
  echo Example: $0 conddblcg conddb atlobk01
  exit 1
endif
set theUser = `echo $1 | awk '{print tolower($0)}'`
set thePswd = `echo $2 | awk '{print tolower($0)}'`
set theHost = `echo $3 | awk '{print tolower($0)}'`
mysql -h ${theHost} -u ${theUser} --password=${thePswd} \
  -e 'show databases'

