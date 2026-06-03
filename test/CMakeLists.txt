find_package(GTest REQUIRED)


add_executable(${PROJECT_NAME}_test_simple_trilateration2d test_simple_trilateration2d.cpp)
target_link_libraries(${PROJECT_NAME}_test_simple_trilateration2d ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_simple_trilateration2d PRIVATE -std=c++17)
add_test(test_simple_trilateration2d ${PROJECT_NAME}_test_simple_trilateration2d)

add_executable(${PROJECT_NAME}_test_position2d_estimator test_position2d_estimator.cpp)
target_link_libraries(${PROJECT_NAME}_test_position2d_estimator  ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_position2d_estimator PRIVATE -std=c++17)
add_test(test_position2d_estimator ${PROJECT_NAME}_test_position2d_estimator)

add_executable(${PROJECT_NAME}_test_pose2d_estimator test_pose2d_estimator.cpp)
target_link_libraries(${PROJECT_NAME}_test_pose2d_estimator  ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_pose2d_estimator PRIVATE -std=c++17)
add_test(test_pose2d_estimator ${PROJECT_NAME}_test_pose2d_estimator)

add_executable(${PROJECT_NAME}_test_round_robin_scheduler test_round_robin_scheduler.cpp)
target_link_libraries(${PROJECT_NAME}_test_round_robin_scheduler   ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_round_robin_scheduler   PRIVATE -std=c++17)
add_test(test_round_robin_scheduler   ${PROJECT_NAME}_test_round_robin_scheduler)

add_executable(${PROJECT_NAME}_test_position_aware_scheduler test_position_aware_scheduler.cpp)
target_link_libraries(${PROJECT_NAME}_test_position_aware_scheduler   ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_position_aware_scheduler   PRIVATE -std=c++17)
add_test(test_position_aware_scheduler   ${PROJECT_NAME}_test_position_aware_scheduler)

add_executable(${PROJECT_NAME}_test_pose2d_serialization test_pose2d_serialization.cpp)
target_link_libraries(${PROJECT_NAME}_test_pose2d_serialization    ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_pose2d_serialization    PRIVATE -std=c++17)
add_test(test_pose2d_serialization    ${PROJECT_NAME}_test_pose2d_serialization )

add_executable(${PROJECT_NAME}_test_twist2d_serialization test_twist2d_serialization.cpp)
target_link_libraries(${PROJECT_NAME}_test_twist2d_serialization    ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_twist2d_serialization    PRIVATE -std=c++17)
add_test(test_twist2d_serialization    ${PROJECT_NAME}_test_twist2d_serialization )


add_executable(${PROJECT_NAME}_test_transceiver_euid test_transceiver_euid.cpp )
target_link_libraries(${PROJECT_NAME}_test_transceiver_euid ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_transceiver_euid PRIVATE -std=c++17)
add_test(transceiver_euid ${PROJECT_NAME}_test_transceiver_euid)

add_executable(${PROJECT_NAME}_test_ranging_result test_ranging_result.cpp )
target_link_libraries(${PROJECT_NAME}_test_ranging_result ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_ranging_result PRIVATE -std=c++17)
add_test(ranging_result ${PROJECT_NAME}_test_ranging_result)

add_executable(${PROJECT_NAME}_test_ranging_status test_ranging_status.cpp )
target_link_libraries(${PROJECT_NAME}_test_ranging_status ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_ranging_status PRIVATE -std=c++17)
add_test(ranging_status ${PROJECT_NAME}_test_ranging_status)

# add_executable(${PROJECT_NAME}_test_simple_coordinator_poller test_simple_coordinator_poller.cpp )
# target_link_libraries(${PROJECT_NAME}_test_simple_coordinator_poller  ${PROJECT_NAME} GTest::GTest GTest::Main)
# target_compile_options(${PROJECT_NAME}_test_simple_coordinator_poller  PRIVATE -std=c++17)
# add_test(test_simple_coordinator_poller  ${PROJECT_NAME}_test_simple_coordinator_poller )

# add_executable(${PROJECT_NAME}_test_position_aware_poller test_position_aware_poller.cpp )
# target_link_libraries(${PROJECT_NAME}_test_position_aware_poller  ${PROJECT_NAME} GTest::GTest GTest::Main)
# target_compile_options(${PROJECT_NAME}_test_position_aware_poller  PRIVATE -std=c++17)
# add_test(test_position_aware_poller  ${PROJECT_NAME}_test_position_aware_poller )

# add_executable(${PROJECT_NAME}_test_georeferenced_coordinator test_georeferenced_coordinator.cpp )
# target_link_libraries(${PROJECT_NAME}_test_georeferenced_coordinator  ${PROJECT_NAME} GTest::GTest GTest::Main)
# target_compile_options(${PROJECT_NAME}_test_georeferenced_coordinator  PRIVATE -std=c++17)
# add_test(test_georeferenced_coordinator  ${PROJECT_NAME}_test_georeferenced_coordinator )

# add_executable(${PROJECT_NAME}_test_mapping test_mapping.cpp )
# target_link_libraries(${PROJECT_NAME}_test_mapping  ${PROJECT_NAME} GTest::GTest GTest::Main)
# target_compile_options(${PROJECT_NAME}_test_mapping  PRIVATE -std=c++17)
# add_test(test_mapping  ${PROJECT_NAME}_test_mapping )
