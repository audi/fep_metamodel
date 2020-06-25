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
find_package(a_util REQUIRED)
add_executable(t_fep_system tester_fep_system.cpp)
set_target_properties(t_fep_system PROPERTIES FOLDER test)
target_link_libraries(t_fep_system PRIVATE fep_metamodel
                                            GTest::Main)
target_link_libraries(t_fep_system PUBLIC a_util)
add_test(NAME t_fep_system COMMAND t_fep_system WORKING_DIRECTORY "..")
set_target_properties(t_fep_system PROPERTIES TIMEOUT 10 VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")
