CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
SET                   (CMAKE_CXX_STANDARD 11)
SET                   (CMAKE_C_STANDARD 11)
SET                   (CMAKE_C_FLAGS          "-pg")
SET                   (CMAKE_EXE_LINKER_FLAGS "-pg")
SET                   (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
PROJECT               (ift)
FIND_PACKAGE          (CUDA)
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

# ------------------------------------------------------
# TESTING
# ------------------------------------------------------
IF(BUILD_TESTING)
  FIND_PACKAGE          (CMocka)
  MESSAGE(STATUS ${CMOCKA_INCLUDE_DIR})
  MESSAGE(STATUS ${CMOCKA_LIBRARIES})
  INCLUDE_DIRECTORIES   (${CMOCKA_INCLUDE_DIR})
  ENABLE_TESTING        ()
  ADD_EXECUTABLE        (test_array ${CMAKE_SOURCE_DIR}/tests/test_array.c)
  TARGET_LINK_LIBRARIES (test_array atkv ${CMOCKA_LIBRARIES})
  ADD_TEST              (test_array test_array)
ENDIF()

#CUDA_ADD_LIBRARY      (ift_gpu SHARED ${CMAKE_SOURCE_DIR}/src/ift.cu)
#ADD_EXECUTABLE        (iftdemo ${CMAKE_SOURCE_DIR}/demos/main.c)
#TARGET_LINK_LIBRARIES (iftdemo ift ift_gpu)
