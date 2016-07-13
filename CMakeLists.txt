CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT               (atkv)
SET                   (CMAKE_CXX_STANDARD 11)
SET                   (CMAKE_C_STANDARD 11)
#SET                   (CMAKE_C_FLAGS          "-pg")
#SET                   (CMAKE_EXE_LINKER_FLAGS "-pg")
SET                   (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
SET                   (GLOBAL_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET                   (AT_VERSION_MAJOR   0)
SET                   (AT_VERSION_MINOR   0)
SET                   (AT_VERSION_RELEASE 1)
SET                   (AT_VERSION_STRING "${AT_VERSION_MAJOR}.${AT_VERSION_MINOR}.${AT_VERSION_RELEASE}")
SET                   (AT_INSTALL_PATH ${CMAKE_INSTALL_PREFIX})
SET                   (AT_RESOURCE_PATH "${CMAKE_INSTALL_PREFIX}/share/at")
#======================================================================= MACROS
MACRO(AT_MAKE_MODULE MODULE)
  STRING(TOUPPER ${MODULE} MODULEUPPER)
  IF(BUILD_${MODULEUPPER})
    # HEADERS
    SET(HDRS_ABS)
    FOREACH(MDLFILE ${${MODULEUPPER}_HDRS})
      LIST(APPEND HDRS_ABS ${CMAKE_SOURCE_DIR}/include/at/${MODULE}/${MDLFILE})
    ENDFOREACH()

    # SOURCES
    SET(SRCS_ABS)
    FOREACH(MDLFILE ${${MODULEUPPER}_SRCS})
      LIST(APPEND SRCS_ABS ${CMAKE_SOURCE_DIR}/src/${MODULE}/${MDLFILE})
    ENDFOREACH()

    ADD_LIBRARY           (atkv_${MODULE} SHARED ${HDRS_ABS} ${SRCS_ABS})
    LIST(APPEND ATKV_LIBRARIES atkv_${MODULE} ${${MODULEUPPER}_DEPS})
  ENDIF()
ENDMACRO(AT_MAKE_MODULE)

MACRO(AT_MAKE_MODULES MODULES)
  FOREACH(MODULE ${MODULES})
    AT_MAKE_MODULE("${MODULE}")
  ENDFOREACH()
ENDMACRO(AT_MAKE_MODULES)
#====================================================================== OPTIONS
option(BUILD_CORE    "" ON)
option(BUILD_IMGPROC "" ON)
option(BUILD_GUI     "" ON)
option(BUILD_DOCS    "Build doxygen documentation" OFF)
option(COVERALLS     "Generate coveralls data" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)
#================================================================= DEPENDENCIES
INCLUDE               (CTest)
FIND_PACKAGE          (PkgConfig)
# ------------------
# CUDA
# ------------------
FIND_PACKAGE          (CUDA)

# ------------------
# Doxygen
# ------------------
#if(BUILD_IMGPROC)
  FIND_PACKAGE(Doxygen QUIET)
#endif()

# ------------------
# GTK 3
# ------------------
if(BUILD_GUI)
  PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
  INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
endif()

# ------------------
# Zlib
# ------------------
if(BUILD_IMGPROC)
  FIND_PACKAGE(ZLIB REQUIRED)
  IF(${ZLIB_FOUND})
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
    ADD_DEFINITIONS(-DHAVE_ZLIB)
  ENDIF()
endif()

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
#================================================================= SOURCES
INCLUDE_DIRECTORIES   (${CMAKE_SOURCE_DIR}/include)
SET                   (MDLS core imgproc gui)
SET                   (CORE_HDRS    type.h i18n-lib.h version.h macro.h optimization.h znzfile.h vec.h array.h grapharray.h list.h queue.h pqueue.h error.h scc.h)
SET                   (CORE_SRCS    znzfile.c vec.c array.c grapharray.c list.c queue.c pqueue.c error.c scc.c)
SET                   (IMGPROC_HDRS color.h img_io.h ift.h dicom.h nifti.h drawing.h)
SET                   (IMGPROC_SRCS color.c img_io.c ift.c dicom.c nifti.c drawing.c)
SET                   (GUI_HDRS type.h imageviewer.h imagewindow.h trackbar.h display.h)
SET                   (GUI_SRCS imageviewer.c imagewindow.c trackbar.c display.c)
SET                   (CORE_DEPS ${ZLIB_LIBRARIES})
SET                   (IMGPROC_DEPS ${CORE_DEPS} ${PNG_LIBRARIES} ${JPEG_LIBRARIES} atkv_core)
SET                   (GUI_DEPS ${GTK3_LIBRARIES} ${CORE_DEPS} atkv_core)

CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/include/at/core/version.h.in" "${CMAKE_SOURCE_DIR}/include/at/core/version.h")

#================================================================= LIBRARY
AT_MAKE_MODULES       ("${MDLS}")

#================================================================= TESTING
IF(BUILD_TESTING)
  FIND_PACKAGE          (CMocka QUIET)
  FILE(COPY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_BINARY_DIR})
  IF(NOT CMOCKA_FOUND)
    include(ExternalProject)
    ExternalProject_Add(CMocka
      GIT_REPOSITORY "git://git.cryptomilk.org/projects/cmocka.git"
      GIT_TAG        "master"
      UPDATE_COMMAND ""
      PATCH_COMMAND  ""
      SOURCE_DIR     "${CMAKE_SOURCE_DIR}/3rdparty/cmocka"
      CMAKE_ARGS -DBuildShared=ON -DBuildExamples=OFF -DCMAKE_INSTALL_PREFIX=${GLOBAL_OUTPUT_PATH}/cmocka
      TEST_COMMAND   ""
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

  INCLUDE_DIRECTORIES   (${CMOCKA_INCLUDE_DIRS})
  ENABLE_TESTING        ()

  # Defining tests----------------------------------------------
  SET(AT_CORE_TESTS    array grapharray list queue pqueue vec)
  SET(AT_IMGPROC_TESTS img_io dicom nifti ift drawing)
  # ------------------------------------------------------------

  # Compiling tests
  FOREACH(MODULE ${MDLS})
    STRING(TOUPPER ${MODULE} MODULEUPPER)
    MESSAGE(STATUS ${MODULEUPPER})
    FOREACH(AT_TEST ${AT_${MODULEUPPER}_TESTS})
      ADD_EXECUTABLE        (test_${AT_TEST} ${CMAKE_SOURCE_DIR}/tests/${MODULE}/test_${AT_TEST}.c)
      TARGET_LINK_LIBRARIES (test_${AT_TEST} atkv_${MODULE} ${${MODULEUPPER}_DEPS} ${CMOCKA_LIBRARIES})
      ADD_TEST              (test_${AT_TEST} test_${AT_TEST})
      if(NOT CMOCKA_FOUND)
        ADD_DEPENDENCIES      (test_${AT_TEST} CMocka)
      endif()
    ENDFOREACH()
  ENDFOREACH()
  FOREACH(AT_TEST ${AT_TESTS})

  ENDFOREACH()

ENDIF()
#=================================================================== DEMOS
IF(BUILD_GUI)
  ADD_EXECUTABLE(at_imagewindow ${CMAKE_SOURCE_DIR}/demo/imagewindow.c)
  ADD_EXECUTABLE(at_display ${CMAKE_SOURCE_DIR}/demo/display.c)
  ADD_EXECUTABLE(at_coreorfc ${CMAKE_SOURCE_DIR}/demo/coreorfc.c)
  TARGET_LINK_LIBRARIES(at_imagewindow atkv_gui ${GUI_DEPS} atkv_imgproc ${IMGPROC_DEPS})
  TARGET_LINK_LIBRARIES(at_display     atkv_gui ${GUI_DEPS} atkv_imgproc ${IMGPROC_DEPS})
  TARGET_LINK_LIBRARIES(at_coreorfc    atkv_gui ${GUI_DEPS} atkv_imgproc ${IMGPROC_DEPS})
ENDIF()
#==================================================================== DOCS
if(Doxygen_FOUND)
  option(BUILD_DOCS      "Build doxygen documentation" ON)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/)
  configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/doc/Doxyfile @ONLY)
    add_custom_target(doc ALL
      ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/doc/Doxyfile
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
      COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_custom_command(TARGET doc PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/doc ${CMAKE_BINARY_DIR}/doc)
endif(Doxygen_FOUND)

INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/data/ DESTINATION share/at)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
INSTALL(TARGETS atkv_core      LIBRARY DESTINATION lib)
INSTALL(TARGETS atkv_imgproc   LIBRARY DESTINATION lib)

#CUDA_ADD_LIBRARY      (ift_gpu SHARED ${CMAKE_SOURCE_DIR}/src/ift.cu)
#ADD_EXECUTABLE        (iftdemo ${CMAKE_SOURCE_DIR}/demos/main.c)
#TARGET_LINK_LIBRARIES (iftdemo ift ift_gpu)
