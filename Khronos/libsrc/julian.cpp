/**	@file Julian.cpp
    @author Garth Santor
    @date 2013-09-19

    Julian calendar class implementation.
    */

#include <khronos/julian_calendar.hpp>

namespace khronos {

    // Converts a Gregorian date to a Julian date.
    Julian julian_from_gregorian(const Gregorian& gregorian) {
        // Convert Gregorian date to Julian Day (JDN).
        jd_t jdn = gregorian_to_jd(
            gregorian.year(), gregorian.month(), gregorian.day(),
            gregorian.hour(), gregorian.minute(), gregorian.second()
        );

        // Convert JDN to Julian date components.
        year_t year;
        month_t month;
        day_t day;
        hour_t hour;
        minute_t minute;
        second_t second;

        jd_to_julian(jdn, year, month, day, hour, minute, second);

        // Return the corresponding Julian date.
        return Julian(year, month, day, hour, minute, second);
    }

    // Retrieves the current Gregorian date, optionally including time.
    Gregorian current_gregorian_date(bool include_time = true) {
        std::time_t utcTime = std::time(nullptr);  // Get the current time.
        std::tm tmLocal;
        localtime_s(&tmLocal, &utcTime);  // Convert to local time.

        if (include_time) {
            // Return a Gregorian date with time.
            return Gregorian(
                year_t(tmLocal.tm_year) + 1900, month_t(tmLocal.tm_mon) + 1,
                day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour),
                minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)
            );
        }
        else {
            // Return a Gregorian date without time.
            return Gregorian(
                year_t(tmLocal.tm_year) + 1900, month_t(tmLocal.tm_mon) + 1,
                day_t(tmLocal.tm_mday)
            );
        }
    }

    // Converts the Julian date to a string representation.
    std::string Julian::to_string() const {
        return std::string(julian_month_name(month_)) + " " +
            std::to_string(day_) + ", " + std::to_string(year_);
    }

    // Default constructor for Julian, initializes to the current date.
    Julian::Julian() {
        // Initialize the Julian date from the current Gregorian date.
        *this = julian_from_gregorian(current_gregorian_date());
    }

    // Constructor for Julian with the option to exclude the time of day.
    Julian::Julian(now_t) {
        // Get the current Gregorian date without time.
        Gregorian gregorianDate = current_gregorian_date(false);

        // Convert Gregorian date to Julian Day (JDN).
        jd_t jdn = gregorian_to_jd(
            gregorianDate.year(),
            gregorianDate.month(),
            gregorianDate.day()
        );

        // Convert JDN to Julian date components.
        jd_to_julian(jdn, year_, month_, day_);

        // Set the time components to 0.
        hour_ = static_cast<hour_t>(0);
        minute_ = static_cast<minute_t>(0);
        second_ = static_cast<second_t>(0);
    }

    // Assignment operator for Julian, assigns from a Julian Day (Jd) object.
    Julian& Julian::operator=(const Jd& jd) {
        // Convert Jd value to Julian date components.
        jd_to_julian(jd.value(), year_, month_, day_, hour_, minute_, second_);
        return *this;
    }

}  // namespace khronos