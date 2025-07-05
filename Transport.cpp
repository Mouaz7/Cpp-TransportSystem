#include "Transport.h"

Transport::Transport(const string& id, const string& cp, const string& at)
    : id(id), chargingPoint(cp) {
    if (validateTime(at)) {
        arrivalTime = at;
    }
    else {
        throw invalid_argument("Invalid time format.");
    }
}

string Transport::getId() const {
    return id;
}

string Transport::getChargingPoint() const {
    return chargingPoint;
}

string Transport::getArrivalTime() const {
    return arrivalTime;
}

bool Transport::setArrivalTime(const string& at) {
    if (validateTime(at)) {
        arrivalTime = at;
        return true;
    }
    return false;
}

bool Transport::validateTime(const string& timeStr) {
    // Validate format using regex
    regex timeFormat(R"((0[0-9]|1[0-2]):[0-5][0-9](am|pm))");
    if (!regex_match(timeStr, timeFormat)) {
        return false;
    }

    // Manually parse the time string using istringstream
    int hour, minute;
    char period[3];
    char delimiter;

    istringstream ss(timeStr);
    ss >> hour >> delimiter >> minute >> period;

    if (ss.fail() || delimiter != ':' || (strcmp(period, "am") != 0 && strcmp(period, "pm") != 0)) {
        return false;
    }

    // Special case for 12:00am (midnight) and 12:00pm (noon)
    if (hour == 12) {
        hour = (strcmp(period, "am") == 0) ? 0 : 12;
    }
    else {
        // Convert to 24-hour format
        if (strcmp(period, "pm") == 0) {
            hour += 12;
        }
    }

    return true; // Only validate the format, not the operating hours
}
