#!/usr/local/bin/tcsh -f

# Check if the SCRAM environment is set up correctly
if ( "${?LOCALRT}" != "1" ) then
  echo ERROR! Variable LOCALRT is not defined
  echo ERROR! Please \'eval \`scram runtime -csh\`\' and try again
  exit 1
endif

# Script initialization
set theTestsDir = ${LOCALRT}/src/CondDBMySQL/tests
setenv COND_DB_DEBUG all
setenv COND_DB_DEBUGLOG /dev/stdout
setenv COND_DB_INIT atlobk01:conddblcg_${USER}:conddblcg:conddb

# Drop the test Conditions database before executing tests
echo "\nStart condDBMySQL_dropCondDB.csh at `date`"
set theLog =  ${LOCALRT}/tmp/${SCRAM_ARCH}/condDBMySQL_dropCondDB.out
echo "Output in \n  ${theLog}"
\rm -f ${theLog}
echo "JobStartTime: `date`" > ${theLog}
condDBMySQL_dropCondDB.csh conddblcg conddb atlobk01 conddblcg_${USER} >>& ${theLog}
echo "JobEndTime: `date`" >> ${theLog}
echo "Ended condDBMySQL_dropCondDB.csh at `date`"

# Loop through all tests and execute them
foreach theTest \
 ( basicSession storeDatax storeDatay readDatax createTags testTags \
   storeTable readTable storeTableVer readTableVer testTableTags )
  set theExe = unitTest_CondDBMySQL_${theTest}
  set theRef = ${theTestsDir}/${theTest}/${theExe}.ref
  set theLog = ${theRef}.${SCRAM_ARCH}.NEW
  echo "\nStart ${theExe} at `date`"
  echo "Output in \n  ${theLog}"
  \rm -f ${theLog}
  echo "JobStartTime: `date`" > ${theLog}
  if ( "$theTest" == "storeDatax" || "$theTest" == "storeDatay" ) then
    time ${theExe} 10 >>& ${theLog}
  else
    time ${theExe} >>& ${theLog}
  endif
  echo "JobEndTime: `date`" >> ${theLog}
  echo "Ended ${theExe} at `date`"
  ###\cp ${theLog} ${theRef}
  echo "Now: diff ${theLog} ${theRef}"
  ###tkdiff ${theLog} ${theRef} &
  echo "=== Start differences"
  diff -I insertionTime \
    -I JobStartTime -I JobEndTime ${theLog} ${theRef}
  echo "=== Ended differences"
end



