#ifndef LISTSCHEDULE_H
#define LISTSCHEDULE_H

#include <vector>
#include "Schedule.h"
#include "ListShuttle.h"
#include "ListPassenger.h"
#include <unordered_set>

using namespace std;

class ListSchedule {
private:
    vector<Schedule> schedules;
    bool isWithinOperatingHours(const string& timeStr) const;
    tm parseTime(const string& timeStr) const;
    bool checkTime(const string& shuttleTimeStr, const string& passengerTimeStr) const;

public:
    void addSchedule(const Schedule& schedule);
    void saveSchedule(const string& outputFilePath) const;
    void clearSchedules();
    vector<Schedule>& getSchedules();
    const vector<Schedule>& getSchedules() const;
    bool match(const Shuttle& shuttle, Passenger& passenger) const;
    void computeSchedule(const ListShuttle& listShuttle, const ListPassenger& listPassenger);
    void computeScheduleByPassengerArrival(const ListShuttle& listShuttle, const ListPassenger& listPassenger);
    void computeScheduleByMinimumShuttle(const ListShuttle& listShuttle, const ListPassenger& listPassenger);
};

#endif
