/**	@file Hebrew.cpp
    @author Garth Santor
    @date 2013-10-20

    Hebrew calendar class implementation.
    */

#include <khronos/hebrew_calendar.hpp>
#include <sstream>
#include <iomanip>

namespace khronos {

    // Default constructor for Hebrew date, initializes to 1st Tishri 5784.
    Hebrew::Hebrew() {
        from_jd(hebrew_to_jd(5784, 7, 1));  // 7th month is Tishri.
    }

    // Constructor to initialize Hebrew date with all components.
    Hebrew::Hebrew(year_t year, month_t month, day_t day,
        hour_t hour, minute_t minute, second_t second)
        : year_(year), month_(month), day_(day),
        hour_(hour), minute_(minute), second_(second) {}

    // Calculates the delay in days at the start of the Hebrew year.
    double hebrew_delay_of_week(year_t year) {
        double months = std::floor((235.0 * year - 234.0) / 19.0);  // Moon cycles.
        int parts = 12084 + 13753 * static_cast<int>(months);  // Parts calculation.
        double days = months * 29 + std::floor(parts / 25920.0);  // Total days.

        if ((3 * static_cast<int>(days + 1)) % 7 < 3)  // Adjust if required.
            days += 1;

        return days;
    }

    // Determines the delay between adjacent Hebrew years.
    double hebrew_delay_adjacent_year(year_t year) {
        double last = hebrew_delay_of_week(year - 1);
        double present = hebrew_delay_of_week(year);
        double next = hebrew_delay_of_week(year + 1);

        if (next - present == 356) return 2.0;  // Long year adjustment.
        if (present - last == 382) return 1.0;  // Short year adjustment.
        return 0.0;
    }

    // Calculates the number of days in a given Hebrew year.
    double hebrew_days_in_year(year_t year) {
        return hebrew_to_jd(year + 1, 7, 1) - hebrew_to_jd(year, 7, 1);  // From Tishri to Tishri.
    }

    // Determines the number of days in a given Hebrew month.
    day_t hebrew_days_in_month(year_t year, month_t month) {
        const int days_in_year = static_cast<int>(hebrew_days_in_year(year));  // Total days in the year.

        // Specific month rules for determining days.
        if (month == 2 || month == 4 || month == 6 || month == 10 || month == 13) return 29;
        if (month == 12 && !is_hebrew_leapyear(year)) return 29;
        if (month == 8 && days_in_year % 10 != 5) return 29;
        if (month == 9 && days_in_year % 10 == 3) return 29;
        return 30;
    }

    // Converts the Hebrew date to a string representation.
    std::string Hebrew::to_string() const {
        std::ostringstream oss;
        oss << hebrew_month_name(month_) << ' ' << (unsigned)day_ << ' ' << year_;

        bool isAm = hour_ < 12;  // Determine if the time is AM or PM.
        int h = (int)hour_;
        if (!isAm) h -= 12;
        if (h == 0) h = 12;  // Handle 12-hour formatting.

        char buff[100];
        sprintf_s(buff, 99, ", %d:%02d:%02.0f %s", h, minute_, second_, isAm ? "am" : "pm");
        oss << buff;

        return oss.str();  // Return the formatted string.
    }

    // Converts the Hebrew date to Julian Day (JDN).
    double Hebrew::to_jd() const {
        return hebrew_to_jd(year_, month_, day_, hour_, minute_, second_);
    }

    // Initializes the Hebrew date from a given Julian Day (JDN).
    void Hebrew::from_jd(double jd) {
        jd_to_hebrew(jd, year_, month_, day_, hour_, minute_, second_);
    }

}  // end of namespace khronos