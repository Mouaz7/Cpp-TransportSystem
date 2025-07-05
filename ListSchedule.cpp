#include "ListSchedule.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

void ListSchedule::addSchedule(const Schedule& schedule) {
    schedules.push_back(schedule);
}

void ListSchedule::saveSchedule(const string& outputFilePath) const {
    ofstream outFile(outputFilePath);
    for (const auto& schedule : schedules) {
        outFile << "Shuttle " << schedule.getShuttle().getId() << " -> Passenger " << schedule.getPassenger().getId() << " to " << schedule.getPassenger().getChargingPoint() << " at " << schedule.getPassenger().getArrivalTime() << endl;
    }
}

void ListSchedule::clearSchedules() {
    schedules.clear();
}

vector<Schedule>& ListSchedule::getSchedules() {
    return schedules;
}

const vector<Schedule>& ListSchedule::getSchedules() const {
    return schedules;
}

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Helper function to convert time string to tm struct
tm ListSchedule::parseTime(const string& timeStr) const {
    tm timeStruct = {};
    stringstream ss(timeStr);
    ss >> get_time(&timeStruct, "%I:%M%p");
    return timeStruct;
}

// Helper function to check if shuttle can arrive up to 10 minutes early but not late
bool ListSchedule::checkTime(const string& shuttleTimeStr, const string& passengerTimeStr) const {
    tm shuttleTime = parseTime(shuttleTimeStr);
    tm passengerTime = parseTime(passengerTimeStr);

    time_t shuttleEpoch = mktime(&shuttleTime);
    time_t passengerEpoch = mktime(&passengerTime);

    // Shuttle should arrive within 10 minutes or earlier than passenger's arrival time
    return difftime(passengerEpoch, shuttleEpoch) >= 0 && difftime(passengerEpoch, shuttleEpoch) <= 600;
}

// Helper function to check if the time is within the operating hours (6:00am to 12:09am)
bool ListSchedule::isWithinOperatingHours(const string& timeStr) const {
    tm timeStruct = parseTime(timeStr);
    int hour = timeStruct.tm_hour;
    int minute = timeStruct.tm_min;
    // Adjust for 24-hour format if needed
    if (timeStr.find("PM") != string::npos && hour != 12) hour += 12;
    if (timeStr.find("AM") != string::npos && hour == 12) hour = 0;

    if ((hour == 6 && minute >= 0) || (hour > 6 && hour < 24) || (hour == 0 && minute <= 9)) {
        return true;
    }
    return false;
}

bool ListSchedule::match(const Shuttle& shuttle, Passenger& passenger) const {
    return toLower(shuttle.getChargingPoint()) == toLower(passenger.getChargingPoint()) &&
        checkTime(shuttle.getArrivalTime(), passenger.getArrivalTime()) &&
        shuttle.getMaxSeating() >= passenger.getGroupSize();
}

void ListSchedule::computeSchedule(const ListShuttle& listShuttle, const ListPassenger& listPassenger) {
    clearSchedules();
    std::unordered_set<std::string> usedShuttles;

    for (const auto& passenger : listPassenger.getPassengers()) {
        if (!isWithinOperatingHours(passenger.getArrivalTime())) {
            continue;  // Skip passengers outside operating hours
        }
        for (const auto& shuttle : listShuttle.getShuttles()) {
            if (usedShuttles.find(shuttle.getId()) == usedShuttles.end() && match(shuttle, const_cast<Passenger&>(passenger))) {
                addSchedule(Schedule(shuttle, passenger));
                usedShuttles.insert(shuttle.getId());
                break;
            }
        }
    }

    cout << "Computed Schedule:" << endl;
    for (const auto& schedule : schedules) {
        cout << "Shuttle " << schedule.getShuttle().getId() << " -> Passenger " << schedule.getPassenger().getId() << " to " << schedule.getPassenger().getChargingPoint() << " at " << schedule.getPassenger().getArrivalTime() << endl;
    }

    cout << endl;
    cout << "Shuttles without passengers:" << endl;
    for (const auto& shuttle : listShuttle.getShuttles()) {
        bool found = false;
        for (const auto& schedule : getSchedules()) {
            if (schedule.getShuttle().getId() == shuttle.getId()) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << shuttle.getId() << " at " << shuttle.getChargingPoint() << " at " << shuttle.getArrivalTime() << endl;
        }
    }
    cout << endl;
    cout << "Passengers without shuttles:" << endl;
    for (const auto& passenger : listPassenger.getPassengers()) {
        bool found = false;
        for (const auto& schedule : getSchedules()) {
            if (schedule.getPassenger().getId() == passenger.getId()) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << passenger.getId() << " to " << passenger.getChargingPoint() << " at " << passenger.getArrivalTime() << endl;
        }
    }
}

