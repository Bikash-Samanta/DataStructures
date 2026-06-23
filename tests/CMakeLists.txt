enable_testing()

add_executable(ds_tests
    test_array.cpp
)

target_link_libraries(ds_tests
    PRIVATE
        dsa
)

add_test(
    NAME ds_tests
    COMMAND ds_tests
)