
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was CrowConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

get_filename_component(CROW_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

list(APPEND CMAKE_MODULE_PATH ${CROW_CMAKE_DIR})
find_dependency(asio)
list(REMOVE_AT CMAKE_MODULE_PATH -1)

set(CROW_INSTALLED_FEATURES "")

if(NOT DEFINED CROW_FEATURES)
  set(CROW_FEATURES ${CROW_INSTALLED_FEATURES})
endif()

if(CROW_ENABLE_COMPRESSION)
  find_dependency(ZLIB)
endif()

if(CROW_ENABLE_SSL)
  find_dependency(OpenSSL)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/CrowTargets.cmake")
check_required_components("Crow")

get_target_property(_CROW_ILL Crow::Crow INTERFACE_LINK_LIBRARIES)
get_target_property(_CROW_ICD Crow::Crow INTERFACE_COMPILE_DEFINITIONS)

if(_CROW_ILL STREQUAL "_CROW_ILL-NOTFOUND")
        set(_CROW_ILL "")
endif()
if(_CROW_ICD STREQUAL "_CROW_ICD-NOTFOUND")
        set(_CROW_ICD "")
endif()

list(REMOVE_ITEM _CROW_ILL "ZLIB::ZLIB" "OpenSSL::SSL")
list(REMOVE_ITEM _CROW_ICD "CROW_ENABLE_SSL" "CROW_ENABLE_COMPRESSION")

if(CROW_ENABLE_COMPRESSION)
  list(APPEND _CROW_ILL "ZLIB::ZLIB")
  list(APPEND _CROW_ICD "CROW_ENABLE_COMPRESSION")
endif()

if(CROW_ENABLE_SSL)
  list(APPEND _CROW_ILL "OpenSSL::SSL")
  list(APPEND _CROW_ICD "CROW_ENABLE_SSL")
endif()

if( NOT (_CROW_ICD STREQUAL "" ) )
  set_target_properties(Crow::Crow PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS "${_CROW_ICD}"
  )
endif()
if( NOT (_CROW_ILL STREQUAL "" ) )
  set_target_properties(Crow::Crow PROPERTIES
    INTERFACE_LINK_LIBRARIES "${_CROW_ILL}"
  )
endif()
