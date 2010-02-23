PROJECT( CondDBMySQL )

#################################################################
#                                                               #
# cmake options                                                 #
#                                                               #
#################################################################
CMAKE_MINIMUM_REQUIRED( VERSION 2.6.2 )
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)

SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

# set default cmake build type to RelWithDebInfo
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# set default install prefix to project root directory
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" )
ENDIF(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")

# project options
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
##SET( MYSQL_INIT_STR "localhost:mydb:calvin:hobbes")


#################################################################
#                                                               #
# CMAKE module path                                             #
#                                                               #
#################################################################
SET( CMAKE_MODULE_PATH  "${CMAKE_MODULE_PATH}" "${${PROJECT_NAME}_SOURCE_DIR}/cmake" )

##################################################################
# find packages (dependencies)                                   #
##################################################################
#use the ILC cmake macro MacroCheckDeps.cmake
#this has the advantage that it look for <PACKAGE>_HOME
#whereas the FIND_PACKAGE(<PACKAGE>) looks for the
#<PACKAGE_DIR> variable
SET( ${PROJECT_NAME}_DEPENDS "MySQL" )



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
IF(MYSQL_LIBRARY)
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



###################################################################################
#                                                                                 #
# installation options                                                            #
#                                                                                 #
###################################################################################
# add install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# add install path to the rpath list (apple)
IF( APPLE )
    SET( CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib" )
    MARK_AS_ADVANCED( CMAKE_INSTALL_NAME_DIR )
ENDIF()
# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )


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


# provide nicer directory layout in build directory
SET( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" CACHE PATH
    "LIBRARY_OUTPUT_PATH" FORCE )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH )

#################
# project version
#################

SET( ${PROJECT_NAME}_MAJOR_VERSION 0  )
SET( ${PROJECT_NAME}_MINOR_VERSION 5 )
SET( ${PROJECT_NAME}_PATCH_LEVEL   10  )

# library Unix style versioning
SET( ${PROJECT_NAME}_SOVERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}" )
SET( ${PROJECT_NAME}_VERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL}" )



######################################################################
#                                                                    #
# subdirectories                                                     #
#                                                                    #
######################################################################
#add only the src directory (the other directories: tests and tools
# are included in the src/CMakeLists.txt)
ADD_SUBDIRECTORY( src )


#####################################################################
#                                                                   #
# install configuration                                             #
#                                                                   #
#####################################################################
# create configuration file from .in file
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY )

# install configuration file
INSTALL( FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION . )


#####################################################################
#                                                                   #
# print status                                                      #
#                                                                   #
#####################################################################
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )
MESSAGE( STATUS "Change a value with: cmake -D<Variable>=<Value>" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS )
