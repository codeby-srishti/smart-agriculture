# Smart Agriculture - Intelligent Irrigation System

## 1. Project Overview

Smart Agriculture - Intelligent Irrigation System is a Linux-based software project that simulates an automated irrigation system.

The system reads simulated soil moisture, temperature and humidity data through a Linux character device driver. A C++ user-space application processes the sensor data and decides whether irrigation is required.

If the soil moisture falls below a defined threshold, the system automatically turns the simulated water pump ON. If the soil moisture is sufficient, the pump remains OFF.

The project demonstrates concepts from Linux, C++, Computer Architecture, Hardware and Software, device drivers and system-level programming.

---

## 2. Problem Statement

Traditional irrigation systems may provide water at fixed intervals even when the soil already contains sufficient moisture.

This can lead to:

- Water wastage
- Unnecessary irrigation
- Increased energy consumption
- Lack of automatic decision-making

The proposed system uses soil moisture information to make an automatic irrigation decision.

---

## 3. Objectives

The main objectives of the project are:

1. Simulate an agricultural soil-monitoring system.
2. Demonstrate communication between user-space software and a Linux kernel driver.
3. Read sensor data through a Linux character device.
4. Process sensor information using C++.
5. Automatically determine whether irrigation is required.
6. Simulate a water-pump actuator.
7. Store sensor readings and system decisions in a log file.
8. Demonstrate Linux, C++, hardware-software interaction and computer architecture concepts.

---

## 4. Technologies Used

### Software

- Ubuntu Linux
- C++
- C
- Linux Kernel Module
- Linux Character Device Driver
- GCC/G++
- Make
- Bash/Linux commands

### Hardware Simulation

The project does not require physical hardware.

The following hardware components are simulated through software:

- Soil moisture sensor
- Temperature sensor
- Humidity sensor
- Water pump

In a real implementation, these components could be replaced with physical sensors and actuators.

---

## 5. System Architecture

The overall system follows this flow:

Sensor Simulation
        |
        v
Linux Character Device Driver
        |
        v
/dev/smart_sensor
        |
        v
C++ User-Space Application
        |
        v
Sensor Data Processing
        |
        v
Irrigation Decision
        |
        +------------------+
        |                  |
        v                  v
      MOIST                DRY
        |                  |
        v                  v
   Pump OFF             Pump ON
                           |
                           v
                    Irrigation Started
                           |
                           v
                       Log File

---

## 6. Linux Character Device Driver

The project contains a custom Linux character device driver.

Driver source:

`driver/smart_sensor.c`

The driver creates the device:

`/dev/smart_sensor`

The character device provides an interface between the Linux kernel and the user-space C++ application.

The driver implements:

- Device initialization
- Device opening
- Reading sensor data
- Device closing
- Device cleanup

Important file operations include:

- open
- read
- release

The driver uses Linux kernel functions such as:

- alloc_chrdev_region()
- cdev_init()
- cdev_add()
- device_create()
- copy_to_user()

---

## 7. Simulated Sensor

The current simulated sensor provides three values:

- Soil moisture
- Temperature
- Humidity

Example:

SOIL=45 TEMP=31.5 HUMIDITY=58

These values are stored inside the driver for simulation purposes.

In a real hardware system, the values could come from physical sensors using interfaces such as:

- GPIO
- I2C
- SPI
- ADC

---

## 8. C++ Application

The main user-space application is:

`src/main.cpp`

The compiled executable is:

`smart_agriculture`

The application opens:

`/dev/smart_sensor`

It reads the sensor information and extracts:

- Soil moisture percentage
- Temperature
- Humidity

The application then applies the irrigation decision logic.

---

## 9. Irrigation Decision Logic

The system uses a soil moisture threshold of 30%.

### Condition 1 - Sufficient Moisture

If:

`Soil Moisture >= 30%`

the system considers the soil sufficiently moist.

Result:

- Irrigation OFF
- Water Pump OFF

### Condition 2 - Low Moisture

If:

`Soil Moisture < 30%`

the system considers the soil dry.

Result:

- Irrigation ON
- Water Pump ON
- Irrigation started

---

## 10. Actuator Simulation

The project simulates a water pump through software.

The actuator output represents what would happen to a physical pump in a real agricultural system.

Example:

Water Pump : OFF
Action      : Irrigation not required.

For dry soil:

Water Pump : ON
Action      : Irrigation started.

In a physical implementation, this software decision could control a relay or motor driver connected to an actual water pump.

---

## 11. Logging System

