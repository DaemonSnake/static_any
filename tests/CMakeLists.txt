include(CTest)

add_executable (TestStaticAny static_any.cpp)
target_link_libraries(TestStaticAny static_any unconstexpr)

add_test (NAME MyTest COMMAND TestStaticAny)