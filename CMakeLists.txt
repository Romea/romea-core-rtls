cmake_minimum_required(VERSION 2.8.3)
project(romea_rtls)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 ")

#######################
## Find dependencies ##
#######################
set(${PROJECT_NAME}_CATKIN_DEPS romea_common)
find_package(catkin REQUIRED COMPONENTS ${${PROJECT_NAME}_CATKIN_DEPS})

find_package( PkgConfig )
pkg_check_modules( EIGEN3 REQUIRED eigen3 )

###################################
## catkin specific configuration ##
###################################
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS ${${PROJECT_NAME}_CATKIN_DEPS})

###########
## Build ##
###########

include_directories(
  include
  SYSTEM ${catkin_INCLUDE_DIRS}
  SYSTEM ${EIGEN3_INCLUDE_DIRS}
  )

# Declare a C++ library
add_library(romea_rtls
  src/RTLSRangeNoise.cpp
  src/RTLSRangeRandomNoise.cpp
  src/RTLSTransceiverFunction.cpp
  src/RTLSTransceiverPhy.cpp)

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(trackbod_driver ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Specify libraries to link a library or executable target against
 target_link_libraries(romea_rtls ${catkin_LIBRARIES})



#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS trackbod_driver trackbod_driver_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

#get_filename_component(TEST_WITH_DATA_TEST_DIR "test/data" ABSOLUTE)
#configure_file(test/test_helper.h.in test/test_helper.h)
#include_directories(${CMAKE_CURRENT_BINARY_DIR}/test)
#if(CATKIN_ENABLE_TESTING)

#  catkin_add_gtest(${PROJECT_NAME}_test_4ws test/test_4ws.cpp )
#  target_link_libraries(${PROJECT_NAME}_test_4ws ${PROJECT_NAME} ${catkin_LIBRARIES})

#  catkin_add_gtest(${PROJECT_NAME}_test_1as test/test_1as.cpp )
#  target_link_libraries(${PROJECT_NAME}_test_1as ${PROJECT_NAME} ${catkin_LIBRARIES})

#  catkin_add_gtest(${PROJECT_NAME}_test_2ws test/test_2ws.cpp )
#  target_link_libraries(${PROJECT_NAME}_test_2ws ${PROJECT_NAME} ${catkin_LIBRARIES})

#  catkin_add_gtest(${PROJECT_NAME}_test_ss test/test_ss.cpp )
#  target_link_libraries(${PROJECT_NAME}_test_ss ${PROJECT_NAME} ${catkin_LIBRARIES})


#endif(CATKIN_ENABLE_TESTING)

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
