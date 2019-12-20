# LIBRARY
add_library(blinkstickcpp
	src/blinkstick.cpp
    src/device.cpp
)

include(GenerateExportHeader)
generate_export_header(blinkstickcpp EXPORT_FILE_NAME blinkstick/export.hpp)

target_include_directories(blinkstickcpp
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${HIDAPI_INCLUDE_DIR})

target_link_libraries(blinkstickcpp
    PRIVATE
        ${HIDAPI_LIBRARY}
)

set_target_properties(blinkstickcpp PROPERTIES OUTPUT_NAME blinkstick)
set_property(TARGET blinkstickcpp PROPERTY CXX_STANDARD 17)

install(TARGETS blinkstickcpp EXPORT blinkstickcppTargets
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}")

install(FILES 
            blinkstick.hpp
            device.hpp
            ${PROJECT_BINARY_DIR}/blinkstick/export.hpp
        DESTINATION 
            "${INSTALL_INC_DIR}")

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_BINARY_DIR}/blinkstickcpp/blinkstickcppConfigVersion.cmake"
        VERSION ${blinkstickcpp_VERSION}
        COMPATIBILITY AnyNewerVersion
)

export(EXPORT
            blinkstickcppTargets
       FILE
            "${CMAKE_BINARY_DIR}/blinkstickcpp/blinkstickcppTargets.cmake")

configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/blinkstickcppConfig.cmake.in # input
        ${CMAKE_BINARY_DIR}/blinkstickcpp/blinkstickcppConfig.cmake # output
        INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

install(EXPORT
            blinkstickcppTargets
        FILE
            blinkstickcppTargets.cmake
        DESTINATION
            cmake)

install(FILES
            "${CMAKE_BINARY_DIR}/blinkstickcpp/blinkstickcppConfig.cmake"
            "${CMAKE_BINARY_DIR}/blinkstickcpp/blinkstickcppConfigVersion.cmake"
        DESTINATION
                cmake
        COMPONENT
                Devel)

add_subdirectory(test)