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

## Installation

### Prerequisites

- [Visual Studio 2022](https://visualstudio.microsoft.com/) with C++ development tools  
  *or* any modern C++ compiler with C++11 or later support

### Clone the Repository

```bash
git clone https://github.com/Mouaz7/Cpp-TransportSystem.git
cd Cpp-TransportSystem

Build (using Visual Studio)
Open ProjectFinal.sln in Visual Studio

Set build configuration to Debug or Release

Click Build > Build Solution or press Ctrl+Shift+B

Build (using g++ CLI)
bash
Kopiera
Redigera
g++ -std=c++11 *.cpp -o transport_system
./transport_system
Note: You may need to adjust file names depending on your compiler or environment.

Usage
The system is menu-based and supports:

Adding and listing passengers and shuttles

Scheduling new transport sessions

Saving and loading data from:

passengers.txt

shuttles.txt

schedule.txt

All data is stored in plain text files. Ensure the files are present in the working directory when executing the program.

Technical Details
Language: C++

Paradigm: Object-Oriented Programming (Inheritance, Polymorphism, Encapsulation)

Data Structures: std::vector, file streams

Structure:

Transport, Vehicle (base classes)

Shuttle, Passenger (specialized types)

Schedule (links passenger ↔ shuttle)

ListSchedule, ListPassenger, ListShuttle (managers with file I/O)

Platform: Windows, Visual Studio 2022
