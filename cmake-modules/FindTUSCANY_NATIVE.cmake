SET(TUSCANY_SCACPP $ENV{TUSCANY_SCACPP})
SET(TUSCANY_SDOCPP $ENV{TUSCANY_SDOCPP})


if (NOT TUSCANY_SCACPP)
	message(FATAL_ERROR "Cannot find tuscany sca: environment variable TUSCANY_SCACPP is not set")
endif()

if (NOT TUSCANY_SDOCPP)
	message(FATAL_ERROR "Cannot find tuscany sca: environment variable TUSCANY_SDOCPP is not set")
endif()


	
SET(SCA_INCLUDE_DIR "${TUSCANY_SCACPP}/include")
SET(SCA_LIB_DIR "${TUSCANY_SCACPP}/lib")
SET(SDO_INCLUDE_DIR "${TUSCANY_SDOCPP}/include")
SET(SDO_LIB_DIR "${TUSCANY_SDOCPP}/lib")


if (NOT EXISTS ${SCA_INCLUDE_DIR})
	message(FATAL_ERROR "${TUSCANY_SCACPP} does not contain an include directory")
endif()


if (NOT EXISTS ${SDO_INCLUDE_DIR})
	message(FATAL_ERROR "${TUSCANY_SDOCPP} does not contain an include directory")
endif()

if (NOT EXISTS ${SCA_LIB_DIR})
	message(FATAL_ERROR "${TUSCANY_SCACPP} does not contain a lib directory")
endif()

if (NOT EXISTS ${SDO_LIB_DIR})
	message(FATAL_ERROR "${TUSCANY_SDOCPP} does not contain a lib directory")
endif()


include_directories(${SCA_INCLUDE_DIR})
include_directories(${SDO_INCLUDE_DIR})
find_library(SCALIB NAMES tuscany_sca PATHS ${SCA_LIB_DIR} NO_DEFAULT_PATH)
find_library(SDOLIB NAMES tuscany_sdo PATHS ${SDO_LIB_DIR} NO_DEFAULT_PATH)

if (NOT SCALIB)
	message(FATAL_ERROR "cannot find tuscany SCA library")
endif()

if (NOT SDOLIB)
	message(FATAL_ERROR "cannot find tuscany SDO library")
endif()

message(STATUS "Found SCA")
SET(TUSCANY_NATIVE_FOUND TRUE)
