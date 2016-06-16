CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT               (atkv)
SET                   (CMAKE_CXX_STANDARD 11)
SET                   (CMAKE_C_STANDARD 11)
SET                   (CMAKE_C_FLAGS          "-pg")
SET                   (CMAKE_EXE_LINKER_FLAGS "-pg")
SET                   (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
SET                   (GLOBAL_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

FIND_PACKAGE          (CUDA)

# Global settings
#set(GLOBAL_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)

# Sets global output directory for single configuration (GCC)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})

# Sets global output directory for sub-configurations (msvc, mingw)
#foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
#  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
#  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
#  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
#  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${GLOBAL_OUTPUT_PATH})
#endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)




INCLUDE               (CTest)
INCLUDE_DIRECTORIES   (${CMAKE_SOURCE_DIR}/include)

SET                   (SRCS array.c)
SET                   (HDRS array.h)
#SET                   (HDRS ift.h core.h ift_gpu.h)

FOREACH(HDR ${HDRS})
  LIST(APPEND HDRS_ABS ${CMAKE_SOURCE_DIR}/include/at/${HDR})
ENDFOREACH()
FOREACH(SRC ${SRCS})
  LIST(APPEND SRCS_ABS ${CMAKE_SOURCE_DIR}/src/${SRC})
ENDFOREACH()

ADD_LIBRARY           (atkv    SHARED ${HDRS_ABS} ${SRCS_ABS})


MACRO(AT_DOWNLOAD_CMOCKA)
  set(CMockaCMakeArgs
     -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
     -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external
     -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
     -DUNIT_TESTING=OFF
     )

  ExternalProject_Add(cmockaDownload
      PREFIX ${CMAKE_SOURCE_DIR}/external
      SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/cmocka
      BINARY_DIR ${CMAKE_BINARY_DIR}/external/cmocka-build
      STAMP_DIR ${CMAKE_BINARY_DIR}/external/cmocka-stamp
      TMP_DIR ${CMAKE_BINARY_DIR}/external/cmocka-tmp
      INSTALL_DIR ${CMAKE_BINARY_DIR}/external
      CMAKE_ARGS ${CMockaCMakeArgs}
      # GIT_REPOSITORY "git://git.cryptomilk.org/projects/cmocka.git"
      #GIT_TAG "origin/master"
      #GIT_SUBMODULES ""
      UPDATE_COMMAND cd ${CMAKE_SOURCE_DIR} && git submodule update --init --recursive external/cmocka
      DOWNLOAD_COMMAND cd ${CMAKE_SOURCE_DIR} && git submodule update --init --recursive external/cmocka
  )
  SET(CMOCKA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/include)
  SET(CMOCKA_LIB_DIR ${CMAKE_SOURCE_DIR}/external/lib)
  SET(CMOCKA_LIBRARIES ${CMOCKA_LIB_DIR}/libcmocka.so)
  ADD_LIBRARY(cmocka SHARED IMPORTED)
  SET_TARGET_PROPERTIES(cmocka PROPERTIES IMPORTED_LOCATION ${CMOCKA_LIBRARIES})
ENDMACRO()

# ------------------------------------------------------
# TESTING
# ------------------------------------------------------
IF(BUILD_TESTING)
  FIND_PACKAGE          (CMocka QUIET)
  IF(NOT CMOCKA_FOUND)
    include(ExternalProject)
    ExternalProject_Add(
      CMocka

      GIT_REPOSITORY "git://git.cryptomilk.org/projects/cmocka.git"
      GIT_TAG "master"

      UPDATE_COMMAND ""
      PATCH_COMMAND ""

      SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/cmocka"
      CMAKE_ARGS -DBuildShared=ON -DBuildExamples=OFF -DCMAKE_INSTALL_PREFIX=${GLOBAL_OUTPUT_PATH}/cmocka

      TEST_COMMAND ""
    )
    ExternalProject_Add_Step(
      CMocka CopyToBin
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/cmocka/bin ${GLOBAL_OUTPUT_PATH}
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/cmocka/lib ${GLOBAL_OUTPUT_PATH}
      DEPENDEES install
    )

    set(CMOCKA_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/3rdparty/cmocka/include")
    set(CMOCKA_LIBRARIES "${CMAKE_SHARED_LIBRARY_PREFIX}cmocka${CMAKE_SHARED_LIBRARY_SUFFIX}")
    include_directories(${CMOCKA_INCLUDE_DIRS})
    link_directories(${GLOBAL_OUTPUT_PATH})
  ENDIF()

  MESSAGE(STATUS ${CMOCKA_INCLUDE_DIRS})
  MESSAGE(STATUS ${CMOCKA_LIBRARIES})
  INCLUDE_DIRECTORIES   (${CMOCKA_INCLUDE_DIRS})
  ENABLE_TESTING        ()
  ADD_EXECUTABLE        (test_array ${CMAKE_SOURCE_DIR}/tests/test_array.c)
  TARGET_LINK_LIBRARIES (test_array atkv ${CMOCKA_LIBRARIES})
  ADD_TEST              (test_array test_array)
  if(NOT CMOCKA_FOUND)
    ADD_DEPENDENCIES      (test_array CMocka)
  endif()
ENDIF()

#CUDA_ADD_LIBRARY      (ift_gpu SHARED ${CMAKE_SOURCE_DIR}/src/ift.cu)
#ADD_EXECUTABLE        (iftdemo ${CMAKE_SOURCE_DIR}/demos/main.c)
#TARGET_LINK_LIBRARIES (iftdemo ift ift_gpu)
