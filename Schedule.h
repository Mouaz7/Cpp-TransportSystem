#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "Shuttle.h"
#include "Passenger.h"

class Schedule {
private:
    Shuttle shuttle;
    Passenger passenger;
public:
    Schedule(const Shuttle& shuttle, const Passenger& passenger);
    Shuttle getShuttle() const;
    Passenger getPassenger() const;
};

#endif
