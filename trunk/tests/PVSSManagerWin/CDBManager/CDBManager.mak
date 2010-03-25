# Microsoft Developer Studio Generated NMAKE File, Based on CDBManager.dsp
!IF "$(CFG)" == ""
CFG=CDBMANAGER - WIN32 RELEASE
!MESSAGE No configuration specified. Defaulting to CDBMANAGER - WIN32 RELEASE.
!ENDIF 

!IF "$(CFG)" != "CDBManager - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CDBManager.mak" CFG="CDBMANAGER - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CDBManager - Win32 Release" (based on "Win32 (x86) Console Application")
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

ALL : "$(OUTDIR)\CDBManager.exe"


CLEAN :
	-@erase "$(INTDIR)\addVerInfo.obj"
	-@erase "$(INTDIR)\CDBMain.obj"
	-@erase "$(INTDIR)\CDBManager.obj"
	-@erase "$(INTDIR)\CDBResources.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\CDBManager.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CDBManager.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=$(API_ROOT)\lib.winnt\libManager.lib $(API_ROOT)\lib.winnt\libMessages.lib $(API_ROOT)\lib.winnt\libDatapoint.lib $(API_ROOT)\lib.winnt\libConfigs.lib $(API_ROOT)\lib.winnt\libBasics.lib $(API_ROOT)\lib.winnt\bcm.lib $(API_ROOT)\lib.winnt\port.lib wsock32.lib user32.lib advapi32.lib netapi32.lib ..\..\..\..\conddb_static\release\condDB.lib libmysql.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\CDBManager.pdb" /machine:I386 /nodefaultlib:"msvcrt" /nodefaultlib:"msvcprt" /nodefaultlib:"libcmt" /nodefaultlib:"libcmtd" /out:"$(OUTDIR)\CDBManager.exe" 
LINK32_OBJS= \
	"$(INTDIR)\addVerInfo.obj" \
	"$(INTDIR)\CDBMain.obj" \
	"$(INTDIR)\CDBManager.obj" \
	"$(INTDIR)\CDBResources.obj"

"$(OUTDIR)\CDBManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I ".\\" /I "$(API_ROOT)\include\winnt\pvssincl" /I "$(API_ROOT)\include\Basics\Utilities" /I "$(API_ROOT)\include\Basics\Variables" /I "$(API_ROOT)\include\Basics\NoPosix" /I "$(API_ROOT)\include\winnt\BCM\PORT" /I "$(API_ROOT)\include\BCM\PORT" /I "$(API_ROOT)\include\BCM\BCM" /I "$(API_ROOT)\include\Configs" /I "$(API_ROOT)\include\Datapoint" /I "$(API_ROOT)\include\Messages" /I "$(API_ROOT)\include\Manager" /I "..\..\..\..\include" /FI"$(API_ROOT)\include\winnt\win32.h" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "IS_MSWIN__" /D "_EXCURSION" /D "bcm_boolean_h" /D "FUNCPROTO" /Fp"$(INTDIR)\CDBManager.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
!IF EXISTS("CDBManager.dep")
!INCLUDE "CDBManager.dep"
!ELSE 
!MESSAGE Warning: cannot find "CDBManager.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CDBManager - Win32 Release"
SOURCE=.\addVerInfo.cxx
CPP_SWITCHES=/nologo /ML /W4 /GX /O2 /I ".\\" /I "$(API_ROOT)\include\winnt\pvssincl" /I "$(API_ROOT)\include\Basics\Utilities" /I "$(API_ROOT)\include\Basics\Variables" /I "$(API_ROOT)\include\Basics\NoPosix" /I "$(API_ROOT)\include\winnt\BCM\PORT" /I "$(API_ROOT)\include\BCM\PORT" /I "$(API_ROOT)\include\BCM\BCM" /I "$(API_ROOT)\include\Configs" /I "$(API_ROOT)\include\Datapoint" /I "$(API_ROOT)\include\Messages" /I "$(API_ROOT)\include\Manager" /I "..\..\..\..\include" /FI"$(API_ROOT)\include\winnt\win32.h" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "IS_MSWIN__" /D "_EXCURSION" /D "bcm_boolean_h" /D "FUNCPROTO" /Fp"$(INTDIR)\CDBManager.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\addVerInfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


SOURCE=.\CDBMain.cxx

"$(INTDIR)\CDBMain.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CDBManager.cxx

"$(INTDIR)\CDBManager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\CDBResources.cxx

"$(INTDIR)\CDBResources.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

