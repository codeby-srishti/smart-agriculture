# 🌱 Smart Agriculture - Intelligent Irrigation System

A Linux-based smart agriculture system that monitors simulated soil conditions and automatically decides whether irrigation is required.

The project demonstrates **Linux kernel programming, C++, character device drivers, hardware-software interaction, actuator control, and system logging** without requiring physical hardware.

---

## 🚀 Project Overview

Traditional irrigation systems may waste water by irrigating crops even when the soil already contains sufficient moisture.

This project simulates an intelligent irrigation system where:

**Sensor → Linux Driver → C++ Application → Decision Engine → Water Pump**

The system reads soil moisture, temperature, and humidity data through a custom Linux character device driver.

If soil moisture falls below **30%**, irrigation is automatically switched ON.

---

## 🏗️ System Architecture

```text
             SIMULATED SENSOR
                    │
                    ▼
          ┌─────────────────────┐
          │ Linux Character     │
          │ Device Driver       │
          └──────────┬──────────┘
                     │
                     ▼
              /dev/smart_sensor
                     │
                     ▼
          ┌─────────────────────┐
          │ C++ User-Space      │
          │ Application         │
          └──────────┬──────────┘
                     │
                     ▼
             Decision Engine
                     │
             ┌───────┴───────┐
             ▼               ▼
          MOIST              DRY
             │               │
             ▼               ▼
         Pump OFF         Pump ON
                             │
                             ▼
                    Irrigation Started
                             │
                             ▼
                         Log File
