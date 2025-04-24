/**	@file Julian_to_jd.cpp
    @author Garth Santor
    @date 2012-09-14

    Date and time library Julian conversion implementations.
    */

#include <khronos/julian_calendar.hpp>




namespace khronos {

    // Constructor for Julian date from a Julian Day (Jd).
    Julian::Julian(Jd jd) {
        // Convert Jd to Julian date components.
        jd_to_julian(jd.value(), year_, month_, day_, hour_, minute_, second_);

        // Adjust for negative years to follow historical convention (1 BCE = year 0).
        if (year_ <= 0) {
            year_ = -(year_ - 1);
        }
    }

    // Converts a Julian date (year, month, day) to Julian Day (JDN).
    jd_t julian_to_jd(year_t year, month_t month, day_t day) {
        long long a = (14 - month) / 12;  
        long long y = year + 4800 - a;  
        long long m = month + 12 * a - 3;  

        // Calculate the Julian Day Number.
        return day + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083.5;
    }

    // Converts a Julian date with time components to Julian Day (JDN).
    jd_t julian_to_jd(year_t year, month_t month, day_t day,
        hour_t hour, minute_t minute, second_t second) {
        // Add the fractional day from time of day to the JDN.
        return julian_to_jd(year, month, day) + tod(hour, minute, second);
    }

    // Converts a Julian Day (JDN) to a Julian date (year, month, day).
    void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day) {
        double a = floor(jd + 0.5);  
        double b = a + 1524;  
        double c = floor((b - 122.1) / 365.25);  
        double d = floor(365.25 * c);  
        double e = floor((b - d) / 30.6001);  

        // Determine the month, adjusting for January and February.
        month = (month_t)floor(e < 14 ? e - 1 : e - 13);

        // Calculate the year, adjusting for months earlier than March.
        year = (year_t)floor(month > 2 ? c - 4716 : c - 4715);

        // Calculate the day of the month.
        day = (day_t)(b - d - floor(30.6001 * e));
    }

    // Converts a Julian Day (JDN) to a Julian date with time components.
    void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day,
        hour_t& hour, minute_t& minute, second_t& second) {
        // Convert the date part first.
        jd_to_julian(jd, year, month, day);

        // Extract the fractional part of the day.
        double tod = (jd + 0.5) - floor(jd + 0.5);
        int secondsInDay = static_cast<int>(floor(tod * SECONDS_PER_DAY + 0.5));

        // Extract hours, minutes, and seconds from the total seconds.
        hour = secondsInDay / 3600;
        minute = (secondsInDay / 60) % 60;
        second = secondsInDay % 60;
    }

}  // end of namespace khronos