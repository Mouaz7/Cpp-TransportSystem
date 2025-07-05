#include "Passenger.h"
#include <iostream>
using namespace std;

Passenger::Passenger(const string& id, const string& cp, const string& at, int groupSize)
    : Transport(id, cp, at), groupSize(groupSize), originalGroupSize(groupSize) {
    if (groupSize <= 0 || groupSize > 15) {
        throw invalid_argument("Invalid group size.");
    }
}

int Passenger::getGroupSize() const {
    return groupSize;
}

int Passenger::getOriginalGroupSize() const {
    return originalGroupSize;
}

void Passenger::setGroupSize(int newGroupSize) {
    if (newGroupSize < 0 || newGroupSize > 15) {
        throw invalid_argument("Invalid group size.");
    }
    this->groupSize = newGroupSize;
}

void Passenger::displayInfo() const {
    cout << "Passenger Group Size: "<<groupSize<<endl;
    cout << "Passenger Original Group Size: " << originalGroupSize << endl;
    cout << "Passenger Arrival Time: " << this->getArrivalTime() << endl;
    cout << "Passenger Charging Point: " << this->getChargingPoint() << endl;
    cout << "Passenger ID: " << this->getId() << endl;
}