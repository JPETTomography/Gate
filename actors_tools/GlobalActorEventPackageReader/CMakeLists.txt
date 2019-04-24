cmake_minimum_required(VERSION 2.6)

#MANDATORY: Before first cmake please insert here your path to ROOT installation directory
set(ROOT_INSTALLATION_DIRECTORY "/home/mateusz/Libraries/Installed/root")
#set(GLOBAL_ACTOR_DICTIONARIES "/home/mateusz/Libraries/Build/Gate/libGlobalActorEventPackageDict.so")

################################################################################
# Handling of thisroot.sh
################################################################################

message(STATUS "Your ROOT installation directory is: ${ROOT_INSTALLATION_DIRECTORY}")

if(EXISTS ${ROOT_INSTALLATION_DIRECTORY}/bin/thisroot.sh)
  message(STATUS "Status of thisroot.sh file : EXIST")
else()
 message(FATAL_ERROR "Status of thisroot.sh file :  DOES NOT EXIST - is your path to ROOT installation directory is correct? Please check a variable ROOT_INSTALLATION_DIRECTORY in this CMakeList.txt file.")
endif()

message(STATUS "Calling thisroot.sh")
execute_process(COMMAND source ${ROOT_INSTALLATION_DIRECTORY}/bin/thisroot.sh)

################################################################################
# Project configuration
################################################################################

set(projectName ga_epr)
set(projectBinary ${projectName}.x)

project(${projectName} CXX) # using only C++

add_definitions(-std=c++11) # using C++11 standard

################################################################################
# Handling of ROOT 
################################################################################

# first, try the old-school ROOTSYS variable
set(root_prefix $ENV{ROOTSYS})
if(root_prefix)
  list(APPEND CMAKE_MODULE_PATH ${root_prefix}/etc/cmake/)  
endif()

# try also a search based on root-config
execute_process(COMMAND root-config --etcdir OUTPUT_VARIABLE ROOT_ETCDIR OUTPUT_STRIP_TRAILING_WHITESPACE)
list(APPEND CMAKE_MODULE_PATH ${ROOT_ETCDIR}/cmake)

find_package(ROOT QUIET)

if(ROOT_FOUND)
  message(STATUS "Found ROOT version: ${ROOT_VERSION} (${ROOT_CONFIG_EXECUTABLE})")
else()
  # if we failed with CMake based find_package, use fallback
  # root-config version from cmake/fallback/
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fallback)
  message(WARNING "No CMake modules from a ROOT installation found. Trying fallback mode.")
  find_package(ROOT 5 REQUIRED)
endif()

if(ROOT_USE_FILE)
  include(${ROOT_USE_FILE})
endif()

################################################################################
# Coping dictioanries code
################################################################################

execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/copy_dictionaries.sh ${CMAKE_CURRENT_SOURCE_DIR})

################################################################################
# Sources and linking 
################################################################################

# Adding ROOT dictionaries
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)  #to change the LD_LIBRARY_PATH and accept pcm
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(dict Dictionaries)
ROOT_GENERATE_DICTIONARY(GlobalActorEventPackageDict ${dict}/GateGlobalActorDictionaryEvent.hh ${dict}/GateGlobalActorDictionaryTrack.hh ${dict}/GateGlobalActorDictionaryHit.hh 
 ${dict}/GateGlobalActorDictionaryEnums.hh LINKDEF ${dict}/GateGlobalActorDictionariesLinkDef.hh)
#---Create a shared library with geneated dictionary
add_library(GlobalActorEventPackageDict SHARED ${dict}/GateGlobalActorDictionaryEvent.cc ${dict}/GateGlobalActorDictionaryTrack.cc ${dict}/GateGlobalActorDictionaryHit.cc ${dict}/GateGlobalActorDictionaryEnums.cc GlobalActorEventPackageDict.cxx)
target_link_libraries(GlobalActorEventPackageDict ${ROOT_LIBRARIES})

file(GLOB SOURCES 
 Run.cpp
 Reader/Reader.cpp
 AnalysisClass/AnalysisClass.cpp
 AnalysisClassesManager/AnalysisClassesManager.cpp
 ${dict}/GateGlobalActorDictionaryEnums.cc
 ${dict}/GateGlobalActorDictionaryEvent.cc
 ${dict}/GateGlobalActorDictionaryHit.cc
 ${dict}/GateGlobalActorDictionaryTrack.cc
 UserAnalysisClasses/*.cpp)

file(GLOB HEADERS
 Dictionaries.h 
 Reader/Reader.h
 AnalysisClass/AnalysisClass.h
 AnalysisClassesManager/AnalysisClassesManager.h
 ${dict}/GateGlobalActorDictionariesLinkDef.hh
 ${dict}/GateGlobalActorDictionaryEnums.hh
 ${dict}/GateGlobalActorDictionaryEvent.hh
 ${dict}/GateGlobalActorDictionaryHit.hh
 ${dict}/GateGlobalActorDictionaryTrack.hh
 UserAnalysisClasses/*.h)
 
add_executable(ga_epr ${SOURCES} ${HEADERS} )
target_link_libraries(ga_epr ${ROOT_LIBRARIES} GlobalActorEventPackageDict)
