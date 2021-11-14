INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_VBGEN vbgen)

FIND_PATH(
    VBGEN_INCLUDE_DIRS
    NAMES vbgen/api.h
    HINTS $ENV{VBGEN_DIR}/include
        ${PC_VBGEN_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    VBGEN_LIBRARIES
    NAMES gnuradio-vbgen
    HINTS $ENV{VBGEN_DIR}/lib
        ${PC_VBGEN_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VBGEN DEFAULT_MSG VBGEN_LIBRARIES VBGEN_INCLUDE_DIRS)
MARK_AS_ADVANCED(VBGEN_LIBRARIES VBGEN_INCLUDE_DIRS)

