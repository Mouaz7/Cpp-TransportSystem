#ifndef VEHICLE_H
#define VEHICLE_H

#include "Transport.h"


/*
    The Vehicle class is another Abstract class that inherits from Transport class and doesnt define the displayInfo() function.
    Due to this it, stays abstract. The purpose of this class is to create the logic that a shuttle inherits from a vehicle.
*/


class Vehicle : public Transport {                  // A new vehicle class was constructed to make the logical inheritiance applicable.
private:
    
public:
    Vehicle(const string& id, const string& cp, const string& at);
};

#endif
