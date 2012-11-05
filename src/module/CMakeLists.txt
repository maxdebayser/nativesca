set(HEADERS
	IoCImplementationExtension.h
	ComponentContext.h
	CompositeContext.h
	IoCInterfaceExtension.h
	IoCServiceWrapper.h
	IoCServiceProxy.h
	VariantToParameter.h
	model/IoCImplementation.h
	model/IoCInterface.h
	model/IoCServiceBinding.h
	model/IoCReferenceBinding.h
)

set(SOURCES
	CPPIoCExtension.cpp
	ComponentContext.cpp
	CompositeContext.cpp
	IoCImplementationExtension.cpp
	IoCInterfaceExtension.cpp
	IoCServiceWrapper.cpp
	IoCServiceProxy.cpp
	VariantToParameter.cpp
	model/IoCImplementation.cpp
	model/IoCInterface.cpp
	model/IoCServiceBinding.cpp
	model/IoCReferenceBinding.cpp
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fPIC")


set(LIBS
	${SCALIB}
	${SDOLIB}
	${SelfPortraitLib}
)

add_library(tuscany_sca_ioc SHARED ${HEADERS} ${SOURCES})
target_link_libraries(tuscany_sca_ioc ${LIBS})


install(TARGETS tuscany_sca_ioc LIBRARY DESTINATION "${IOC_INSTALL_DIR}/lib")
install(CODE "EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory \"${IOC_INSTALL_DIR}/module\")")
if(CMAKE_HOST_UNIX)
	install(CODE "EXECUTE_PROCESS(COMMAND ln -sf \"${IOC_INSTALL_DIR}/lib/libtuscany_sca_ioc.so\" \"${IOC_INSTALL_DIR}/module/libtuscany_sca_ioc.so\")")
endif(CMAKE_HOST_UNIX)
install(DIRECTORY ./ DESTINATION "${IOC_INSTALL_DIR}/include" FILES_MATCHING PATTERN "*.h")
