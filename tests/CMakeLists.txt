include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    cstring_view.cpp
    cstring.cpp
)

if(NOT MSVC)
    target_compile_options(tests PRIVATE
        # Suppress self-move warning in CStringTest_MoveAssignmentSelf test case
        -Wno-self-move
    )
endif()

target_link_libraries(tests PRIVATE
    ${PROJECT_NAME}
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)