# Microsoft Developer Studio Project File - Name="CDBManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=CDBMANAGER - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CDBManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CDBManager.mak" CFG="CDBMANAGER - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CDBManager - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".\\" /I "$(API_ROOT)\include\winnt\pvssincl" /I "$(API_ROOT)\include\Basics\Utilities" /I "$(API_ROOT)\include\Basics\Variables" /I "$(API_ROOT)\include\Basics\NoPosix" /I "$(API_ROOT)\include\winnt\BCM\PORT" /I "$(API_ROOT)\include\BCMNew" /I "$(API_ROOT)\include\BCM\PORT" /I "$(API_ROOT)\include\BCM\BCM" /I "$(API_ROOT)\include\Configs" /I "$(API_ROOT)\include\Datapoint" /I "$(API_ROOT)\include\Messages" /I "$(API_ROOT)\include\Manager" /I "..\..\..\..\include" /I "..\..\..\src" /FI"$(API_ROOT)\include\winnt\win32.h" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "IS_MSWIN__" /D "_EXCURSION" /D "bcm_boolean_h" /D "FUNCPROTO" /YX /FD /c
# ADD BASE RSC /l 0x816 /d "NDEBUG"
# ADD RSC /l 0x816 /i "../CDBManager" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 $(API_ROOT)\lib.winnt\libManager.lib $(API_ROOT)\lib.winnt\libMessages.lib $(API_ROOT)\lib.winnt\libDatapoint.lib $(API_ROOT)\lib.winnt\libConfigs.lib $(API_ROOT)\lib.winnt\libBasics.lib $(API_ROOT)\lib.winnt\bcm.lib wsock32.lib user32.lib advapi32.lib netapi32.lib /nologo /subsystem:console /incremental:yes ..\..\..\..\conddb_static\release\condDB.lib \mysql\lib\opt\libmysql.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"msvcrt" /nodefaultlib:"msvcprt" /nodefaultlib:"libcmt" /nodefaultlib:"libcmtd" /out:"Release/PVSS00CDBManager.exe"
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "CDBManager - Win32 Release"
# Begin Source File

SOURCE=.\addVerInfo.cxx
# ADD CPP /W4
# End Source File
# Begin Source File

SOURCE=.\CDBMain.cxx
# End Source File
# Begin Source File

SOURCE=.\CDBManager.cxx
# End Source File
# Begin Source File

SOURCE=.\CDBManager.hxx
# End Source File
# Begin Source File

SOURCE=.\CDBResources.cxx
# End Source File
# Begin Source File

SOURCE=.\CDBResources.hxx
# End Source File
# End Target
# End Project
