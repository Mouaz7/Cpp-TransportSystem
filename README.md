## UML Class Diagram

![UML](UML.png)

# Cpp-TransportSystem

A C++ application for managing transport schedules, shuttle assignments, and passenger bookings.

## Overview

Cpp-TransportSystem is a modular, object-oriented C++ project that simulates a basic transport management platform. It provides core functionality for registering passengers, defining shuttles, and scheduling transport between them under defined time constraints.

The system is designed using clean architectural principles and includes a clear separation between data models, scheduling logic, and file persistence.

## Features

- 🚌 Add and manage shuttles (model, capacity)
- 👥 Register passenger groups
- 🗓️ Schedule trips between shuttles and passengers
- ⏱ Validate time constraints and operating hours
- 💾 Load and save all data from/to text files
- 📊 View, list and manage current schedules
- 🧩 Modular OOP design with abstract base classes and derived components

## Technical Details

- **Language**: C++
- **Paradigm**: Object-Oriented Programming (Inheritance, Polymorphism, Encapsulation)
- **Data Structures**: `std::vector`, file streams
- **Structure**:
  - `Transport`, `Vehicle` (base classes)
  - `Shuttle`, `Passenger` (specialized types)
  - `Schedule` (links passenger ↔ shuttle)
  - `ListSchedule`, `ListPassenger`, `ListShuttle` (managers with file I/O)
- **Platform**: Windows, Visual Studio 2022

## Purpose

The project is developed as part of a software engineering course with the goal of applying object-oriented programming principles to a realistic, file-based scheduling scenario.

