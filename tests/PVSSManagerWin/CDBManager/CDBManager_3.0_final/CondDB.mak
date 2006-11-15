# Microsoft Developer Studio Generated NMAKE File, Based on CondDB.dsp
!IF "$(CFG)" == ""
CFG=CondDB - Win32 Release
!MESSAGE No configuration specified. Defaulting to CondDB - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "CondDB - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CondDB.mak" CFG="CondDB - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CondDB - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\PVSS00CDBManager.exe" ".\addVerInfo.obj"


CLEAN :
	-@erase "$(INTDIR)\addVerInfo.obj"
	-@erase "$(INTDIR)\CDBMain.obj"
	-@erase "$(INTDIR)\CDBManager.obj"
	-@erase "$(INTDIR)\CDBResources.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PVSS00CDBManager.exe"
	-@erase "addVerInfo.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /Zi /I "..\." /I ".\\" /I "$(API_ROOT)\include\winnt\pvssincl" /I "$(API_ROOT)\include\Basics\Utilities" /I "$(API_ROOT)\include\Basics\Variables" /I "$(API_ROOT)\include\Basics\NoPosix" /I "$(API_ROOT)\include\BCMNew" /I "$(API_ROOT)\include\Configs" /I "$(API_ROOT)\include\Datapoint" /I "$(API_ROOT)\include\Messages" /I "..\..\..\..\src" /I "..\..\..\..\ConditionsDB" /I "$(API_ROOT)\include\Manager" /FI"$(API_ROOT)\include\winnt\win32.h" /D "WIN32" /D "IS_MSWIN__" /D "_EXCURSION" /D "bcm_boolean_h" /D "FUNCPROTO" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CondDB.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=$(API_ROOT)\lib.winnt\libManager.lib $(API_ROOT)\lib.winnt\libMessages.lib $(API_ROOT)\lib.winnt\libDatapoint.lib $(API_ROOT)\lib.winnt\libBasics.lib $(API_ROOT)\lib.winnt\bcm.lib wsock32.lib user32.lib advapi32.lib netapi32.lib $(API_ROOT)\lib.winnt\libConfigs.lib  ..\..\..\..\config\VS6.0_project\Release\condDB.lib \mysql\lib\opt\libmysql.lib /nologo /subsystem:console /pdb:none /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib /out:"$(OUTDIR)\PVSS00CDBManager.exe" 
LINK32_OBJS= \
	"$(INTDIR)\CDBResources.obj" \
	"$(INTDIR)\CDBMain.obj" \
	"$(INTDIR)\CDBManager.obj" \
	"$(INTDIR)\addVerInfo.obj"

"$(OUTDIR)\PVSS00CDBManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

InputPath=.\Release\PVSS00CDBManager.exe
SOURCE="$(InputPath)"

".\addVerInfo.obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	Release\addVerInfo.obj
<< 
	

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("CondDB.dep")
!INCLUDE "CondDB.dep"
!ELSE 
!MESSAGE Warning: cannot find "CondDB.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CondDB - Win32 Release"
SOURCE=..\addVerInfo.cxx

"$(INTDIR)\addVerInfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CDBMain.cxx

"$(INTDIR)\CDBMain.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CDBManager.cxx

"$(INTDIR)\CDBManager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CDBResources.cxx

"$(INTDIR)\CDBResources.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

