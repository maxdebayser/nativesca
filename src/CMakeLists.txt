include_directories(${nativesca_SOURCE_DIR}/src/module)
include_directories(${nativesca_SOURCE_DIR}/src/utils)

add_subdirectory(module)
add_subdirectory(unit_test)
add_subdirectory(calculator)

install(DIRECTORY ./xsd DESTINATION "${IOC_INSTALL_DIR}")
