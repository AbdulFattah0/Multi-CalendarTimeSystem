/**	@file Jdn.cpp
    @author Garth Santor
    @date 2012-09-16

    Date and time library to_string method implementations.
    */


#include <khronos/julian_day.hpp>
#include <khronos/gregorian_calendar.hpp>

#include <sstream>

namespace khronos {

    // Converts the Julian Day (JDN) to a string representation.
    std::string Jd::to_string() const {
        std::ostringstream oss;
        oss << "JD " << jd_;  // Format the Julian Day value.
        return oss.str();
    }

    // Default constructor for Jd, initializes to the current local time.
    Jd::Jd() : jd_(0) {
        time_t nowTime = time(NULL);  // Get the current time.
        struct tm tmNow;
        localtime_s(&tmNow, &nowTime);  // Convert to local time.

        // Calculate the Julian Day with the current date and time.
        jd_ = gregorian_to_jd(
            year_t(tmNow.tm_year) + 1900,
            month_t(tmNow.tm_mon) + 1,
            day_t(tmNow.tm_mday)
        ) + tod(tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);
    }

    // Constructor for Jd with options to include or exclude time of day.
    Jd::Jd(now_t n) : jd_(0) {
        time_t nowTime = time(NULL);
        struct tm tmNow;
        localtime_s(&tmNow, &nowTime);  // Convert to local time.

        // Calculate the Julian Day based on the selected option.
        if (n == NOTIMEOFDAY) {
            jd_ = gregorian_to_jd(
                year_t(tmNow.tm_year) + 1900,
                month_t(tmNow.tm_mon) + 1,
                day_t(tmNow.tm_mday)
            );
        }
        else if (n == WTIMEOFDAY) {
            jd_ = gregorian_to_jd(
                year_t(tmNow.tm_year) + 1900,
                month_t(tmNow.tm_mon) + 1,
                day_t(tmNow.tm_mday)
            ) + tod(tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);
        }
    }

    // Converts a Julian Day (JDN) to a Julian Day Number (JDN) 
    jd_t jd_to_jdn(jd_t jd) {
        return std::floor(jd + 0.5);  // Round to the nearest integer.
    }

    // Overload of the << operator to print a Jd object.
    std::ostream& operator<<(std::ostream& os, const Jd& jd) {
        return os << jd.to_string();  // Output the string representation.
    }

}  // end of namespace khronos







