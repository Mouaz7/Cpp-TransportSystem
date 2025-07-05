#ifndef LISTPASSENGER_H
#define LISTPASSENGER_H

#include <vector>
#include "Passenger.h"
#include <string>

using namespace std;

class ListPassenger {
private:
    vector<Passenger> passengers;
public:
    void addPassenger(const Passenger& passenger);
    void editPassenger(const string& id, const Passenger& newPassenger);
    void deletePassenger(const string& id);
    const Passenger* getPassengerById(const string& id) const;
    vector<Passenger> getPassengers() const;

    bool loadPassengers(const string& filePath);
    void savePassengers(const string& filePath) const;
    void addEditDeletePassenger(const string& filePath);
};

#endif
