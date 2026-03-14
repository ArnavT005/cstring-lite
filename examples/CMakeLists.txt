add_executable(
    examples
    main.cpp
    one.cpp
    two.cpp
)

target_link_libraries(examples PRIVATE
    ${PROJECT_NAME}
)