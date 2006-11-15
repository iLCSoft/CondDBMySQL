# Microsoft Developer Studio Generated NMAKE File, Based on condDB_static.dsp
!IF "$(CFG)" == ""
CFG=condDB_static - Win32 Release
!MESSAGE No configuration specified. Defaulting to condDB_static - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "condDB_static - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "condDB_static.mak" CFG="condDB_static - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "condDB_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe
OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\condDB.lib"


CLEAN :
	-@erase "$(INTDIR)\CondDBDataAccess.obj"
	-@erase "$(INTDIR)\CondDBDataIterator.obj"
	-@erase "$(INTDIR)\CondDBInterface.obj"
	-@erase "$(INTDIR)\CondDBMySQLMgrFactory.obj"
	-@erase "$(INTDIR)\CondDBObject.obj"
	-@erase "$(INTDIR)\CondDBTag.obj"
	-@erase "$(INTDIR)\CondDBTagMgr.obj"
	-@erase "$(INTDIR)\CondFolder.obj"
	-@erase "$(INTDIR)\CondFolderMgr.obj"
	-@erase "$(INTDIR)\CondFolderSet.obj"
	-@erase "$(INTDIR)\CondObjFactory.obj"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\MySqlConnection.obj"
	-@erase "$(INTDIR)\MySqlDataMgr.obj"
	-@erase "$(INTDIR)\MySqlDBMgr.obj"
	-@erase "$(INTDIR)\MySqlFolderMgr.obj"
	-@erase "$(INTDIR)\MySqlObjectMgr.obj"
	-@erase "$(INTDIR)\MySqlTagMgr.obj"
	-@erase "$(INTDIR)\SimpleTime.obj"
	-@erase "$(INTDIR)\SimpleTimeDuration.obj"
	-@erase "$(INTDIR)\SimpleTimeInterval.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\condDB.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\condDB_static.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\condDB.lib" 
LIB32_OBJS= \
	"$(INTDIR)\CondDBDataAccess.obj" \
	"$(INTDIR)\CondDBDataIterator.obj" \
	"$(INTDIR)\CondDBInterface.obj" \
	"$(INTDIR)\CondDBMySQLMgrFactory.obj" \
	"$(INTDIR)\CondDBObject.obj" \
	"$(INTDIR)\CondDBTag.obj" \
	"$(INTDIR)\CondDBTagMgr.obj" \
	"$(INTDIR)\CondFolder.obj" \
	"$(INTDIR)\CondFolderMgr.obj" \
	"$(INTDIR)\CondFolderSet.obj" \
	"$(INTDIR)\CondObjFactory.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\MySqlConnection.obj" \
	"$(INTDIR)\MySqlDataMgr.obj" \
	"$(INTDIR)\MySqlDBMgr.obj" \
	"$(INTDIR)\MySqlFolderMgr.obj" \
	"$(INTDIR)\MySqlObjectMgr.obj" \
	"$(INTDIR)\MySqlTagMgr.obj" \
	"$(INTDIR)\SimpleTime.obj" \
	"$(INTDIR)\SimpleTimeDuration.obj" \
	"$(INTDIR)\SimpleTimeInterval.obj"

"$(OUTDIR)\condDB.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\condDB_static.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("condDB_static.dep")
!INCLUDE "condDB_static.dep"
!ELSE 
!MESSAGE Warning: cannot find "condDB_static.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "condDB_static - Win32 Release"
SOURCE=..\implementationMySQL\src\CondDBDataAccess.cxx

"$(INTDIR)\CondDBDataAccess.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondDBDataIterator.cxx

"$(INTDIR)\CondDBDataIterator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondDBInterface.cxx

"$(INTDIR)\CondDBInterface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondDBMySQLMgrFactory.cxx

"$(INTDIR)\CondDBMySQLMgrFactory.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondDBObject.cxx

"$(INTDIR)\CondDBObject.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondDBTag.cxx

"$(INTDIR)\CondDBTag.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondDBTagMgr.cxx

"$(INTDIR)\CondDBTagMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondFolder.cxx

"$(INTDIR)\CondFolder.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondFolderMgr.cxx

"$(INTDIR)\CondFolderMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondFolderSet.cxx

"$(INTDIR)\CondFolderSet.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\CondObjFactory.cxx

"$(INTDIR)\CondObjFactory.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\Debug.cxx

"$(INTDIR)\Debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\MySqlConnection.cxx

"$(INTDIR)\MySqlConnection.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\MySqlDataMgr.cxx

"$(INTDIR)\MySqlDataMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\MySqlDBMgr.cxx

"$(INTDIR)\MySqlDBMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\MySqlFolderMgr.cxx

"$(INTDIR)\MySqlFolderMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\MySqlObjectMgr.cxx

"$(INTDIR)\MySqlObjectMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\MySqlTagMgr.cxx

"$(INTDIR)\MySqlTagMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\SimpleTime.cxx

"$(INTDIR)\SimpleTime.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\SimpleTimeDuration.cxx

"$(INTDIR)\SimpleTimeDuration.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\implementationMySQL\src\SimpleTimeInterval.cxx

"$(INTDIR)\SimpleTimeInterval.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

