/*
khronos\jdn_to_gregorian.cpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'gregorian::from_jdn' implementation.
*/

#include <khronos/gregorian_calendar.hpp>

namespace khronos {

    // Converts a Julian Day (JDN) to a Gregorian date (year, month, day).
    void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day) {
        int Z = static_cast<int>(std::floor(jd + 0.5));  // Adjust to nearest whole day.

        // Calculate intermediate values for the Gregorian date.
        int A = Z + 32044;
        int B = (4 * A + 3) / 146097;
        int C = A - (B * 146097) / 4;
        int D = (4 * C + 3) / 1461;
        int E = C - (1461 * D) / 4;
        int M = (5 * E + 2) / 153;

        // Extract the day, month, and year from the calculations.
        day = E - (153 * M + 2) / 5 + 1;
        month = M + 3 - 12 * (M / 10);
        year = B * 100 + D - 4800 + (M / 10);
    }

    // Converts a Julian Day (JDN) to a Gregorian date with time components.
    void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day,
        hour_t& hour, minute_t& minute, second_t& second) {
        int Z = static_cast<int>(std::floor(jd + 0.5));  // Adjust to nearest whole day.
        double fractional_day = jd + 0.5 - Z;  // Extract the fractional part of the day.

        // Convert the date part using the simpler conversion function.
        jd_to_gregorian(Z, year, month, day);

        // Convert the fractional day into total seconds.
        int total_seconds = static_cast<int>(std::round(fractional_day * 86400.0));

        // Ensure the total seconds do not exceed the maximum for a day.
        total_seconds = std::min(total_seconds, 86399);

        // Extract hours, minutes, and seconds from the total seconds.
        hour = total_seconds / 3600;
        minute = (total_seconds % 3600) / 60;
        second = total_seconds % 60;
    }

}  // end of namespace khronos