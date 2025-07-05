#include "Schedule.h"

Schedule::Schedule(const Shuttle& shuttle, const Passenger& passenger)
    : shuttle(shuttle), passenger(passenger) {}

Shuttle Schedule::getShuttle() const {
    return shuttle;
}

Passenger Schedule::getPassenger() const {
    return passenger;
}
