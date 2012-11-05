include_directories(${nativesca_SOURCE_DIR}/src/calculator/sample.calculator)
include_directories(${nativesca_SOURCE_DIR}/src/module)

add_subdirectory(client)
add_subdirectory(sample.calculator)

SET(COMPOSITE_FILE
	sample.calculator.app.composite
)

add_custom_target(
	composite ALL
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${COMPOSITE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}"
	DEPENDS ${COMPOSITE_FILE}
)
