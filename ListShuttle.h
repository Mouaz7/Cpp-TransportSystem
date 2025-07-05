#ifndef LISTSHUTTLE_H
#define LISTSHUTTLE_H

#include <vector>
#include "Shuttle.h"
#include <string>

using namespace std;

class ListShuttle {
private:
    vector<Shuttle> shuttles;
public:
    void addShuttle(const Shuttle& shuttle);
    void editShuttle(const string& id, const Shuttle& newShuttle);
    void deleteShuttle(const string& id);
    const Shuttle* getShuttleById(const string& id) const;
    vector<Shuttle> getShuttles() const;

    bool loadShuttles(const string& filePath);
    void saveShuttles(const string& filePath) const;
    void addEditDeleteShuttle(const string& filePath);
};

#endif
