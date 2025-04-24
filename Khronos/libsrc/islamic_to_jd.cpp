/**	@file Islamic_to_jd.cpp
	@author Garth Santor
	@date 2013-09-19

	Islamic calendar conversion implementations.
	*/

#include <khronos/islamic_calendar.hpp>


namespace khronos {

    // Converts an Islamic date to Julian Day (JDN).
    jd_t islamic_to_jd(year_t year, month_t month, day_t day) {
        return day + ceil(29.5 * (month - 1)) +
            (year - 1) * 354.0 +
            floor((3 + 11.0 * year) / 30.0) +
            ISLAMIC_EPOCH - 1;
    }

    // Converts an Islamic date with time components to Julian Day (JDN).
    jd_t islamic_to_jd(year_t year, month_t month, day_t day,
        hour_t hour, minute_t minute, second_t second) {
        double tod_ = tod(hour, minute, second);  // Calculate time fraction.
        if (tod_ >= 0.5)
            tod_ -= 1;  // Adjust if time fraction exceeds half a day.

        return islamic_to_jd(year, month, day) + tod_;  // Return the JDN.
    }

    // Conversion operator from Islamic date to Julian Day (JDN).
    Islamic::operator Jd() const {
        return Jd(islamic_to_jd(year_, month_, day_, hour_, minute_, second_));
    }

}  // end of namespace khronos
