#include "ListPassenger.h"
#include "Transport.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void existingPassengers(const ListPassenger& listPassenger) {
    cout << "Existing Passengers - ";
    const auto passengers = listPassenger.getPassengers();
    for (auto it = passengers.begin(); it != passengers.end(); ++it) {
        if (it != passengers.begin()) {
            cout << ", ";
        }
        cout << it->getId();
    }
    cout << endl;
}

void ListPassenger::addPassenger(const Passenger& passenger) {
    passengers.push_back(passenger);
}

void ListPassenger::editPassenger(const string& id, const Passenger& newPassenger) {
    for (auto& passenger : passengers) {
        if (passenger.getId() == id) {
            passenger = newPassenger;
            return;
        }
    }
}

void ListPassenger::deletePassenger(const string& id) {
    passengers.erase(remove_if(passengers.begin(), passengers.end(), [&id](const Passenger& passenger) {
        return passenger.getId() == id;
        }), passengers.end());
}

const Passenger* ListPassenger::getPassengerById(const string& id) const {
    for (const auto& passenger : passengers) {
        if (passenger.getId() == id) {
            return &passenger;
        }
    }
    return nullptr;
}

vector<Passenger> ListPassenger::getPassengers() const {
    return passengers;
}

bool ListPassenger::loadPassengers(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string id, chargingPoint, arrivalTime;
        int groupSize;
        if (!getline(ss, id, ',') || !getline(ss, chargingPoint, ',') || !getline(ss, arrivalTime, ',') || !(ss >> groupSize)) {
            cerr << "Error: Malformed line in " << filePath << ": " << line << endl;
            continue;
        }
        if (Transport::validateTime(arrivalTime)) {
            addPassenger(Passenger(id, chargingPoint, arrivalTime, groupSize));
        }
        else {
            cerr << "Error: Invalid arrival time format in " << filePath << ": " << line << endl;
        }
    }
    return true;
}

void ListPassenger::savePassengers(const string& filePath) const {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return;
    }
    for (const auto& passenger : getPassengers()) {
        file << passenger.getId() << "," << passenger.getChargingPoint() << "," << passenger.getArrivalTime() << "," << passenger.getGroupSize() << endl;
    }
}

