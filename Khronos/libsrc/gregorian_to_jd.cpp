/*
khronos\gregorian_to_jdn.cpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'gregorian::to_jdn' implementation.
*/

#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>



namespace khronos {

    // Converts a Gregorian date to Julian Day (JDN).
    jd_t gregorian_to_jd(year_t year, month_t month, day_t day) {
        if (month <= 2) {  // Adjust month and year for January and February.
            year -= 1;
            month += 12;
        }
        jd_t A = static_cast<jd_t>(std::floor(year / 100.0));  // Century value.
        jd_t B = 2 - A + std::floor(A / 4.0);  // Gregorian calendar correction.

        // Calculate the Julian Day Number.
        return std::floor(365.25 * (year + 4716.0)) +
            std::floor(30.6001 * (month + 1.0)) +
            static_cast<jd_t>(day) + B - 1524.5;
    }

    // Converts a Gregorian date with time components to Julian Day (JDN).
    jd_t gregorian_to_jd(year_t year, month_t month, day_t day,
        hour_t hour, minute_t minute, second_t second) {

        jd_t jdn = gregorian_to_jd(year, month, day);  // Get the JDN for the date.

        // Calculate the fractional part of the day.
        double time_fraction = (hour * 3600.0 + minute * 60.0 + second) / 86400.0;

        return jdn + time_fraction;  // Return the JDN with the time fraction added.
    }

}  // end of namespace khronos