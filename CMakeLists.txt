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
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
cmake_policy(SET CMP0011 NEW)

# Disable extensions here and require the chosen CMAKE_CXX_STANDARD (coming from Conan)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_CXX_STANDARD)
    message(WARNING "CMAKE_CXX_STANDARD not found, setting it to 11")
    set(CMAKE_CXX_STANDARD "11")
elseif(CMAKE_CXX_STANDARD LESS 11)
    message(FATAL_ERROR "CMAKE_CXX_STANDARD >= 11 required")
endif()

project(fep_metamodel VERSION 1.0.2)
set(FEP_METAMODEL_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(FEP_METAMODEL_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(FEP_METAMODEL_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(DOXYGEN_WARN_IF_UNDOCUMENTED "No")

set(FEP_METAMODEL_VERSION "${FEP_METAMODEL_VERSION_MAJOR}.${FEP_METAMODEL_VERSION_MINOR}.${FEP_METAMODEL_VERSION_PATCH}")

# Options #################################################
option(fep_metamodel_cmake_enable_tests
       "Enable tests - requires googletest (default: OFF)"
       OFF)


# Enable strict compiler warnings
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /wd4100 /wd4251 /wd4505 /wd4290")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -Wno-reorder")
endif()

# Use of Conan #################################
if(CONAN_COMPILER)
    message(STATUS "Including Conan build info")
    if ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
        include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo.cmake)
        include(${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo.cmake)
    elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
        include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
    elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo_multi.cmake)
        include(${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo_multi.cmake)
    else()
        message(FATAL_ERROR "Conan build info can't be found.")
    endif()

    if(CORTEX_WORKSPACE)
        conan_basic_setup(TARGETS)
    else()
        conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
    endif()
    set(gtest_search_mode CONFIG)
endif()

set(DDL_BASE_DIR ${CONAN_DDL_ROOT})

add_subdirectory(doc)
add_subdirectory(src)

if(fep_metamodel_cmake_enable_tests)
    find_package(GTest REQUIRED ${gtest_search_mode})
    enable_testing()
    set(fep_metamodel_cmake_integrated_tests ON)
    add_subdirectory(test)
endif()


install(FILES
        README.md
        LICENSE.md
        DESTINATION
        ./
)
install(DIRECTORY
        doc/license
        DESTINATION
        doc
)
install(FILES
        doc/changelog.md
        DESTINATION
        doc
)
