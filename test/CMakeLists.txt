find_package(GTest REQUIRED)


add_executable(${PROJECT_NAME}_test_coordinator test_coordinator.cpp )
target_link_libraries(${PROJECT_NAME}_test_coordinator  ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_coordinator  PRIVATE -std=c++17)
add_test(test_coordinator  ${PROJECT_NAME}_test_coordinator )


add_executable(${PROJECT_NAME}_test_transceiver_ranging_euid test_transceiver_ranging_euid.cpp )
target_link_libraries(${PROJECT_NAME}_test_transceiver_ranging_euid ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_transceiver_ranging_euid PRIVATE -std=c++17)
add_test(transceiver_ranging_euid ${PROJECT_NAME}_test_transceiver_ranging_euid)

add_executable(${PROJECT_NAME}_test_transceiver_ranging_result test_transceiver_ranging_result.cpp )
target_link_libraries(${PROJECT_NAME}_test_transceiver_ranging_result ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_transceiver_ranging_result PRIVATE -std=c++17)
add_test(transceiver_ranging_result ${PROJECT_NAME}_test_transceiver_ranging_result)

add_executable(${PROJECT_NAME}_test_transceiver_ranging_status test_transceiver_ranging_status.cpp )
target_link_libraries(${PROJECT_NAME}_test_transceiver_ranging_status ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_transceiver_ranging_status PRIVATE -std=c++17)
add_test(transceiver_ranging_status ${PROJECT_NAME}_test_transceiver_ranging_statut)
