include(CTest)

add_executable (TestStaticAny static_any.cpp)
target_link_libraries(TestStaticAny static_any unconstexpr)

add_executable (TestResult result.cpp)
target_link_libraries(TestResult static_any unconstexpr)

add_test (NAME MyTest COMMAND TestStaticAny TestResult)