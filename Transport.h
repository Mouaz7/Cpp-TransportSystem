#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>

using namespace std;

/*
    This is the main abstract class that is inherited in all the child classes.
    It has a virtual function displayInfo() that is inherited in the child classes and shows that this class is abstract.
*/

class Transport {                                            
private:
    string id;
    string chargingPoint;
    string arrivalTime;
public:
    Transport(const string& id, const string& cp, const string& at);
    virtual ~Transport() = default;
    virtual void displayInfo() const = 0;                      // Pure virtual function (abstract class)

    string getId() const;
    string getChargingPoint() const;
    string getArrivalTime() const;
    bool setArrivalTime(const string& at);
    static bool validateTime(const string& timeStr);
};

#endif
