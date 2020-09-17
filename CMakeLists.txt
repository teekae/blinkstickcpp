cmake_minimum_required (VERSION 2.6)

project (blinkstickcpp LANGUAGES CXX)

option(BUILD_CLI "Build command line BlinkStick control program" ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(HIDAPI REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/blinkstickcpp.pc.in
		${CMAKE_BINARY_DIR}/blinkstickcpp.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/blinkstickcpp.pc DESTINATION "${INSTALL_PKGCONFIG_DIR}")

set(blinkstickcpp_VERSION_MAJOR 0)
set(blinkstickcpp_VERSION_MINOR 2)
set(blinkstickcpp_VERSION_PATCH 1)

set(blinkstickcpp_VERSION "${blinkstickcpp_VERSION_MAJOR}.${blinkstickcpp_VERSION_MINOR}.${blinkstickcpp_VERSION_PATCH}")
add_subdirectory(blinkstickcpp)
