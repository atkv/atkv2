CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT               (atkv)
SET                   (CMAKE_CXX_STANDARD 11)
SET                   (CMAKE_C_STANDARD 11)
SET                   (CMAKE_C_FLAGS          "-pg")
SET                   (CMAKE_EXE_LINKER_FLAGS "-pg")
SET                   (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
SET                   (GLOBAL_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#================================================================= DEPENDENCIES
# ------------------
# CUDA
# ------------------
FIND_PACKAGE          (CUDA)
# ------------------
# JPEG
# ------------------
#if(BUILD_IMGPROC)
  FIND_PACKAGE(JPEG QUIET)
  INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIRS})
  ADD_DEFINITIONS(-DHAVE_JPEG)
#endif()

# ------------------
# PNG
# ------------------
#if(BUILD_IMGPROC)
  find_package(PNG QUIET)
  INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
  ADD_DEFINITIONS(-DHAVE_PNG)
#endif()

INCLUDE               (CTest)
INCLUDE_DIRECTORIES   (${CMAKE_SOURCE_DIR}/include)

SET                   (SRCS array.c grapharray.c list.c queue.c pqueue.c error.c img_io.c)
SET                   (HDRS macro.h optimization.h array.h grapharray.h list.h queue.h pqueue.h error.h img_io.h)
SET                   (DEPS ${PNG_LIBRARIES} ${JPEG_LIBRARIES})

FOREACH(HDR ${HDRS})
  LIST(APPEND HDRS_ABS ${CMAKE_SOURCE_DIR}/include/at/${HDR})
ENDFOREACH()
FOREACH(SRC ${SRCS})
  LIST(APPEND SRCS_ABS ${CMAKE_SOURCE_DIR}/src/${SRC})
ENDFOREACH()

ADD_LIBRARY           (atkv    SHARED ${HDRS_ABS} ${SRCS_ABS})

# ------------------------------------------------------
# TESTING
# ------------------------------------------------------
IF(BUILD_TESTING)
  FIND_PACKAGE          (CMocka QUIET)
  IF(NOT CMOCKA_FOUND)
    include(ExternalProject)
    ExternalProject_Add(CMocka
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
  SET(AT_TESTS array grapharray list queue pqueue img_io)
  FOREACH(AT_TEST ${AT_TESTS})
    ADD_EXECUTABLE        (test_${AT_TEST} ${CMAKE_SOURCE_DIR}/tests/test_${AT_TEST}.c)
    TARGET_LINK_LIBRARIES (test_${AT_TEST} atkv ${CMOCKA_LIBRARIES} ${DEPS})
    ADD_TEST              (test_${AT_TEST} test_${AT_TEST})
    if(NOT CMOCKA_FOUND)
      ADD_DEPENDENCIES      (test_${AT_TEST} CMocka)
    endif()
  ENDFOREACH()

ENDIF()

#CUDA_ADD_LIBRARY      (ift_gpu SHARED ${CMAKE_SOURCE_DIR}/src/ift.cu)
#ADD_EXECUTABLE        (iftdemo ${CMAKE_SOURCE_DIR}/demos/main.c)
#TARGET_LINK_LIBRARIES (iftdemo ift ift_gpu)
