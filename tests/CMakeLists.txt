include(CTest)

add_executable (TestStaticAny static_any.cpp)
target_link_libraries(TestStaticAny static_any)

add_executable (TestResult result.cpp)
target_link_libraries(TestResult static_any)

add_test (NAME ${PROJECT_NAME}_UnitTests COMMAND TestStaticAny TestResult)