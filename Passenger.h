#ifndef PASSENGER_H
#define PASSENGER_H

#include "Transport.h"

/*
    This class inherits from the Transport Class as their is a direct link between the two. It has a virtual function displayInfo() 
    that needs to be defined in order to change this class from its default abstract nature.
*/

class Passenger : public Transport {
private:
    int groupSize;
    int originalGroupSize;
public:
    Passenger(const string& id, const string& cp, const string& at, int groupSize);
    int getGroupSize() const;
    int getOriginalGroupSize() const;
    void setGroupSize(int newGroupSize);
    void displayInfo() const override;                         // Abstract Class Function
};

#endif
