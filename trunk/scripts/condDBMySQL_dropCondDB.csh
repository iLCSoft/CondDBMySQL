#!/bin/csh -f
if ( "$4" == "" || "$5" != "" ) then
  echo Usage: $0 user pswd host conditionsdb
  echo Example: $0 conddblcg conddb atlobk01 conddblcg_${USER}
  exit 1
endif
set theUser = `echo $1 | awk '{print tolower($0)}'`
set thePswd = `echo $2 | awk '{print tolower($0)}'`
set theHost = `echo $3 | awk '{print tolower($0)}'`
set theCondDB = `echo $4 | awk '{print tolower($0)}'`
mysql -h ${theHost} -u ${theUser} --password=${thePswd} \
  -e "drop database ${theCondDB}"
