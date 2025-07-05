#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <unordered_set>
#include "ListShuttle.h"
#include "ListPassenger.h"
#include "ListSchedule.h"

using namespace std;

void displayShuttles(const ListShuttle& listShuttle) {
    cout << "Shuttles:" << endl;
    for (const auto& shuttle : listShuttle.getShuttles()) {
        cout << shuttle.getId() << " at " << shuttle.getChargingPoint() << " at " << shuttle.getArrivalTime() << " with a Maximum Seating of " << shuttle.getMaxSeating() << endl;
    }
}

void displayPassengers(const ListPassenger& listPassenger) {
    cout << "Passengers:" << endl;
    for (const auto& passenger : listPassenger.getPassengers()) {
        cout << passenger.getId() << " to " << passenger.getChargingPoint() << " at " << passenger.getArrivalTime() << " with a Group Size of " << passenger.getGroupSize() << endl;
    }
}

void displayMainMenu() {
    cout << "1. Display all Passengers and Shuttles" << endl;
    cout << "2. Compute Schedule" << endl;
    cout << "3. Compute by Minimum Shuttle Dispatch" << endl;
    cout << "4. Compute by Passenger Arrival Time" << endl;
    cout << "5. Add/Edit/Delete Shuttle or Passenger" << endl;
    cout << "6. Save Schedule to file" << endl;
    cout << "7. Exit" << endl;
}

void displayEditMenu() {
    cout << "1. Modify Shuttle" << endl;
    cout << "2. Modify Passenger" << endl;
    cout << "3. Back to Main Page" << endl;
}

int main() {
    ListShuttle listShuttle;
    ListPassenger listPassenger;
    ListSchedule listSchedule;

    bool scheduleComputed = false;


    listShuttle.loadShuttles("shuttles.txt");
    listPassenger.loadPassengers("passengers.txt");

    int choice;
    while (true) {
        displayMainMenu();

        bool validInput = false;
        while (!validInput) {
            cout << "Enter your choice: ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 7 || cin.peek() != '\n') {
                cout << "Invalid choice. Please enter a number between 1 and 7.\n" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                displayMainMenu();
            }
            else {
                validInput = true;
            }
        }

        switch (choice) {
        case 1:
            cout << endl;
            displayShuttles(listShuttle);
            cout << endl;
            displayPassengers(listPassenger);
            cout << endl;
            break;
        case 2:
            cout << endl;
            listSchedule.computeSchedule(listShuttle, listPassenger);
            scheduleComputed = true;
            cout << endl;
            break;
        case 3:
            cout << endl;
            listSchedule.computeScheduleByMinimumShuttle(listShuttle, listPassenger);
            cout << endl;
            break;
        case 4:
            cout << endl;
            listSchedule.computeScheduleByPassengerArrival(listShuttle, listPassenger);
            cout << endl;
            break;
        case 5:
            cout << endl;
            int subChoice;
            displayEditMenu();

            validInput = false;
            while (!validInput) {
                cout << "Enter your choice: ";
                cin >> subChoice;
                if (cin.fail() || subChoice < 1 || subChoice > 3 || cin.peek() != '\n') {
                    cout << "Invalid choice. Please enter a number between 1 and 3.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    displayEditMenu();
                }
                else {
                    validInput = true;
                }
            }

            if (subChoice == 1) {
                listShuttle.addEditDeleteShuttle("shuttles.txt");
            }
            else if (subChoice == 2) {
                listPassenger.addEditDeletePassenger("passengers.txt");
            }
            else if (subChoice == 3) {
                cout << "Returning to Main Page..." << endl;
            }
            cout << endl;
            break;
        case 6:
            cout << endl;
            if (scheduleComputed) {
                listSchedule.saveSchedule("schedule.txt");
                cout << "Schedule saved to 'schedule.txt'" << endl;
            }
            else {
                cout << "Schedule not computed, please enter 2 to compute." << endl;
            }
            cout << endl;
            break;
        case 7:
            return 0;
        }
    }
    return 0;
}
