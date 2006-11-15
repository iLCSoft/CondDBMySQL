# Microsoft Developer Studio Project File - Name="condDB_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=condDB_static - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "condDB_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "condDB_static.mak" CFG="condDB_static - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "condDB_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
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
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "\mysql\include" /I "..\..\ConditionsDB" /D "__LCC__" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x816 /d "NDEBUG"
# ADD RSC /l 0x816 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\condDB.lib"
# Begin Target

# Name "condDB_static - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\Blob.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBDataAccess.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBDataIterator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBInterface.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBMySQLMgrFactory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBObject.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBTable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBTag.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBTagMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBTimePath.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondFolder.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondFolderMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondFolderSet.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\CondObjFactory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\Debug.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlConnection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlDataMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlDBMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlFolderMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlObjectMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlOnlineMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlTagMgr.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\SimpleTime.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\SimpleTimeDuration.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\SimpleTimeInterval.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\Blob.h
# End Source File
# Begin Source File

SOURCE="..\..\src\boot-sql.h"
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBDataAccess.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBDataIterator.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\CondDBException.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\CondDBKey.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBLog.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\CondDBMySQLMgrFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBObject.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\CondDBObjFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\CondDBObjyDBMgrFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\CondDBOracleDBMgrFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBTable.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBTag.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBTagMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondDBTimePath.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\CondDBVector.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondFolder.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondFolderMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\CondFolderSet.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Debug.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBDataAccess.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBDataIterator.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBFolder.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBFolderMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBLogMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBObject.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBStreamable.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBTable.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBTag.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBTagMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\ICondDBTimePath.h
# End Source File
# Begin Source File

SOURCE=..\..\src\IntegerConversion.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlConnection.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlDBMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlFolderMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlObjectMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlOnlineMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlTagMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\MySqlTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\ConditionsDB\SimpleTime.h
# End Source File
# Begin Source File

SOURCE=..\..\src\SimpleTimeDuration.h
# End Source File
# Begin Source File

SOURCE=..\..\src\SimpleTimeInterval.h
# End Source File
# End Group
# End Target
# End Project
