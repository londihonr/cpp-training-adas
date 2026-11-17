file(GLOB SOURCE "*.cpp")

add_library(training STATIC ${SOURCE})

target_include_directories(training PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(MSVC)
    target_compile_options(training PRIVATE /W4)
else()
    target_compile_options(training PRIVATE -Wall -Wextra -Werror -Wnon-virtual-dtor -Woverloaded-virtual)
endif()