void ListSchedule::computeScheduleByPassengerArrival(const ListShuttle& listShuttle, const ListPassenger& listPassenger) {
    clearSchedules();
    std::unordered_map<std::string, int> shuttleSeatsAvailable;
    auto passengers = listPassenger.getPassengers();
    auto shuttles = listShuttle.getShuttles();
    vector<Passenger> remainingPassengers;
    vector<Passenger> outOfHoursPassengers;

    // Initialize shuttle seat availability
    for (const auto& shuttle : shuttles) {
        shuttleSeatsAvailable[shuttle.getId()] = shuttle.getMaxSeating();
    }

    // Sort passengers by charging point alphabetically and then by arrival time
    sort(passengers.begin(), passengers.end(), [this](const Passenger& a, const Passenger& b) {
        if (toLower(a.getChargingPoint()) == toLower(b.getChargingPoint())) {
            tm timeA = parseTime(a.getArrivalTime());
            tm timeB = parseTime(b.getArrivalTime());
            return difftime(mktime(&timeA), mktime(&timeB)) < 0;
        }
        return toLower(a.getChargingPoint()) < toLower(b.getChargingPoint());
        });

    // Sort shuttles by charging point alphabetically and then by arrival time
    sort(shuttles.begin(), shuttles.end(), [this](const Shuttle& a, const Shuttle& b) {
        if (toLower(a.getChargingPoint()) == toLower(b.getChargingPoint())) {
            tm timeA = parseTime(a.getArrivalTime());
            tm timeB = parseTime(b.getArrivalTime());
            return difftime(mktime(&timeA), mktime(&timeB)) < 0;
        }
        return toLower(a.getChargingPoint()) < toLower(b.getChargingPoint());
        });

    // Display the sorted passengers list
    cout << "Expected Sort List for passengers.txt:\n";
    for (const auto& passenger : passengers) {
        cout << passenger.getId() << "," << passenger.getChargingPoint() << "," << passenger.getArrivalTime() << "," << passenger.getGroupSize() << "\n";
    }
    cout << "\n";

    // Display the sorted shuttles list
    cout << "Expected Sort List for shuttles.txt:\n";
    for (const auto& shuttle : shuttles) {
        cout << shuttle.getId() << "," << shuttle.getChargingPoint() << "," << shuttle.getArrivalTime() << "," << shuttle.getMaxSeating() << "\n";
    }
    cout << "\n";

    // Schedule computation logic
    for (auto& passenger : passengers) {
        if (!isWithinOperatingHours(passenger.getArrivalTime())) {
            outOfHoursPassengers.push_back(passenger);
            continue;  // Skip passengers outside operating hours
        }
        bool matched = false;
        for (auto& shuttle : shuttles) {
            if (match(shuttle, passenger) && shuttleSeatsAvailable[shuttle.getId()] > 0) {
                int seatsToAssign = min(shuttleSeatsAvailable[shuttle.getId()], passenger.getGroupSize());
                addSchedule(Schedule(shuttle, passenger));
                shuttleSeatsAvailable[shuttle.getId()] -= seatsToAssign;
                passenger.setGroupSize(passenger.getGroupSize() - seatsToAssign);
                if (passenger.getGroupSize() == 0) {
                    matched = true;
                    break;  // Move to next passenger
                }
            }
        }
        if (!matched) {
            remainingPassengers.push_back(passenger);
        }
    }

    // Display the computed schedules
    cout << "============== By Passenger Arrival Time ===============\n";
    cout << left << setw(5) << "ID" << setw(5) << "Shu." << setw(15) << "Dest." << setw(10) << "Time" << setw(10) << "Cap." << setw(10) << "Seats Avail." << "<--- "
        << setw(5) << "Pas." << setw(15) << "Dest." << setw(10) << "Time" << setw(5) << "Grp." << setw(5) << "Pax\n";

    for (size_t i = 0; i < schedules.size(); ++i) {
        const auto& schedule = schedules[i];
        cout << left << setw(5) << i + 1
            << setw(5) << schedule.getShuttle().getId()
            << setw(15) << schedule.getShuttle().getChargingPoint()
            << setw(10) << schedule.getShuttle().getArrivalTime()
            << setw(10) << schedule.getShuttle().getMaxSeating()
            << "(" << shuttleSeatsAvailable[schedule.getShuttle().getId()] << ")"
            << "<--- "
            << setw(5) << schedule.getPassenger().getId()
            << setw(15) << schedule.getPassenger().getChargingPoint()
            << setw(10) << schedule.getPassenger().getArrivalTime()
            << setw(5) << schedule.getPassenger().getOriginalGroupSize()
            << "(" << (schedule.getPassenger().getGroupSize() == 0 ? '0' : schedule.getPassenger().getGroupSize()) << ")\n";
    }

    cout << endl;
    cout << "============== Shuttle with Seat Available ==============\n";
    cout << left << setw(5) << "Name" << setw(15) << "Destination" << setw(10) << "Time" << setw(10) << "Capacity" << "Seat\n";

    for (const auto& shuttle : shuttles) {
        if (shuttleSeatsAvailable[shuttle.getId()] > 0) {
            cout << left << setw(5) << shuttle.getId()
                << setw(15) << shuttle.getChargingPoint()
                << setw(10) << shuttle.getArrivalTime()
                << setw(10) << shuttle.getMaxSeating()
                << "(" << shuttleSeatsAvailable[shuttle.getId()] << ")\n";
        }
    }

    cout << endl;
    cout << "============== Remaining Passenger ======================\n";
    cout << left << setw(5) << "Name" << setw(15) << "Destination" << setw(10) << "Time" << setw(10) << "Group" << "Pax\n";

    for (const auto& passenger : remainingPassengers) {
        cout << left << setw(5) << passenger.getId()
            << setw(15) << passenger.getChargingPoint()
            << setw(10) << passenger.getArrivalTime()
            << setw(10) << passenger.getOriginalGroupSize()
            << "(" << (passenger.getGroupSize() == 0 ? '0' : passenger.getGroupSize()) << ")\n";
    }

    cout << endl;
    cout << "============== Passengers Outside Operating Hours ======================\n";
    cout << left << setw(5) << "Name" << setw(15) << "Destination" << setw(10) << "Time" << setw(10) << "Group" << "Pax\n";

    for (const auto& passenger : outOfHoursPassengers) {
        cout << left << setw(5) << passenger.getId()
            << setw(15) << passenger.getChargingPoint()
            << setw(10) << passenger.getArrivalTime()
            << setw(10) << passenger.getOriginalGroupSize()
            << "(" << (passenger.getGroupSize() == 0 ? '0' : passenger.getGroupSize()) << ")\n";
    }

    cout << endl;
    cout << "(#) - Shuttle: Number of Seat\n";
    cout << "(#) - Passenger: Number of Pax\n";
}

