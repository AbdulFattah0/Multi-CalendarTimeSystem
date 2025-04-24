/**	@file Hebrew_to_jd.cpp
    @author Garth Santor
    @date 2012-09-14

    Hebrew calendar conversion implementations.
    */

#include<khronos/islamic_calendar.hpp>
#include<khronos/hebrew_calendar.hpp>
#include <cmath>

namespace khronos {

    // Converts a Hebrew date to Julian Day (JDN).
    jd_t hebrew_to_jd(year_t year, month_t month, day_t day) {
        jd_t jdn = HEBREW_EPOCH + hebrew_delay_of_week(year) +
            hebrew_delay_adjacent_year(year) + day + 1;

        // Add days for months in the year based on whether the month is before or after Tishri.
        month_t m;
        if (month < 7) {  // Months before Tishri (7th month).
            for (m = 7; m <= hebrew_months_in_year(year); m++)
                jdn += hebrew_days_in_month(year, m);
            for (m = 1; m < month; m++)
                jdn += hebrew_days_in_month(year, m);
        }
        else {  // Months starting from Tishri.
            for (m = 7; m < month; m++)
                jdn += hebrew_days_in_month(year, m);
        }

        return jdn;  // Return the calculated Julian Day Number.
    }

    // Converts Julian Day (JDN) to a Hebrew date (year, month, day).
    void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day) {
        double a = std::round(jd + 0.5);  // Adjust the day fraction.

        // Estimate the Hebrew year.
        double count = std::floor(((a - HEBREW_EPOCH) * 98496.0) / 35975351.0);
        year = static_cast<year_t>(count - 1);

        // Find the correct year by comparing with the next year's start.
        while (a >= hebrew_to_jd(year + 1, 7, 1)) {
            ++year;
        }

        // Determine the first month (Tishri or Nisan).
        month_t first_month = (a < hebrew_to_jd(year, 1, 1)) ? 7 : 1;
        month = first_month;

        // Find the correct month within the year.
        while (a > hebrew_to_jd(year, month, hebrew_days_in_month(year, month))) {
            ++month;
        }

        // Calculate the day of the month.
        day = static_cast<day_t>(a - hebrew_to_jd(year, month, 1) + 1);
    }

    // Converts Julian Day (JDN) to a Hebrew date with time components.
    void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day,
        hour_t& hour, minute_t& minute, second_t& second) {
        jd_to_hebrew(jd, year, month, day);  // Convert the date part.

        // Extract the time of day from the fractional part of the JDN.
        double tod = jd + 0.5 - std::floor(jd + 0.5);
        int totalSeconds = static_cast<int>(std::round(tod * SECONDS_PER_DAY));

        // Calculate hours, minutes, and seconds.
        hour = totalSeconds / 3600;
        minute = (totalSeconds % 3600) / 60;
        second = totalSeconds % 60;
    }

    // Converts a Hebrew date with time components to Julian Day (JDN).
    jd_t hebrew_to_jd(year_t year, month_t month, day_t day,
        hour_t hour, minute_t minute, second_t second) {
        double tod_ = tod(hour, minute, second);  // Calculate time of day fraction.
        if (tod_ >= 0.5)
            tod_ -= 1;  // Adjust if the time fraction exceeds half a day.

        return hebrew_to_jd(year, month, day) + tod_;  // Return the final JDN.
    }

}  // end of namespace khronos
