SET(HEADERS
	Calculator.h
	Divide.h
	CalculatorImpl.h
	DivideImpl.h
)

SET(METADATA
	Calculator.h.cpp
	Divide.h.cpp
	CalculatorImpl.h.cpp
	DivideImpl.h.cpp
)

SET(SOURCES
	CalculatorImpl.cpp
	DivideImpl.cpp
)

SET(COMPONENT_FILES
	CalculatorComponent.componentType
	DivideComponent.componentType
	sample.calculator.composite
)


set_source_files_properties(${METADATA} PROPERTIES GENERATED TRUE)


add_library(Calculator SHARED ${HEADERS} ${SOURCES})

add_library(Calculator-md SHARED ${METADATA})
target_link_libraries(Calculator-md ${SelfPortraitLib} Calculator)

# horrible...
foreach(file ${COMPONENT_FILES})
	ADD_CUSTOM_TARGET("${file}_dependency"
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
		DEPENDS ${COMPONENT_FILES}
	)
	add_dependencies(Calculator "${file}_dependency")
endforeach()

foreach(iface ${HEADERS})
	ADD_CUSTOM_TARGET("${iface}_dependency"
		COMMAND ${SELFPC} "${CMAKE_CURRENT_SOURCE_DIR}/${iface}" "--interface-diagnostics" "-o" "${iface}.cpp"
		DEPENDS ${HEADERS}
	)
	add_dependencies(Calculator-md "${iface}_dependency")
endforeach()
