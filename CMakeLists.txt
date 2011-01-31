#################################################################
# CMakeLists to build CondDBMySQL
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
#################################################################



# project name
PROJECT( CondDBMySQL )


# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 9 )
SET( ${PROJECT_NAME}_VERSION_PATCH 3 )



### DEPENDENCIES ############################################################

FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES REQUIRED )

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )


# ----------- MySQL --------------------------------------------------------
# looks for mysql in MySQL_DIR
FIND_PACKAGE( MySQL REQUIRED )

INCLUDE_DIRECTORIES( "${MySQL_INCLUDE_DIRS}" )
LINK_LIBRARIES( "${MySQL_LIBRARIES}" )

MESSAGE (STATUS "Check for MySQL_INCLUDE_DIRS: ${MySQL_INCLUDE_DIRS}")
MESSAGE (STATUS "Check for MySQL_LIBRARIES: ${MySQL_LIBRARIES}")
# --------------------------------------------------------------------------




###########################################################################
#                                                                         #
# file used to create config.h                                            #
#                                                                         #
# -At the end of this file,  the line CONFIGURE_FILE() creates a config.h #
# from a config.h.cmake (similar as configure creates a config.h from a   #
# config.h.in).                                                           #
# -E.g. The CONFIGURE_FILE() command will read this file (i.e.            #
#  config.h.cmake) and replace "#cmakedefine HAVE_UNISTD_H" with          #
#  "#define HAVE_UNISTD_H 1" if unistd.h was found, or                    #
#  "#undef HAVE_UNISTD_H" if not (and respectively for string.h).         #
###########################################################################

#Defines the default directory to write the code files for user tables
SET(CONDDBROOT "${CMAKE_CURRENT_SOURCE_DIR}")

#Defines to 1 if the system is big endian
INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(CONDDB_BIGENDIAN)

#Define to 1 if you have the <flfcn.h> header file
INCLUDE(CheckIncludeFiles)
CHECK_INCLUDE_FILES( dlfcn.h     HAVE_DLFCN_H )
#Define to 1 if you have the <inttypes.h> header file
CHECK_INCLUDE_FILES( inttypes.h  HAVE_INTTYPES_H )

#Define to 1 if you have the 'mysqlclient' library (-lmysqlclient)
IF(MySQL_LIBRARIES)
 SET(HAVE_LIBMYSQLCLIENT TRUE)
ENDIF()

#Define to 1 if you have the 'z' library (-lz)
INCLUDE (CheckLibraryExists)
CHECK_LIBRARY_EXISTS(z deflate "" HAVE_LIBZ)

#Define to 1 if you have the <memory.h> header file
CHECK_INCLUDE_FILES( memory.h  HAVE_MEMORY_H )
#Define to 1 if you have the <pwd.h> header file
CHECK_INCLUDE_FILES( pwd.h HAVE_PWD_H )
#Define to 1 if you have the <stdint.h> header file
CHECK_INCLUDE_FILES( stdint.h HAVE_STDINT_H )
#Define to 1 if you have the <stdlib.h> header file
CHECK_INCLUDE_FILES( stdlib.h  HAVE_STDLIB_H )
#Define to 1 if you have the <strings.h> header file
CHECK_INCLUDE_FILES( strings.h HAVE_STRINGS_H )
#Define to 1 if you have the <string.h> header file
CHECK_INCLUDE_FILES( string.h HAVE_STRING_H )
#Define to 1 if you have the <sys/stat.h> header file
CHECK_INCLUDE_FILES( sys/stat.h HAVE_SYS_STAT_H )
#Define to 1 if you have the <sys/time.h> header file
CHECK_INCLUDE_FILES( sys/time.h HAVE_SYS_TIME_H )
#Define to 1 if you have the <sys/types.h> header file
CHECK_INCLUDE_FILES( sys/types.h HAVE_SYS_TYPES_H )
#Define to 1 if you have the <time.h> header file
CHECK_INCLUDE_FILES( time.h HAVE_TIME_H )
#Define to 1 if you have the <unistd.h> header file
CHECK_INCLUDE_FILES( unistd.h HAVE_UNISTD_H )


#Suffix for lib directories
SET(MYLIBSUFF {""})
#Name of package
SET(PACKAGE "ConditionsDB")
#Define the address where bug reports for this package should be sent
SET(PACKAGE_BUGREPORT {""})
#Define the full name of the package
SET(PACKAGE_NAME {""})
#Define the full name and version of this package
SET(PACKAGE_STRING {""})
#Define the one symbol short name of this package
SET(PACKAGE_TARNAME {""})
#Define the version of this package
SET(PACKAGE_VERSION {""})
#Define the version number of this package
SET(VERSION "0.4")

#####################################################################################
#CONFIGURE_FILE needs the full path
#CMAKE_CURRENT_DIR is the directory where this file, CMakeLists.txt, is found
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)
MESSAGE( STATUS "CMake created from CondDBMySQL/config.h.cmake the file CondDBMySQL/src/config.h" )
MESSAGE( STATUS "-----------------------------------------------------------" )




