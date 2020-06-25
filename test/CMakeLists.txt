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
if (NOT fep_metamodel_cmake_integrated_tests)
    project(fep_metamodel_tests)
    # Disable extensions here and require the chosen CMAKE_CXX_STANDARD (coming from Conan)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    if(NOT CMAKE_CXX_STANDARD)
        message(WARNING "CMAKE_CXX_STANDARD not found, setting it to 11")
        set(CMAKE_CXX_STANDARD "11")
    elseif(CMAKE_CXX_STANDARD LESS 11)
        message(FATAL_ERROR "CMAKE_CXX_STANDARD >= 11 required")
    endif()
    # if Conan is used, we have to include the generated file and doing some basic setup
    if(CONAN_COMPILER)
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
    endif()
    
    find_package(a_util REQUIRED)
    find_package(fep_metamodel REQUIRED)
    enable_testing()
endif ()
find_package (Threads)
find_package(gtest REQUIRED CONFIG)
include(GoogleTest)

if (WIN32 AND CMAKE_CL_64)
    add_definitions(-DWIN64)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(function)