The application stores sensor readings and system information in:

`logs/sensor_log.txt`

Logging provides a record of:

- Sensor readings
- Soil condition
- Irrigation decision
- Pump status

This can help with monitoring and debugging.

---

## 12. Computer Architecture Concepts

The project demonstrates the interaction between different layers of a computer system.

### User Space

The C++ application runs in user space.

It requests sensor information from the device file.

### Kernel Space

The Linux character device driver runs in kernel space.

It provides controlled access to the simulated sensor.

### Device Interface

The `/dev/smart_sensor` device acts as the interface between the user application and the kernel driver.

This demonstrates the separation between user-level applications and operating-system-level hardware interfaces.

---

## 13. Hardware and Software Interaction

Although physical hardware is not used, the project models a real hardware-software system.

### Hardware side

In a real system:

Sensor -> Driver -> Application -> Decision -> Actuator

### Current simulation

Software Sensor -> Linux Driver -> C++ Application -> Decision -> Simulated Pump

This makes the project suitable for demonstrating hardware-software interaction without requiring expensive physical components.

---

## 14. Testing

Two major test cases were performed.

### Test Case 1 - Moist Soil

Input:

SOIL=45
TEMP=31.5
HUMIDITY=58

Output:

Soil Moisture : 45%
Status         : MOIST
Irrigation     : OFF
Water Pump     : OFF

Reason:

Soil moisture is sufficient.

Result:

PASS

---

### Test Case 2 - Dry Soil

Input:

SOIL=20
TEMP=31.5
HUMIDITY=58

Output:

Soil Moisture : 20%
Status         : DRY
Irrigation     : ON
Water Pump     : ON

Reason:

Soil moisture is below 30%.

Result:

PASS

---

## 15. Logging Verification

The application successfully generated the message:

Log: Sensor data saved successfully.

This confirms that sensor data and irrigation decisions can be recorded by the system.

---

## 16. Linux Commands Used

Important Linux commands used during development include:

`ls`

Used to list files and directories.

`cd`

Used to navigate between directories.

`make`

Used to compile the Linux kernel module.

`insmod`

Used to load the kernel module.

`rmmod`

Used to remove the kernel module.

`lsmod`

Used to check loaded kernel modules.

`chmod`

Used to modify file/device permissions.

`chown`

Used to change file ownership.

`cat`

Used to display file contents.

`grep`

Used to search for specific text.

`sudo`

Used to execute commands with elevated privileges.

---

## 17. Project Directory Structure

The project is organized as follows:

smart-agriculture/

├── docs/

├── driver/

│   ├── smart_sensor.c

│   ├── smart_sensor.ko

│   └── Makefile

├── include/

├── logs/

├── src/

│   └── main.cpp

└── smart_agriculture

---

## 18. Advantages

The proposed system provides:

- Automatic irrigation decision-making
- Reduced unnecessary watering
- Linux system programming experience
- Hardware-software interaction
- Modular architecture
- Sensor data logging
- Easy extension to physical hardware

---

## 19. Limitations

The current project uses simulated sensor values rather than physical sensors.

The water pump is also simulated through software output.

The system does not currently provide:

- Mobile application control
- Cloud connectivity
- Real-time remote monitoring
- Weather prediction
- Physical sensor hardware

These limitations are intentional because the project focuses on demonstrating Linux, C++, computer architecture and hardware-software concepts within the available development environment.

---

## 20. Future Improvements

The system can be extended by:

1. Connecting a real soil moisture sensor.
2. Connecting a temperature and humidity sensor.
3. Controlling a real water pump through a relay.
4. Adding an IoT communication layer.
5. Adding cloud-based monitoring.
6. Adding a web or mobile dashboard.
7. Storing historical sensor data in a database.
8. Adding weather-based irrigation prediction.
9. Adding multiple agricultural zones.
10. Adding automatic water usage optimization.

---

## 21. Conclusion

The Smart Agriculture - Intelligent Irrigation System successfully demonstrates how Linux, C++, device drivers and hardware-software interaction can be combined to build a real-world inspired automation system.

The Linux character device driver provides sensor data through `/dev/smart_sensor`. The C++ application processes the data and determines whether irrigation is required.

Testing demonstrated both system states:

45% soil moisture resulted in irrigation being OFF.

20% soil moisture resulted in irrigation being ON.

The project therefore demonstrates the complete conceptual flow from sensor data acquisition to automated actuator control and logging.

The system can be further extended to work with physical sensors and actuators, making it suitable as a foundation for a real smart agriculture solution.
