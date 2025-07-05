#include "Shuttle.h"
#include <iostream>
using namespace std;

Shuttle::Shuttle(const string& id, const string& cp, const string& at, const string& model)
    : Vehicle(id, cp, at), model(model) {
    setMaxSeating();
}

void Shuttle::setMaxSeating() {
    if (model == "Small") {
        maxSeating = 2;
    }
    else if (model == "Family") {
        maxSeating = 5;
    }
    else if (model == "Premium") {
        maxSeating = 12;
    }
    else {
        throw invalid_argument("Invalid model type.");
    }
}

string Shuttle::getModel() const {
    return model;
}

int Shuttle::getMaxSeating() const {
    return maxSeating;
}

void Shuttle::displayInfo() const {
    cout << "Shuttle Model: " << model << endl;
    cout << "Shuttle Max Seating: " << maxSeating << endl;
    cout << "Shuttle Arrival Time: " << this->getArrivalTime() << endl;
    cout << "Shuttle Charging Point: " << this->getChargingPoint() << endl;
    cout << "Shuttle ID: " << this->getId() << endl;
}