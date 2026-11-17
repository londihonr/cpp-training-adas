# GoogleTest 使用课程提供的版本：把课程 zip 中的 tests/googletest 拷贝到本目录即可。
# 若本机已安装 GoogleTest（例如通过包管理器），则回退到 find_package。
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
    set(ADAS_GTEST_MAIN gtest_main)
else()
    find_package(GTest REQUIRED)
    set(ADAS_GTEST_MAIN GTest::gtest_main)
endif()

aux_source_directory(. SOURCE)

add_executable(training_main ${SOURCE})

target_link_libraries(training_main PRIVATE training ${ADAS_GTEST_MAIN})

if(MSVC)
    target_compile_options(training_main PRIVATE /W4)
else()
    target_compile_options(training_main PRIVATE -Wall -Wextra -Werror)
endif()

add_test(NAME training_main COMMAND training_main)