#####################################################################
#                                                                   #
# installation directories                                          #
#                                                                   #
#####################################################################



#default destination for header files: ${CMAKE_INSTALL_PREFIX}/include
SET(INCLUDE_INSTALL_DIR "include" CACHE PATH "Directory to install the header file")
MARK_AS_ADVANCED(INCLUDE_INSTALL_DIR)

# default destination for library files: ${CMAKE_INSTALL_PREFIX}/lib
SET( LIB_INSTALL_DIR "lib" CACHE PATH "Directory to install the library" )
MARK_AS_ADVANCED( LIB_INSTALL_DIR )

# default destination for .cmake files: ${CMAKE_INSTALL_PREFIX}/cmake
SET( CONFIG_INSTALL_DIR "cmake" CACHE PATH "Directory to install the XXXConfig.cmake files" )
MARK_AS_ADVANCED( CONFIG_INSTALL_DIR )

# default destination for library files: ${CMAKE_INSTALL_PREFIX}/bin
SET( EXECUTABLE_INSTALL_DIR "bin" CACHE PATH "Directory to install the executables" )
MARK_AS_ADVANCED( EXECUTABLE_INSTALL_DIR )




#####################################################################
#                                                                   #
# library                                                           #
#                                                                   #
#####################################################################



INCLUDE_DIRECTORIES( ./include )


ADD_DEFINITIONS( "-Wall -ansi" )
ADD_DEFINITIONS( -DMYSQL_INIT_STR="localhost:mydb:calvin:hobbes" )

AUX_SOURCE_DIRECTORY( ./src library_sources )
ADD_SHARED_LIBRARY( conddb ${library_sources} )


# ------------ install header files + conddb library ---------------------
INSTALL_SHARED_LIBRARY( conddb DESTINATION ${LIB_INSTALL_DIR} )
INSTALL_DIRECTORY( ./include/ DESTINATION ${INCLUDE_INSTALL_DIR} FILES_MATCHING PATTERN "*.h" )
INSTALL_DIRECTORY( ./src/ DESTINATION ${INCLUDE_INSTALL_DIR} FILES_MATCHING PATTERN "*.h" )
# ------------------------------------------------------------------------




#####################################################################
#                                                                   #
# tests                                                             #
#                                                                   #
#####################################################################


INCLUDE_DIRECTORIES( ./scramIncludes ./tests/storeDataOnChange )


ADD_CUSTOM_TARGET( tests )
MACRO( ADD_CONDDBMYSQL_TEST _testname )
    IF( BUILD_TESTING )
        ADD_EXECUTABLE( ${_testname} ${ARGN} )
    ELSE()
        ADD_EXECUTABLE( ${_testname} EXCLUDE_FROM_ALL ${ARGN} )
    ENDIF()
    ADD_DEPENDENCIES( tests ${_testname} )
    TARGET_LINK_LIBRARIES( ${_testname} conddb )
    #INSTALL( TARGETS ${_testname} DESTINATION ${EXECUTABLE_INSTALL_DIR}/conddbmysql_tests )
ENDMACRO( ADD_CONDDBMYSQL_TEST )



ADD_CONDDBMYSQL_TEST( basicSession ./tests/basicSession/basicSession.cxx )
ADD_CONDDBMYSQL_TEST( createTags ./tests/createTags/createTags.cxx )
ADD_CONDDBMYSQL_TEST( performanceTests ./tests/performanceTests/performanceTests.cxx )
ADD_CONDDBMYSQL_TEST( readDatax ./tests/readDatax/readDatax.cxx )
ADD_CONDDBMYSQL_TEST( readTable ./tests/readTable/readTable.cxx )
ADD_CONDDBMYSQL_TEST( readTableVer ./tests/readTableVer/readTableVer.cxx )
ADD_CONDDBMYSQL_TEST( storeDatax ./tests/storeDatax/storeDatax.cxx )
ADD_CONDDBMYSQL_TEST( storeDatay ./tests/storeDatay/storeDatay.cxx )
ADD_CONDDBMYSQL_TEST( storeTable ./tests/storeTable/storeTable.cxx )
ADD_CONDDBMYSQL_TEST( storeTableVer ./tests/storeTableVer/storeTableVer.cxx )
ADD_CONDDBMYSQL_TEST( testFolders ./tests/testFolders/testFolders.cxx )
ADD_CONDDBMYSQL_TEST( testTableTags ./tests/testTableTags/testTableTags.cxx )
ADD_CONDDBMYSQL_TEST( testTags ./tests/testTags/testTags.cxx )
ADD_CONDDBMYSQL_TEST( teste ./tools/migration/teste.cxx )
ADD_CONDDBMYSQL_TEST( storeDataOnChange ./tests/storeDataOnChange/storeDataOnChange.cxx ./tests/storeDataOnChange/cdbStore.cxx )




#####################################################################
#                                                                   #
# print status                                                      #
#                                                                   #
#####################################################################

# display some variables and write them to cache
DISPLAY_STD_VARIABLES()


# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( CondDBMySQLConfig.cmake CondDBMySQLConfigVersion.cmake CondDBMySQLLibDeps.cmake )

