#pragma once

/**	@file khronos/julian_calendar.hpp
    @author Garth Santor
    @date 2013-09-19

    Julian calendar functions and class declaration.
    */

#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include<khronos/gregorian_calendar.hpp>



namespace khronos {

    // Julian calendar epoch
    constexpr jd_t JULIAN_EPOCH = 1721423.5;

    // Checks if the given Julian year is a leap year.
    constexpr bool is_julian_leapyear(year_t year) {
        return (year % 4) == 0;
    }

    // Returns the number of days in a specific Julian month, accounting for leap years.
    constexpr day_t julian_days_in_month(month_t month, bool isLeapYear) {
        constexpr std::array<day_t, 12> days_in_month = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        if (month == 2 && isLeapYear) {
            return 29;  
        }
        return days_in_month[month - 1];  // Return the days for the given month.
    }

    // Retrieves the full name of the given month.
    constexpr const char* julian_month_name(month_t month) {
        return civil::month_name_long(month);
    }

    // Retrieves the short name of the given month.
    constexpr const char* julian_short_month_name(month_t month) {
        return civil::month_name_short(month);
    }

    // Converts a BCE year to an astronomical year.
    constexpr year_t operator"" _BC(unsigned long long year) {
        return -(static_cast<year_t>(year) - 1);
    }

    // Converts a BCE year to its astronomical representation.
    constexpr year_t bce_to_astronomical(year_t year) {
        return -(year - 1);
    }

    // Function prototypes for conversions between Julian and Julian Day.
    jd_t julian_to_jd(year_t year, month_t month, day_t day);
    jd_t julian_to_jd(year_t year, month_t month, day_t day,
        hour_t hour, minute_t minute, second_t second);
    void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day);
    void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day,
        hour_t& hour, minute_t& minute, second_t& second);

    // Class representing a Julian calendar date.
    class Julian {
    public:
        year_t year_;     
        month_t month_;   
        day_t day_;       
        hour_t hour_ = 0; 
        minute_t minute_ = 0; 
        second_t second_ = 0; 

        // Constructor for a date without time.
        constexpr Julian(year_t year, month_t month, day_t day)
            : year_(year), month_(month), day_(day) {}

        // Constructor for a date with time components.
        constexpr Julian(year_t year, month_t month, day_t day,
            hour_t hour, minute_t minute, second_t second)
            : year_(year), month_(month), day_(day),
            hour_(hour), minute_(minute), second_(second) {}

        // Getters for date and time components.
        constexpr year_t year() const { return year_; }
        constexpr month_t month() const { return month_; }
        constexpr day_t day() const { return day_; }
        constexpr hour_t hour() const { return hour_; }
        constexpr minute_t minute() const { return minute_; }
        constexpr second_t second() const { return second_; }

        // Constructors for various initialization options.
        Julian();  // Default constructor.
        Julian(now_t);  // Initializes with current time.
        Julian(Jd jd);  // Initializes from a Julian Day.

        // Conversion to Julian Day.
        constexpr operator Jd() const {
            return Jd(julian_to_jd(year_, month_, day_, hour_, minute_, second_));
        }

        // Assignment operator for a Julian Day.
        Julian& operator=(const Jd& jd);

        // Converts the date to a string.
        std::string to_string() const;

        // Static function to check if a year is a leap year.
        static constexpr bool leap_year(year_t year) {
            return is_julian_leapyear(year);
        }

        // Adds months to the date, adjusting for overflow.
        Julian constexpr operator+(const months& m) const {
            year_t new_year = year_;
            month_t new_month = month_ + m.value;

            if (new_month > 12) {
                new_year += (new_month - 1) / 12;
                new_month = (new_month - 1) % 12 + 1;
            }

            if (new_month < 1) {
                new_year -= (-new_month) / 12 + 1;
                new_month = 12 + (new_month % 12);
            }

            day_t new_day = std::min(day_, julian_days_in_month(new_month, is_julian_leapyear(new_year)));
            return Julian(new_year, new_month, new_day, hour_, minute_, second_);
        }

        // Subtracts months from the date, adjusting for overflow.
        Julian constexpr operator-(const months& m) const {
            year_t new_year = year_;
            month_t new_month = month_ - m.value;

            if (new_month < 1) {
                new_year -= (-new_month) / 12 + 1;
                new_month = 12 + (new_month % 12);
            }

            if (new_month > 12) {
                new_year += (new_month - 1) / 12;
                new_month = (new_month - 1) % 12 + 1;
            }

            day_t new_day = std::min(day_, julian_days_in_month(new_month, is_julian_leapyear(new_year)));
            return Julian(new_year, new_month, new_day, hour_, minute_, second_);
        }
    };

    // Getter functions for Julian date components.
    constexpr year_t year(const Julian& g) { return g.year(); }
    constexpr month_t month(const Julian& g) { return g.month(); }
    constexpr day_t day(const Julian& g) { return g.day(); }
    constexpr hour_t hour(const Julian& g) { return g.hour(); }
    constexpr minute_t minute(const Julian& g) { return g.minute(); }
    constexpr second_t second(const Julian& g) { return g.second(); }

}  // namespace khronos