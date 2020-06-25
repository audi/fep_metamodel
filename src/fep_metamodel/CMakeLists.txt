 #
 # Copyright @ 2019 Audi AG. All rights reserved.
 # 
 #     This Source Code Form is subject to the terms of the Mozilla
 #     Public License, v. 2.0. If a copy of the MPL was not distributed
 #     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 # 
 # If it is not possible or desirable to put the notice in a particular file, then
 # You may include the notice in a location (such as a LICENSE file in a
 # relevant directory) where a recipient would be likely to look for such a notice.
 # 
 # You may add additional accurate notices of copyright ownership.
 #
#this how to use it while compiling this project
set(BUILD_INTERFACE_INCLUDES ${PROJECT_SOURCE_DIR}/include)
#this is how to use includes on the target
set(INSTALL_INTERFACE_INCLUDES include)

find_package(a_util REQUIRED)

add_library(fep_metamodel 
            fep_element.cpp 
            ${BUILD_INTERFACE_INCLUDES}/fep_metamodel/fep_element.h 
            fep_system.cpp 
            ${BUILD_INTERFACE_INCLUDES}/fep_metamodel/fep_system.h)

target_include_directories(fep_metamodel PUBLIC
    "$<BUILD_INTERFACE:${BUILD_INTERFACE_INCLUDES}>"
    "$<INSTALL_INTERFACE:${INSTALL_INTERFACE_INCLUDES}>"
)
target_link_libraries(fep_metamodel PUBLIC a_util)

set_target_properties(fep_metamodel PROPERTIES
    OUTPUT_NAME fep_metamodel
    DEBUG_POSTFIX "d${FEP_METAMODEL_VERSION_MAJOR}.${FEP_METAMODEL_VERSION_MINOR}"
    RELEASE_POSTFIX "${FEP_METAMODEL_VERSION_MAJOR}.${FEP_METAMODEL_VERSION_MINOR}"
    RELWITHDEBINFO_POSTFIX "${FEP_METAMODEL_VERSION_MAJOR}.${FEP_METAMODEL_VERSION_MINOR}"
)

configure_file(fep_metamodel-config.cmake.in ${CMAKE_BINARY_DIR}/fep_metamodel-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/fep_metamodel-config.cmake DESTINATION .)
# Create version config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/fep_metamodel-config-version.cmake
                                 COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fep_metamodel-config-version.cmake DESTINATION .)

install(
    TARGETS fep_metamodel
    LIBRARY NAMELINK_SKIP DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib
)

# install content from include directory
install(
    DIRECTORY
        ${BUILD_INTERFACE_INCLUDES}/fep_metamodel
    DESTINATION
        include
    FILES_MATCHING PATTERN
        "*.h"
)
