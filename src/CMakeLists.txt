SET( lib1name ${PROJECT_NAME} )

##############################################################
# include directories                                        #
##############################################################
INCLUDE_DIRECTORIES( "${${PROJECT_NAME}_SOURCE_DIR}/include" 
                     "${${PROJECT_NAME}_SOURCE_DIR}/scramIncludes")
INCLUDE_DIRECTORIES( "${${PROJECT_NAME}_SOURCE_DIR}/tests/storeDataOnChange" )

##############################################################
# dependencies                                               #
##############################################################
INCLUDE_DIRECTORIES( "${MySQL_INCLUDE_DIRS}" )
LINK_LIBRARIES( "${MySQL_LIBRARIES}" )

MESSAGE (STATUS "Check for MySQL_INCLUDE_DIRS: ${MySQL_INCLUDE_DIRS}")
MESSAGE (STATUS "Check for MySQL_LIBRARIES: ${MySQL_LIBRARIES}")

##############################################################
# sources                                                    #
##############################################################
AUX_SOURCE_DIRECTORY(. ${lib1name}_src)

# require proper c++
ADD_DEFINITIONS( "-Wall -ansi" )
IF( "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.6 )
    ADD_DEFINITIONS( "-DMYSQL_INIT_STR=\\\"localhost:mydb:calvin:hobbes\\\"" )
ELSE()
    ADD_DEFINITIONS( -DMYSQL_INIT_STR="localhost:mydb:calvin:hobbes" )
ENDIF()

ADD_LIBRARY( ${lib1name} ${${lib1name}_src} )
SET_TARGET_PROPERTIES( ${lib1name} PROPERTIES
                       VERSION ${${PROJECT_NAME}_VERSION}
                       SOVERSION ${${PROJECT_NAME}_SOVERSION}
                       # allow creatind static and shared libs without conflicts
                       #CLEAN_DIRECT_OUTPUT 1
		       #avoid conflicts between library and binary target names
		       OUTPUT_NAME conddb
                      )
##############################################################
# binaries                                                   #
##############################################################
# create the bin directory (if it is not already there)
ADD_CUSTOM_TARGET( bin )


#####################################################

SET( CONDDBMYSQL_EXEC_SRCS ../tests/basicSession/basicSession.cxx
                           ../tests/createTags/createTags.cxx
			   ../tests/performanceTests/performanceTests.cxx
			   ../tests/readDatax/readDatax.cxx
			   ../tests/readTable/readTable.cxx
			   ../tests/readTableVer/readTableVer.cxx
			   ../tests/storeDatax/storeDatax.cxx
			   ../tests/storeDatay/storeDatay.cxx
			   ../tests/storeTable/storeTable.cxx
			   ../tests/storeTableVer/storeTableVer.cxx
			   ../tests/testFolders/testFolders.cxx
			   ../tests/testTableTags/testTableTags.cxx
			   ../tests/testTags/testTags.cxx
			   ../tools/migration/teste.cxx
			   )

FOREACH( src ${CONDDBMYSQL_EXEC_SRCS} )
  #extract executable name from the source, i.e. ../tests/basicSession/basicSession.cxx -> basicSession
  STRING( REGEX REPLACE ".*/(.*)\\.cxx" "\\1" tempExe ${src} )
  
  ADD_EXECUTABLE( ${tempExe} ${src})
  ADD_DEPENDENCIES( bin ${tempExe} )
  TARGET_LINK_LIBRARIES( ${tempExe} ${lib1name})
  INSTALL( TARGETS ${tempExe} DESTINATION bin )
ENDFOREACH()

#--------------------
ADD_EXECUTABLE( storeDataOnChange ../tests/storeDataOnChange/storeDataOnChange.cxx 
                                  ../tests/storeDataOnChange/cdbStore.cxx)
ADD_DEPENDENCIES( bin storeDataOnChange )
TARGET_LINK_LIBRARIES(storeDataOnChange ${lib1name})
INSTALL( TARGETS storeDataOnChange DESTINATION bin )


###########################################################################
#                                                                         #
# install                                                                 #
#                                                                         #
###########################################################################
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/include/" DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*CVS*" EXCLUDE
         PATTERN "*.svn" EXCLUDE
)

INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/src/" DESTINATION ${INCLUDE_INSTALL_DIR}
         PATTERN "*~" EXCLUDE
         PATTERN "*.cxx" EXCLUDE
         PATTERN "*CVS*" EXCLUDE
         PATTERN "*.svn" EXCLUDE
         PATTERN "*.txt" EXCLUDE
)


INSTALL( TARGETS ${lib1name}
         DESTINATION ${LIB_INSTALL_DIR}
         PERMISSIONS
         OWNER_READ OWNER_WRITE OWNER_EXECUTE
         GROUP_READ GROUP_EXECUTE
         WORLD_READ WORLD_EXECUTE  )

