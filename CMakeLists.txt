CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT               (atkv)
SET                   (CMAKE_CXX_STANDARD 11)
SET                   (CMAKE_C_STANDARD 11)
SET                   (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
SET                   (GLOBAL_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET                   (AT_PROJECT_NAME   @kv)
SET                   (AT_PROJECT_DESCRIPTION "Library for graph problems in C and Python")
SET                   (AT_VERSION_MAJOR   0)
SET                   (AT_VERSION_MINOR   0)
SET                   (AT_VERSION_RELEASE 4)
SET                   (AT_WEBSITE http://atkv.github.io)
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
    LIST(APPEND HDRS_ABS ${CMAKE_SOURCE_DIR}/include/at/${MODULE}.h)

    # SOURCES
    SET(SRCS_ABS)
    FOREACH(MDLFILE ${${MODULEUPPER}_SRCS})
      LIST(APPEND SRCS_ABS ${CMAKE_SOURCE_DIR}/src/${MODULE}/${MDLFILE})
    ENDFOREACH()
    LIST(APPEND AT_SRCS_ABS ${SRCS_ABS})

    ADD_LIBRARY           (atkv_${MODULE} SHARED ${HDRS_ABS} ${SRCS_ABS})
    SET(${MODULEUPPER}_LIBRARIES atkv_${MODULE} ${${MODULEUPPER}_DEPS})
    LIST(APPEND ATKV_LIBRARIES ${${MODULEUPPER}_LIBRARIES})
    LIST(APPEND PKG_CONFIG_LIBS "-latkv_${MODULE}")
  ENDIF()
ENDMACRO(AT_MAKE_MODULE)

MACRO(AT_MAKE_MODULES MODULES)
  FOREACH(MODULE ${MODULES})
    AT_MAKE_MODULE("${MODULE}")
  ENDFOREACH()
ENDMACRO(AT_MAKE_MODULES)

FUNCTION(JOIN VALUES GLUE OUTPUT)
  STRING (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  STRING (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  SET (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
ENDFUNCTION()
#====================================================================== OPTIONS
option(BUILD_CORE    "" ON)
option(BUILD_IMGPROC "" ON)
option(BUILD_GUI     "" ON)
option(BUILD_CHART   "" ON)
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
if(BUILD_GUI STREQUAL "ON" OR BUILD_CHART STREQUAL "ON")
  PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
  INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
endif()

# ------------------
# Zlib
# ------------------
if(BUILD_IMGPROC STREQUAL "ON" OR BUILD_CORE STREQUAL "ON")
  FIND_PACKAGE(ZLIB REQUIRED)
  IF(${ZLIB_FOUND})
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
    ADD_DEFINITIONS(-DHAVE_ZLIB)
  ENDIF()
endif()

# ------------------
# JPEG
# ------------------
if(BUILD_IMGPROC)
  FIND_PACKAGE(JPEG QUIET)
  INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIRS})
  ADD_DEFINITIONS(-DHAVE_JPEG)
endif()

# ------------------
# PNG
# ------------------
if(BUILD_IMGPROC)
  FIND_PACKAGE(PNG QUIET)
  INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
  ADD_DEFINITIONS(-DHAVE_PNG)
endif()
#================================================================= SOURCES
INCLUDE_DIRECTORIES   (${CMAKE_SOURCE_DIR}/include)
SET                   (MDLS core imgproc gui chart)
SET                   (CORE_HDRS    type.h i18n-lib.h version.h macro.h optimization.h znzfile.h vec.h mat4.h range.h array.h stat.h grapharray.h slist.h list.h queue.h pqueue.h error.h scc.h)
SET                   (CORE_SRCS    znzfile.c vec.c mat4.c range.c array.c stat.c grapharray.c slist.c list.c queue.c pqueue.c error.c scc.c)
SET                   (CORE_DEPS    ${ZLIB_LIBRARIES})
SET                   (IMGPROC_HDRS color.h img_io.h seed.h ift.h dicom.h nifti.h drawing.h)
SET                   (IMGPROC_SRCS color.c img_io.c seed.c ift.c dicom.c nifti.c drawing.c)
SET                   (IMGPROC_DEPS ${CORE_DEPS} ${PNG_LIBRARIES} ${JPEG_LIBRARIES} atkv_core)
SET                   (GUI_HDRS     type.h imageviewer.h imagewindow.h trackbar.h sliceviewer.h slicewindow.h display.h)
SET                   (GUI_SRCS     imageviewer.c imagewindow.c trackbar.c sliceviewer.c slicewindow.c display.c)
SET                   (GUI_DEPS     ${GTK3_LIBRARIES} ${CORE_DEPS} atkv_core)
SET                   (CHART_HDRS   type.h lineplot.h pieplot.h barplot.h scatterplot.h axis.h subchart.h chart.h render.h io.h chartviewer.h chartwindow.h)
SET                   (CHART_SRCS   type.c lineplot.c pieplot.c barplot.c scatterplot.c axis.c subchart.c chart.c render.c io.c chartviewer.c chartwindow.c)
SET                   (CHART_DEPS   ${GUI_DEPS})

CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/include/at/core/version.h.in" "${CMAKE_SOURCE_DIR}/include/at/core/version.h")

#================================================================= LIBRARY
if (COVERALLS)
  include(Coveralls)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
endif(COVERALLS)
AT_MAKE_MODULES       ("${MDLS}")
#============================================================== PKG-CONFIG
SET(PKG_CONFIG_REQUIRES    gtk+-3.0)
SET(PKG_CONFIG_LIBDIR      "\${prefix}/lib")
SET(PKG_CONFIG_INCLUDEDIR  "\${prefix}/include")
LIST(APPEND PKG_CONFIG_LIBS "-L\${libdir} -lpng -ljpeg -lz")
JOIN("${PKG_CONFIG_LIBS}" " " PKG_CONFIG_LIBS)
SET(PKG_CONFIG_CFLAGS      "-I\${includedir}")
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
)
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
  IF (COVERALLS)
    set(COVERALLS_REPO_TOKEN "7uyw7LtYInww5slk3pBM1HTQo3DC7YjrC")
    coveralls_setup("${AT_SRCS_ABS}" ${COVERALLS_UPLOAD} "${CMAKE_SOURCE_DIR}/cmake/")
  ENDIF(COVERALLS)

  INCLUDE_DIRECTORIES   (${CMOCKA_INCLUDE_DIRS})
  ENABLE_TESTING        ()

  # Defining tests----------------------------------------------
  SET(AT_CORE_TESTS    array stat grapharray slist list queue pqueue vec mat4)
  SET(AT_IMGPROC_TESTS img_io dicom nifti ift drawing color)
  SET(AT_CHART_TESTS   line)
  # ------------------------------------------------------------

  # Compiling tests
  FOREACH(MODULE ${MDLS})
    STRING(TOUPPER ${MODULE} MODULEUPPER)
    IF(BUILD_${MODULEUPPER})
      FOREACH(AT_TEST ${AT_${MODULEUPPER}_TESTS})
        ADD_EXECUTABLE        (test_${AT_TEST} ${CMAKE_SOURCE_DIR}/tests/${MODULE}/test_${AT_TEST}.c)
        TARGET_LINK_LIBRARIES (test_${AT_TEST} ${${MODULEUPPER}_LIBRARIES} ${CMOCKA_LIBRARIES} m)
        ADD_TEST              (test_${AT_TEST} test_${AT_TEST})
        if(NOT CMOCKA_FOUND)
          ADD_DEPENDENCIES      (test_${AT_TEST} CMocka)
        endif()
      ENDFOREACH()
    ENDIF()
  ENDFOREACH()
  FOREACH(AT_TEST ${AT_TESTS})

  ENDFOREACH()

ENDIF()
#=================================================================== DEMOS
IF(BUILD_GUI)
  ADD_EXECUTABLE(at_imagewindow ${CMAKE_SOURCE_DIR}/demo/imagewindow.c)
  ADD_EXECUTABLE(at_slicewindow ${CMAKE_SOURCE_DIR}/demo/slicewindow.c)
  ADD_EXECUTABLE(at_display     ${CMAKE_SOURCE_DIR}/demo/display.c)
  ADD_EXECUTABLE(at_coreorfc    ${CMAKE_SOURCE_DIR}/demo/coreorfc.c)
  ADD_EXECUTABLE(at_trackbar    ${CMAKE_SOURCE_DIR}/demo/trackbar.c)
  TARGET_LINK_LIBRARIES(at_imagewindow ${ATKV_LIBRARIES})
  TARGET_LINK_LIBRARIES(at_slicewindow ${ATKV_LIBRARIES})
  TARGET_LINK_LIBRARIES(at_display     ${ATKV_LIBRARIES})
  TARGET_LINK_LIBRARIES(at_coreorfc    ${ATKV_LIBRARIES})
  TARGET_LINK_LIBRARIES(at_trackbar    ${ATKV_LIBRARIES})
ENDIF()
IF(BUILD_CHART)
  ADD_EXECUTABLE(at_lineplot    ${CMAKE_SOURCE_DIR}/demo/lineplot.c)
  ADD_EXECUTABLE(at_dotplot     ${CMAKE_SOURCE_DIR}/demo/dotplot.c)
  ADD_EXECUTABLE(at_scatterplot ${CMAKE_SOURCE_DIR}/demo/scatterplot.c)
  ADD_EXECUTABLE(at_barplot     ${CMAKE_SOURCE_DIR}/demo/barplot.c)
  ADD_EXECUTABLE(at_pieplot     ${CMAKE_SOURCE_DIR}/demo/pieplot.c)
  TARGET_LINK_LIBRARIES(at_dotplot     ${ATKV_LIBRARIES} m)
  TARGET_LINK_LIBRARIES(at_lineplot    ${ATKV_LIBRARIES} m)
  TARGET_LINK_LIBRARIES(at_scatterplot ${ATKV_LIBRARIES} m)
  TARGET_LINK_LIBRARIES(at_barplot     ${ATKV_LIBRARIES} m)
  TARGET_LINK_LIBRARIES(at_pieplot     ${ATKV_LIBRARIES} m)
ENDIF()
#==================================================================== DOCS
if(Doxygen_FOUND)
  option(BUILD_DOCS      "Build doxygen documentation" OFF)
  if(BUILD_DOCS)
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
  endif(BUILD_DOCS)
endif(Doxygen_FOUND)

INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/data/ DESTINATION share/at)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
INSTALL(FILES    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION lib/pkgconfig)
IF(BUILD_CORE)
  INSTALL(TARGETS   atkv_core      LIBRARY DESTINATION lib)
ENDIF(BUILD_CORE)
IF(BUILD_IMGPROC)
  INSTALL(TARGETS   atkv_imgproc   LIBRARY DESTINATION lib)
ENDIF(BUILD_IMGPROC)
IF(BUILD_GUI)
  INSTALL(TARGETS   atkv_gui       LIBRARY DESTINATION lib)
ENDIF(BUILD_GUI)
IF(BUILD_CHART)
  INSTALL(TARGETS   atkv_chart     LIBRARY DESTINATION lib)
ENDIF(BUILD_CHART)

#CUDA_ADD_LIBRARY      (ift_gpu SHARED ${CMAKE_SOURCE_DIR}/src/ift.cu)
#ADD_EXECUTABLE        (iftdemo ${CMAKE_SOURCE_DIR}/demos/main.c)
#TARGET_LINK_LIBRARIES (iftdemo ift ift_gpu)