void ListPassenger::addEditDeletePassenger(const string& filePath) {
    int choice;
    while (true) {
        bool validInput = false;
        while (!validInput) {
            cout << endl;
            cout << "1. Add Passenger" << endl;
            cout << "2. Edit Passenger" << endl;
            cout << "3. Delete Passenger" << endl;
            cout << "4. Back to Edit Page" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 4 || cin.peek() != '\n') {
                cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                validInput = true;
            }
        }

        if (choice == 4) {
            break;
        }

        string id, chargingPoint, arrivalTime;
        int intId, groupSize;
        ListPassenger tempListPassenger = *this;

        switch (choice) {
        case 1:
            cout << endl;
            while (true) {
                cout << "Enter Passenger ID: P";
                cin >> intId;
                if (cin.fail() || intId < 1 || intId > 99 || cin.peek() != '\n') {
                    cout << "Invalid ID. Please enter an integer between 01 and 99.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                id = "P" + string(2 - to_string(intId).length(), '0') + to_string(intId);
                if (tempListPassenger.getPassengerById(id) != nullptr) {
                    cerr << "Error: ID " << id << " already exists. Please enter a different ID." << endl;
                    existingPassengers(tempListPassenger);
                    cout << endl;
                }
                else {
                    break;
                }
            }

            cout << "Enter Charging Point: ";
            cin >> chargingPoint;
            while (true) {
                cout << "Enter Arrival Time (e.g., 08:05am): ";
                cin >> arrivalTime;
                if (Transport::validateTime(arrivalTime)) {
                    break;
                }
                else {
                    cerr << "Error: Invalid arrival time format.\n" << endl;
                }
            }

            while (true) {
                cout << "Enter Group Size: ";
                cin >> groupSize;
                if (cin.fail() || groupSize <= 0 || groupSize > 15 || cin.peek() != '\n') {
                    cout << "Invalid Group Size. Please enter a number between 1 and 15.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else {
                    break;
                }
            }

            tempListPassenger.addPassenger(Passenger(id, chargingPoint, arrivalTime, groupSize));
            cout << "Successfully added '" << id << " at " << chargingPoint << " at " << arrivalTime << " with group size " << groupSize << "'" << endl;
            break;
        case 2:
            while (true) {
                cout << "Enter Passenger ID to Edit: P";
                cin >> intId;
                if (cin.fail() || intId < 1 || intId > 99 || cin.peek() != '\n') {
                    cout << "Invalid ID. Please enter an integer between 01 and 99.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                id = "P" + string(2 - to_string(intId).length(), '0') + to_string(intId);
                if (tempListPassenger.getPassengerById(id) == nullptr) {
                    cerr << "Error: Passenger ID " << id << " not found. Please enter a different ID." << endl;
                    existingPassengers(tempListPassenger);
                    cout << endl;
                    continue;
                }
                break;
            }

            cout << "Enter new Charging Point: ";
            cin >> chargingPoint;
            while (true) {
                cout << "Enter new Arrival Time (e.g., 08:05am): ";
                cin >> arrivalTime;
                if (Transport::validateTime(arrivalTime)) {
                    break;
                }
                else {
                    cerr << "Error: Invalid arrival time format.\n" << endl;
                }
            }

            while (true) {
                cout << "Enter new Group Size: ";
                cin >> groupSize;
                if (cin.fail() || groupSize <= 0 || groupSize > 15 || cin.peek() != '\n') {
                    cout << "Invalid Group Size. Please enter a number between 1 and 15.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else {
                    break;
                }
            }

            tempListPassenger.editPassenger(id, Passenger(id, chargingPoint, arrivalTime, groupSize));
            cout << "Successfully edited '" << id << " at " << chargingPoint << " at " << arrivalTime << " with group size " << groupSize << "'" << endl;
            break;
        case 3:
            while (true) {
                cout << "Enter Passenger ID to Delete: P";
                cin >> intId;
                if (cin.fail() || intId < 1 || intId > 99 || cin.peek() != '\n') {
                    cout << "Invalid ID. Please enter an integer between 01 and 99.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                id = "P" + string(2 - to_string(intId).length(), '0') + to_string(intId);
                const Passenger* passenger = tempListPassenger.getPassengerById(id);
                if (passenger == nullptr) {
                    cerr << "Error: Passenger ID " << id << " not found. Please enter a different ID." << endl;
                    existingPassengers(tempListPassenger);
                    cout << endl;
                    continue;
                }
                chargingPoint = passenger->getChargingPoint();
                arrivalTime = passenger->getArrivalTime();
                break;
            }
            tempListPassenger.deletePassenger(id);
            cout << "Successfully deleted '" << id << " at " << chargingPoint << " at " << arrivalTime << "'" << endl;
            break;
        default:
            cout << "Invalid choice.\n" << endl;
            continue;
        }

        if (tempListPassenger.getPassengers().size() > 99) {
            cout << "Error: Cannot add more than 99 passengers. Remove a passenger to add a new one." << endl;
            continue;
        }

        *this = tempListPassenger;

        char saveChoice;
        validInput = false;
        while (!validInput) {
            cout << "Do you want to save changes to text file? (y/n): ";
            cin >> saveChoice;
            if (cin.fail() || (saveChoice != 'y' && saveChoice != 'Y' && saveChoice != 'n' && saveChoice != 'N') || cin.peek() != '\n') {
                cout << "Invalid input. Please enter 'y' or 'n'.\n" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                validInput = true;
            }
        }

        if (saveChoice == 'y' || saveChoice == 'Y') {
            savePassengers(filePath);
            cout << "Changes saved to text file." << endl;
        }
        else {
            cout << "Changes not saved to text file." << endl;
        }
    }
}
