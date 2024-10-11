# Romea Core RTLS Library

The **Romea Core RTLS Library** offers a comprehensive set of tools to implement Real-Time Location Systems (RTLS) specifically designed for mobile robotics. The library provides several key features to enable efficient and accurate localization in robotics systems:

## **RTLS Transmitter Coordination:**

The library provides tools for coordinating a set of RTLS transmitters to establish **robot-to-robot**, **robot-to-human**, and **robot-to-world** localization systems. These tools enable the seamless synchronization and management of multiple transmitters, whether deployed in the environment or mounted on robots.

## **Serialization of Data:**

The library includes functionality to serialize information such as **pose** (position and orientation) and **twist** (linear and angular velocities), which are essential for implementing **robot-to-robot** and **robot-to-human** localization systems. This data can be shared between robots, alongside the messages required for range estimation between transmitters.

## **Trilateration Algorithms:**

The library implements trilateration algorithms that estimate the **pose** or **position** of a robot based on the configuration of RTLS transmitters, both those mounted on the robots and those distributed within the environment. These algorithms are key to deriving accurate localization information from distance measurements between the transmitters.


## **Usage**

1. create a ROS workspace
2. cd worskpace
3. mkdir src
4. wget https://raw.githubusercontent.com/Romea/romea-core-rtls/refs/heads/main/romea_rtls_public.repos
5. vcs import src < romea_rtls_public.repos
6. build packages
   - catkin build for ROS1
   - colcon build for ROS2
7. create your application using this library

## **Contributing**

If you'd like to contribute to this project, here are some guidelines:

1. Fork the repository.
2. Create a new branch for your changes.
3. Make your changes.
4. Write tests to cover your changes.
5. Run the tests to ensure they pass.
6. Commit your changes.
7. Push your changes to your forked repository.
8. Submit a pull request.

## **License**

This project is released under the Apache License 2.0. See the LICENSE file for details.

## **Authors**

The Romea Core RTLS library was developed by **Jean Laneurit** in the context of various research projects carried out at INRAE.

## **Contact**

If you have any questions or comments about Romea Core RTLS library, please contact **[Jean Laneurit](mailto:jean.laneurit@inrae.fr)** 