void ListSchedule::computeScheduleByMinimumShuttle(const ListShuttle& listShuttle, const ListPassenger& listPassenger) {
    clearSchedules();
    std::unordered_set<std::string> usedShuttles;

    auto passengers = listPassenger.getPassengers();
    auto shuttles = listShuttle.getShuttles();

    // Sort passengers by charging point (alphabetically) and then by group size (descending)
    std::sort(passengers.begin(), passengers.end(), [](const Passenger& a, const Passenger& b) {
        if (a.getChargingPoint() == b.getChargingPoint()) {
            return a.getGroupSize() > b.getGroupSize();
        }
        return a.getChargingPoint() < b.getChargingPoint();
        });

    // Sort shuttles by charging point (alphabetically) and then by max seating (descending)
    std::sort(shuttles.begin(), shuttles.end(), [](const Shuttle& a, const Shuttle& b) {
        if (a.getChargingPoint() == b.getChargingPoint()) {
            return a.getMaxSeating() > b.getMaxSeating();
        }
        return a.getChargingPoint() < b.getChargingPoint();
        });

    for (const auto& passenger : passengers) {
        if (!isWithinOperatingHours(passenger.getArrivalTime())) {
            continue;  // Skip passengers outside operating hours
        }
    }

    cout << "Expected Sort List for passengers.txt:\n";
    for (const auto& passenger : passengers) {
        cout << passenger.getId() << "," << passenger.getChargingPoint() << "," << passenger.getArrivalTime() << "," << passenger.getGroupSize() << "\n";
    }

    cout << "\nExpected Sort List for shuttles.txt:\n";
    for (const auto& shuttle : shuttles) {
        cout << shuttle.getId() << "," << shuttle.getChargingPoint() << "," << shuttle.getArrivalTime() << "," << shuttle.getModel() << "\n";
    }
    cout << endl;


    // Print the results
    cout << "Computed Match for the Minimum Shuttle Dispatch:\n";
    cout << left << setw(5) << "ID" << setw(5) << "Shu." << setw(20) << "Dest." << setw(12) << "Time" << setw(5) << "Cap." << setw(5) << "Seat" << setw(7) << "Pas." << setw(20) << "Dest." << setw(12) << "Time" << setw(7) << "Grp." << setw(5) << "Pax\n";

    for (size_t i = 0; i < schedules.size(); ++i) {
        const auto& schedule = schedules[i];
        int remainingSeats = schedule.getShuttle().getMaxSeating() - schedule.getPassenger().getGroupSize();
        int remainingPax = std::max(schedule.getPassenger().getGroupSize() - schedule.getShuttle().getMaxSeating(), 0);
        cout << left << setw(5) << i + 1
            << setw(5) << schedule.getShuttle().getId()
            << setw(20) << schedule.getShuttle().getChargingPoint()
            << setw(12) << schedule.getShuttle().getArrivalTime()
            << setw(5) << schedule.getShuttle().getMaxSeating()
            << setw(5) << remainingSeats
            << "<-- "
            << setw(7) << schedule.getPassenger().getId()
            << setw(20) << schedule.getPassenger().getChargingPoint()
            << setw(12) << schedule.getPassenger().getArrivalTime()
            << setw(7) << schedule.getPassenger().getGroupSize()
            << setw(5) << remainingPax << "\n";
    }

    cout << "\nShuttles without passengers:\n";
    for (const auto& shuttle : shuttles) {
        if (usedShuttles.find(shuttle.getId()) == usedShuttles.end()) {
            cout << shuttle.getId() << " at " << shuttle.getChargingPoint() << " at " << shuttle.getArrivalTime() << "\n";
        }
    }

    cout << "\nPassengers without shuttles:\n";
    for (const auto& passenger : passengers) {
        bool found = false;
        for (const auto& schedule : getSchedules()) {
            if (schedule.getPassenger().getId() == passenger.getId()) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << passenger.getId() << " to " << passenger.getChargingPoint() << " at " << passenger.getArrivalTime() << "\n";
        }
    }
}