
set(HEADERS
)

set(SOURCES
	CalculatorClient.cpp
)

SET(SCRIPTS
	runclient.sh
)

SET(LIBS
	${SCALIB}
	${SCACPPLIB}
	tuscany_sca_ioc
)

add_executable(client ${HEADERS} ${SOURCES})

target_link_libraries(client ${LIBS})

# horrible...
foreach(file ${SCRIPTS})
	ADD_CUSTOM_TARGET("${file}_dependency"
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
		DEPENDS ${SCRIPTS}
	)
	add_dependencies(client "${file}_dependency")
endforeach()
