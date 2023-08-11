find_package(GTest REQUIRED)


add_executable(${PROJECT_NAME}_test_simple_trialteration test_simple_trilateration.cpp)
target_link_libraries(${PROJECT_NAME}_test_simple_trialteration ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_simple_trialteration PRIVATE -std=c++17)
add_test(test ${PROJECT_NAME}_test_simple_trialteration)

add_executable(${PROJECT_NAME}_test_position_estimator test_position_estimator.cpp)
target_link_libraries(${PROJECT_NAME}_test_position_estimator  ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_position_estimator PRIVATE -std=c++17)
add_test(test ${PROJECT_NAME}_test_position_estimator)

add_executable(${PROJECT_NAME}_test_pose_estimator test_pose_estimator.cpp)
target_link_libraries(${PROJECT_NAME}_test_pose_estimator  ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_pose_estimator PRIVATE -std=c++17)
add_test(test ${PROJECT_NAME}_test_pose_estimator)

add_executable(${PROJECT_NAME}_test_simple_coordinator_scheduler test_simple_coordinator_scheduler.cpp)
target_link_libraries(${PROJECT_NAME}_test_simple_coordinator_scheduler   ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_simple_coordinator_scheduler   PRIVATE -std=c++17)
add_test(test_simple_coordinator_scheduler   ${PROJECT_NAME}_test_simple_coordinator_scheduler)

add_executable(${PROJECT_NAME}_test_georeferenced_coordinator_scheduler test_georeferenced_coordinator_scheduler.cpp)
target_link_libraries(${PROJECT_NAME}_test_georeferenced_coordinator_scheduler   ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_georeferenced_coordinator_scheduler   PRIVATE -std=c++17)
add_test(test_georeferenced_coordinator_scheduler   ${PROJECT_NAME}_test_georeferenced_coordinator_scheduler)

add_executable(${PROJECT_NAME}_test_pose2d_serialization test_pose2d_serialization.cpp)
target_link_libraries(${PROJECT_NAME}_test_pose2d_serialization    ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_pose2d_serialization    PRIVATE -std=c++17)
add_test(test_pose2d_serialization    ${PROJECT_NAME}_test_pose2d_serialization )

add_executable(${PROJECT_NAME}_test_twist2d_serialization test_twist2d_serialization.cpp)
target_link_libraries(${PROJECT_NAME}_test_twist2d_serialization    ${PROJECT_NAME} GTest::GTest GTest::Main)
target_compile_options(${PROJECT_NAME}_test_twist2d_serialization    PRIVATE -std=c++17)
add_test(test_twist2d_serialization    ${PROJECT_NAME}_test_twist2d_serialization )

# add_executable(${PROJECT_NAME}_test_simple_coordinator_poller test_simple_coordinator_poller.cpp )
# target_link_libraries(${PROJECT_NAME}_test_simple_coordinator_poller  ${PROJECT_NAME} GTest::GTest GTest::Main)
# target_compile_options(${PROJECT_NAME}_test_simple_coordinator_poller  PRIVATE -std=c++17)
# add_test(test_simple_coordinator_poller  ${PROJECT_NAME}_test_simple_coordinator_poller )

# add_executable(${PROJECT_NAME}_test_georeferenced_coordinator_poller test_georeferenced_coordinator_poller.cpp )
# target_link_libraries(${PROJECT_NAME}_test_georeferenced_coordinator_poller  ${PROJECT_NAME} GTest::GTest GTest::Main)
# target_compile_options(${PROJECT_NAME}_test_georeferenced_coordinator_poller  PRIVATE -std=c++17)
# add_test(test_georeferenced_coordinator_poller  ${PROJECT_NAME}_test_georeferenced_coordinator_poller )

# add_executable(${PROJECT_NAME}_test_georeferenced_coordinator test_georeferenced_coordinator.cpp )
# target_link_libraries(${PROJECT_NAME}_test_georeferenced_coordinator  ${PROJECT_NAME} GTest::GTest GTest::Main)
# target_compile_options(${PROJECT_NAME}_test_georeferenced_coordinator  PRIVATE -std=c++17)
# add_test(test_georeferenced_coordinator  ${PROJECT_NAME}_test_georeferenced_coordinator )

# add_executable(${PROJECT_NAME}_test_mapping test_mapping.cpp )
# target_link_libraries(${PROJECT_NAME}_test_mapping  ${PROJECT_NAME} GTest::GTest GTest::Main)
# target_compile_options(${PROJECT_NAME}_test_mapping  PRIVATE -std=c++17)
# add_test(test_mapping  ${PROJECT_NAME}_test_mapping )
