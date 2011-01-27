#############################################################
# cmake module for finding mysqlclient
#
# returns:
#   MySQL_FOUND        : set to TRUE or FALSE
#   MySQL_INCLUDE_DIRS : paths to mysql includes
#   MySQL_LIBRARY_DIRS : paths to mysql libraries
#   MySQL_LIBRARIES    : list of mysql libraries
#
# @author Jan Engels, DESY
#############################################################

# -- fix for backwards compatibility
IF( NOT DEFINED MySQL_DIR AND DEFINED MySQL_HOME )
    SET( MySQL_DIR "${MySQL_HOME}" )
ENDIF( NOT DEFINED MySQL_DIR AND DEFINED MySQL_HOME )

IF( NOT MySQL_FIND_QUIETLY )
    MESSAGE( STATUS "Check for MySQL: ${MySQL_DIR}" )
ENDIF( NOT MySQL_FIND_QUIETLY )

# include dirs
SET( MySQL_INCLUDE_DIRS MySQL_INCLUDE_DIRS-NOTFOUND )
FIND_PATH( MySQL_INCLUDE_DIRS
    NAMES mysql.h
    PATHS ${MySQL_DIR}
    /usr
    /usr/local
    /opt/local
    /opt/products/mysql/5.0.45
    /opt/products/mysql/5.0.26
    PATH_SUFFIXES include include/mysql include/mysql5/mysql
    NO_DEFAULT_PATH
)

# libraries
SET( MySQL_LIBRARIES MySQL_LIBRARIES-NOTFOUND )
FIND_LIBRARY( MySQL_LIBRARIES
    NAMES mysqlclient mysqlclient_r
    PATHS ${MySQL_DIR}
    /usr
    /usr/local
    /opt/local
    /opt/products/mysql/5.0.45
    /opt/products/mysql/5.0.26
    PATH_SUFFIXES lib64 lib64/mysql lib64/mysql5/mysql lib lib/mysql lib/mysql5/mysql
    NO_DEFAULT_PATH
)
IF( MySQL_LIBRARIES )
    GET_FILENAME_COMPONENT( MySQL_LIBRARY_DIRS ${MySQL_LIBRARIES} PATH )
ENDIF( MySQL_LIBRARIES )
MARK_AS_ADVANCED( MySQL_INCLUDE_DIRS MySQL_LIBRARY_DIRS MySQL_LIBRARIES )

# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set MySQL_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( MySQL DEFAULT_MSG MySQL_INCLUDE_DIRS MySQL_LIBRARIES )

