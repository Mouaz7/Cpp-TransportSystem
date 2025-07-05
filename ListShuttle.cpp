#include "ListShuttle.h"
#include "Transport.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void existingShuttles(const ListShuttle& listShuttle) {
    cout << "Existing Shuttles - ";
    const auto shuttles = listShuttle.getShuttles();
    for (auto it = shuttles.begin(); it != shuttles.end(); ++it) {
        if (it != shuttles.begin()) {
            cout << ", ";
        }
        cout << it->getId();
    }
    cout << endl;
}

void ListShuttle::addShuttle(const Shuttle& shuttle) {
    shuttles.push_back(shuttle);
}

void ListShuttle::editShuttle(const string& id, const Shuttle& newShuttle) {
    for (auto& shuttle : shuttles) {
        if (shuttle.getId() == id) {
            shuttle = newShuttle;
            return;
        }
    }
}

void ListShuttle::deleteShuttle(const string& id) {
    shuttles.erase(remove_if(shuttles.begin(), shuttles.end(), [&id](const Shuttle& shuttle) {
        return shuttle.getId() == id;
        }), shuttles.end());
}

const Shuttle* ListShuttle::getShuttleById(const string& id) const {
    for (const auto& shuttle : shuttles) {
        if (shuttle.getId() == id) {
            return &shuttle;
        }
    }
    return nullptr;
}

vector<Shuttle> ListShuttle::getShuttles() const {
    return shuttles;
}

bool ListShuttle::loadShuttles(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string id, chargingPoint, arrivalTime, model;
        if (!getline(ss, id, ',') || !getline(ss, chargingPoint, ',') || !getline(ss, arrivalTime, ',') || !getline(ss, model, ',')) {
            cerr << "Error: Malformed line in " << filePath << ": " << line << endl;
            continue;
        }
        if (Transport::validateTime(arrivalTime)) {
            addShuttle(Shuttle(id, chargingPoint, arrivalTime, model));
        }
        else {
            cerr << "Error: Invalid arrival time format in " << filePath << ": " << line << endl;
        }
    }
    return true;
}

void ListShuttle::saveShuttles(const string& filePath) const {
    ofstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return;
    }
    for (const auto& shuttle : getShuttles()) {
        file << shuttle.getId() << "," << shuttle.getChargingPoint() << "," << shuttle.getArrivalTime() << "," << shuttle.getModel() << endl;
    }
}

void ListShuttle::addEditDeleteShuttle(const string& filePath) {
    int choice;
    while (true) {
        bool validInput = false;
        while (!validInput) {
            cout << endl;
            cout << "1. Add Shuttle" << endl;
            cout << "2. Edit Shuttle" << endl;
            cout << "3. Delete Shuttle" << endl;
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

        string id, chargingPoint, arrivalTime, model;
        int intId;
        ListShuttle tempListShuttle = *this;

        switch (choice) {
        case 1:
            cout << endl;
            while (true) {
                cout << "Enter Shuttle ID: S";
                cin >> intId;
                if (cin.fail() || intId < 1 || intId > 99 || cin.peek() != '\n') {
                    cout << "Invalid ID. Please enter an integer between 01 and 99.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                id = "S" + string(2 - to_string(intId).length(), '0') + to_string(intId);
                if (tempListShuttle.getShuttleById(id) != nullptr) {
                    cerr << "Error: ID " << id << " already exists. Please enter a different ID." << endl;
                    existingShuttles(tempListShuttle);
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
                cout << "Enter Model (Small, Family, Premium): ";
                cin >> model;
                if (model != "Small" && model != "Family" && model != "Premium") {
                    cout << "Invalid Model. Please enter 'Small', 'Family', or 'Premium'.\n" << endl;
                }
                else {
                    break;
                }
            }

            tempListShuttle.addShuttle(Shuttle(id, chargingPoint, arrivalTime, model));
            cout << "Successfully added '" << id << " at " << chargingPoint << " at " << arrivalTime << " with model " << model << "'" << endl;
            break;
        case 2:
            while (true) {
                cout << "Enter Shuttle ID to Edit: S";
                cin >> intId;
                if (cin.fail() || intId < 1 || intId > 99 || cin.peek() != '\n') {
                    cout << "Invalid ID. Please enter an integer between 01 and 99.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                id = "S" + string(2 - to_string(intId).length(), '0') + to_string(intId);
                if (tempListShuttle.getShuttleById(id) == nullptr) {
                    cerr << "Error: Shuttle ID " << id << " not found. Please enter a different ID." << endl;
                    existingShuttles(tempListShuttle);
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
                cout << "Enter new Model (Small, Family, Premium): ";
                cin >> model;
                if (model != "Small" && model != "Family" && model != "Premium") {
                    cout << "Invalid Model. Please enter 'Small', 'Family', or 'Premium'.\n" << endl;
                }
                else {
                    break;
                }
            }

            tempListShuttle.editShuttle(id, Shuttle(id, chargingPoint, arrivalTime, model));
            cout << "Successfully edited '" << id << " at " << chargingPoint << " at " << arrivalTime << " with model " << model << "'" << endl;
            break;
        case 3:
            while (true) {
                cout << "Enter Shuttle ID to Delete: S";
                cin >> intId;
                if (cin.fail() || intId < 1 || intId > 99 || cin.peek() != '\n') {
                    cout << "Invalid ID. Please enter an integer between 01 and 99.\n" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                id = "S" + string(2 - to_string(intId).length(), '0') + to_string(intId);
                const Shuttle* shuttle = tempListShuttle.getShuttleById(id);
                if (shuttle == nullptr) {
                    cerr << "Error: Shuttle ID " << id << " not found. Please enter a different ID." << endl;
                    existingShuttles(tempListShuttle);
                    cout << endl;
                    continue;
                }
                chargingPoint = shuttle->getChargingPoint();
                arrivalTime = shuttle->getArrivalTime();
                break;
            }
            tempListShuttle.deleteShuttle(id);
            cout << "Successfully deleted '" << id << " at " << chargingPoint << " at " << arrivalTime << "'" << endl;
            break;
        default:
            cout << "Invalid choice.\n" << endl;
            continue;
        }

        if (tempListShuttle.getShuttles().size() > 99) {
            cout << "Error: Cannot add more than 99 shuttles. Remove a shuttle to add a new one." << endl;
            continue;
        }

        *this = tempListShuttle;

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
            saveShuttles(filePath);
            cout << "Changes saved to text file." << endl;
        }
        else {
            cout << "Changes not saved to text file." << endl;
        }
    }
}
