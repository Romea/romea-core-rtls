find_package(GTest REQUIRED)

include(romea_add_gtest)

romea_add_gtest(test_simple_trilateration2d test_simple_trilateration2d.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_position2d_estimator test_position2d_estimator.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_pose2d_estimator test_pose2d_estimator.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_round_robin_scheduler test_round_robin_scheduler.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_position_aware_scheduler test_position_aware_scheduler.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_pose2d_serialization test_pose2d_serialization.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_twist2d_serialization test_twist2d_serialization.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_transceiver_euid test_transceiver_euid.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_ranging_result test_ranging_result.cpp LIBRARIES ${PROJECT_NAME})
romea_add_gtest(test_ranging_status test_ranging_status.cpp LIBRARIES ${PROJECT_NAME})
