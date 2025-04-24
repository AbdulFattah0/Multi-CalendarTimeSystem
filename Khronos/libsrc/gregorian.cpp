/**	@file	gregorian.cpp
    @author	Garth Santor
    @date	2013-09-19

    Gregorian calendar class implementation.
    */

#include <khronos/gregorian_calendar.hpp>
#include <cmath>
#include<ostream>


#include <sstream>     
#include <iomanip>      
#include <cmath>       
#include <ctime> 
using namespace khronos::civil;
namespace khronos {

    // Formats the hour in 12-hour format (1-12).
    std::string format_hour(hour_t hour) {
        int h = hour % 12;
        if (h == 0) h = 12;  // Handle midnight and noon as 12.
        return std::to_string(h);
    }

    // Returns "am" or "pm" based on the given hour.
    const char* am_pm(hour_t hour) {
        return hour < 12 ? "am" : "pm";
    }

    // Returns the name of the day for the given day of the week.
    const char* day_name(DayOfWeek dow) {
        switch (dow) {
        case SUN: return "Sunday";
        case MON: return "Monday";
        case TUE: return "Tuesday";
        case WED: return "Wednesday";
        case THU: return "Thursday";
        case FRI: return "Friday";
        case SAT: return "Saturday";
        default: return "";
        }
    }

    // Converts the given time components into a fractional day value.
    double tod(hour_t hour, minute_t minute, second_t second) {
        return (hour * 3600 + minute * 60 + second) / (24.0 * 60 * 60);
    }

    // Default constructor for Gregorian date, initializes to the current local time.
    Gregorian::Gregorian() {
        time_t utcTime = time(NULL);  // Get the current UTC time.
        struct tm tmLocal;
        localtime_s(&tmLocal, &utcTime);  // Convert to local time.

        year_ = year_t(tmLocal.tm_year) + 1900;
        month_ = static_cast<Month>(tmLocal.tm_mon + 1);
        day_ = day_t(tmLocal.tm_mday);
        hour_ = hour_t(tmLocal.tm_hour);
        minute_ = minute_t(tmLocal.tm_min);
        second_ = second_t(tmLocal.tm_sec);
    }

    // Constructor to initialize Gregorian date from a Julian Day.
    Gregorian::Gregorian(Jd jd) {
        year_t year;
        month_t month;
        day_t day;
        hour_t hour = 0;
        minute_t minute = 0;
        second_t second = 0;

        // Convert Julian Day to Gregorian date components.
        jd_to_gregorian(jd.jd(), year, month, day, hour, minute, second);

        year_ = year;
        month_ = static_cast<Month>(month);
        day_ = day;
        hour_ = hour;
        minute_ = minute;
        second_ = second;
    }

    // Conversion operator from Gregorian to Julian Day.
    Gregorian::operator Jd() const {
        return Jd(gregorian_to_jd(year_, month_, day_, hour_, minute_, second_));
    }

    // Converts the Gregorian date to a string in a readable format.
    std::string Gregorian::to_string() const {
        std::ostringstream oss;

        // Add day name, month name, and day of the month.
        oss << day_name(day_of_week(*this)) << ", "
            << month_name_long(month_) << " " << day() << " ";

        // Add the year with "CE" or "BCE" based on its value.
        if (year_ > 0) {
            oss << year_ << " CE";
        }
        else {
            oss << std::abs(year_ - 1) << " BCE";
        }

        // Format the hour in 12-hour format and add am/pm.
        int display_hour = (hour_ % 12 == 0) ? 12 : hour_ % 12;
        oss << ", " << display_hour << ":"
            << std::setw(2) << std::setfill('0') << minute_ << ":"
            << std::setw(2) << std::setfill('0') << static_cast<int>(second_)
            << (hour_ < 12 ? " am" : " pm");

        return oss.str();  // Return the formatted string.
    }

}  // end of namespace khronos