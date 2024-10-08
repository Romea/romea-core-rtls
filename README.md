# Romea Core RTLS Library

The **Romea Core RTLS Library** offers a comprehensive set of tools to implement Real-Time Location Systems (RTLS) specifically designed for mobile robotics. The library provides several key features to enable efficient and accurate localization in robotics systems:

## **RTLS Transmitter Coordination:**

The library provides tools for coordinating a set of RTLS transmitters to establish **robot-to-robot**, **robot-to-human**, and **robot-to-world** localization systems. These tools enable the seamless synchronization and management of multiple transmitters, whether deployed in the environment or mounted on robots.

## **Serialization of Data:**

The library includes functionality to serialize information such as **pose** (position and orientation) and **twist** (linear and angular velocities), which are essential for implementing **robot-to-robot** and **robot-to-human** localization systems. This data can be shared between robots, alongside the messages required for range estimation between transmitters.

## **Trilateration Algorithms:**

The library implements trilateration algorithms that estimate the **pose** or **position** of a robot based on the configuration of RTLS transmitters, both those mounted on the robots and those distributed within the environment. These algorithms are key to deriving accurate localization information from distance measurements between the transmitters.



