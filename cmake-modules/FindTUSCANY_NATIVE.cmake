SET(TUSCANY_SCACPP $ENV{TUSCANY_SCACPP})
SET(TUSCANY_SDOCPP $ENV{TUSCANY_SDOCPP})


if (NOT TUSCANY_SCACPP)
	message(FATAL_ERROR "Cannot find tuscany sca: environment variable TUSCANY_SCACPP is not set")
endif()

if (NOT TUSCANY_SDOCPP)
	message(FATAL_ERROR "Cannot find tuscany sca: environment variable TUSCANY_SDOCPP is not set")
endif()


	
SET(SCA_INCLUDE_DIR "${TUSCANY_SCACPP}/include")
SET(SCA_EXT_DIR "${TUSCANY_SCACPP}/extensions")
SET(SCA_LIB_DIR "${TUSCANY_SCACPP}/lib")
SET(SDO_INCLUDE_DIR "${TUSCANY_SDOCPP}/include")
SET(SDO_LIB_DIR "${TUSCANY_SDOCPP}/lib")

SET(SCA_CPP_INCLUDE_DIR "${TUSCANY_SCACPP}/extensions/cpp/include")
SET(SCA_CPP_LIB_DIR "${TUSCANY_SCACPP}/extensions/cpp/lib")

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


if (NOT EXISTS ${SCA_EXT_DIR})
	message(FATAL_ERROR "Cannot find the tuscany extensions directory")
endif()


if (NOT EXISTS ${SCA_CPP_INCLUDE_DIR})
	message(FATAL_ERROR "Cannot find the include directory of the C++ extension")
endif()

if (NOT EXISTS ${SCA_CPP_LIB_DIR})
	message(FATAL_ERROR "Cannot find the lib directory of the C++ extension")
endif()


include_directories(${SCA_INCLUDE_DIR})
include_directories(${SCA_CPP_INCLUDE_DIR})
include_directories(${SDO_INCLUDE_DIR})

find_library(SCALIB NAMES tuscany_sca PATHS ${SCA_LIB_DIR} NO_DEFAULT_PATH)
find_library(SCACPPLIB NAMES tuscany_sca_cpp PATHS ${SCA_CPP_LIB_DIR} NO_DEFAULT_PATH)
find_library(SDOLIB NAMES tuscany_sdo PATHS ${SDO_LIB_DIR} NO_DEFAULT_PATH)

if (NOT SCALIB)
	message(FATAL_ERROR "cannot find tuscany SCA library")
endif()


if (NOT SCACPPLIB)
	message(FATAL_ERROR "cannot find tuscany SCA C++ extension library")
endif()

if (NOT SDOLIB)
	message(FATAL_ERROR "cannot find tuscany SDO library")
endif()

message(STATUS "Found SCA")
SET(TUSCANY_NATIVE_FOUND TRUE)
