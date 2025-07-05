#ifndef SHUTTLE_H
#define SHUTTLE_H

#include "Vehicle.h"

/*
    As the Shuttle class cannot logically inherit from transport class, so this class inherits from the Vehicle Class. 
    It has a virtual function displayInfo() that needs to be defined in order to change this class from its default abstract nature.
*/

class Shuttle : public Vehicle {
private:
    string model;
    int maxSeating;
    void setMaxSeating();
public:
    Shuttle(const string& id, const string& cp, const string& at, const string& model);
    string getModel() const;
    int getMaxSeating() const;
    void displayInfo() const override;                         // Abstract Class Function
};

#endif
