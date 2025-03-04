##############################################################################
# Copyright Deere & Company. For more information,
# please see COPYRIGHT file in root of source repository.
##############################################################################

# Set project's name
set(PROJECT VanillaApp)
project(${PROJECT})

if(RUN_TEST_WITH_CODECOVERAGE)
   include("../BuildSystem/CodeCoverageFlagSet.cmake")
endif()

set(IPM2COMMON_CHANGESET "" FORCE)
execute_process(COMMAND bash "-c" "git submodule status --recursive | grep IPM2Common | awk '{print $1}' | sed 's/\\+//g' | head -n 1"
    OUTPUT_VARIABLE IPM2COMMON_CHANGESET
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
set(APPSPECIFICIPC_CHANGESET "" FORCE)
execute_process(COMMAND bash "-c" "git submodule status --recursive | grep AppSpecificIPC | awk '{print $1}' | sed 's/\\+//g' "
    OUTPUT_VARIABLE APPSPECIFICIPC_CHANGESET
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
configure_file(${CMAKE_SOURCE_DIR}/AppVersion.h.in ${CMAKE_SOURCE_DIR}/AppVersion.h)

#-----------------------------------------------------------------------------
# build the Vanilla
#-----------------------------------------------------------------------------
set (VanillaApp_HEADERS
    VanillaApp.h)
set (VanillaApp_SOURCES
    VanillaApp.cpp)

add_library(l${PROJECT} SHARED
    ${VanillaApp_HEADERS}
    ${VanillaApp_SOURCES}
    )

add_executable(${PROJECT}
    VanillaAppMain.cpp
    )

target_include_directories(l${PROJECT}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/soc-framework/CoreLibs
        ${CMAKE_SOURCE_DIR}/modules/serialization
        ${CMAKE_SOURCE_DIR}/modules/structs
    )

set_target_properties(${PROJECT}
    PROPERTIES INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/")

target_link_libraries(l${PROJECT}
# When an application loaded as a library is linked against
# tracepoint provider's dynamic library (ImageProcessingLatency in our case; which is linked to LTTng),
# make sure the parent app started as a process is also linked against
# the same tracepoint provider's dynamic library to prevent thread locking issues.
    Utils
    ImageProcessingLatency
    ApplicationBase
    lIPCPacketParser
    ParameterIO
    ServiceIO
    ServiceFactory
    serialization
    lA53Diagnostics
    ${Boost_LIBRARIES}
    )

target_link_libraries(${PROJECT}
    l${PROJECT})

install (TARGETS ${PROJECT} RUNTIME DESTINATION bin)
install (TARGETS l${PROJECT} LIBRARY DESTINATION lib)

if(BUILD STREQUAL "ARM")
    install (FILES "${CMAKE_SOURCE_DIR}/soc-framework/configurations/targetMonoConfig_SoC1.xml"
                   "${CMAKE_SOURCE_DIR}/soc-framework/configurations/targetMonoConfig_SoC2.xml"
                   "${CMAKE_SOURCE_DIR}/soc-framework/configurations/targetStereoConfig_SoC1.xml"
                   "${CMAKE_SOURCE_DIR}/soc-framework/configurations/targetStereoConfig_SoC2.xml"
                   "${CMAKE_SOURCE_DIR}/soc-framework/configurations/targetStereoConfig_Nitro.xml"
        DESTINATION bin
        )
    install(DIRECTORY "${XML_DIRECTORY}/"
        DESTINATION bin/LoggingConfigurations
        )
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/soc-framework/configurations/ServiceConfigurations/"
        DESTINATION bin/ServiceConfigurations
        )
else()
    install(FILES "${CMAKE_SOURCE_DIR}/soc-framework/configurations/ubuntuConfig.xml"
        DESTINATION bin
        )
endif()

