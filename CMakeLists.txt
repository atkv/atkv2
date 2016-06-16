CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT               (ift)
FIND_PACKAGE          (CUDA)
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
FIND_PACKAGE(CMocka)
ENABLE_TESTING        ()
ADD_EXECUTABLE        (test_array ${CMAKE_SOURCE_DIR}/tests/test_array.c)
TARGET_LINK_LIBRARIES (test_array atkv ${CMOCKA_LIBRARIES})
ADD_TEST              (test_array test_array)

#CUDA_ADD_LIBRARY      (ift_gpu SHARED ${CMAKE_SOURCE_DIR}/src/ift.cu)
#ADD_EXECUTABLE        (iftdemo ${CMAKE_SOURCE_DIR}/demos/main.c)
#TARGET_LINK_LIBRARIES (iftdemo ift ift_gpu)
