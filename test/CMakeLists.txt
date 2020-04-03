add_executable(test_string string_test.cpp)
target_link_libraries(test_string tinystl)

add_executable(test_list list_test.cpp)
target_link_libraries(test_list tinystl)

add_executable(test_vector vector_test.cpp)
target_link_libraries(test_vector tinystl)

add_test(NAME string COMMAND test_string)
add_test(NAME list COMMAND test_list)
add_test(NAME vector COMMAND test_vector)
