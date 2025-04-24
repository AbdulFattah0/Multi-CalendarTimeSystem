#pragma once

/**	@file khronos/Islamic_calendar.hpp
    @author Garth Santor
    @date 2013-09-19

    Islamic calendar class and functions.
    */
#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/julian_day.hpp>
#include <string>


namespace khronos {

    // Enumeration for the months of the Islamic calendar.
    enum IslamicMonth {
        Muharram = 1, SAFAR, RABI_AL_AWWAL, RabiathThani,
        JUMADA_AL_AWWAL, JumadatTania, RAJAB, Shaban, Ramadan,
        Shawwal, DhulQadah, DhulHijja
    };

    // The epoch for the Islamic calendar (Julian Day Number for Islamic year 1).
    constexpr jd_t ISLAMIC_EPOCH = 1948439.5;

    // Function prototypes for Islamic calendar operations.
    std::string islamic_month_name(month_t month);
    jd_t islamic_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second);
    jd_t islamic_to_jd(year_t year, month_t month, day_t day);
    void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day);
    void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& second);

    // Class representing an Islamic calendar date.
    class Islamic {
    public:
        year_t year_;     
        month_t month_;   
        day_t day_;       
        hour_t hour_{ 0 }; 
        minute_t minute_{ 0 }; 
        second_t second_{ 0.0 }; 

        Islamic(); // Default constructor.
        Islamic(now_t option); // Constructor with current time option.

        // Constructor with year, month, day, and optional time.
        Islamic(year_t y, month_t m, day_t d, hour_t h = 0, minute_t mi = 0, second_t s = 0)
            : year_(y), month_(m), day_(d), hour_(h), minute_(mi), second_(s) {}

        // Equality operator to compare two Islamic dates.
        constexpr bool operator==(const Islamic& other) const {
            return (year_ == other.year_ && month_ == other.month_ && day_ == other.day_ &&
                hour_ == other.hour_ && minute_ == other.minute_ && second_ == other.second_);
        }

        // Converts the Islamic date to a Julian Day Number (JDN).
        jd_t to_jd() const {
            return islamic_to_jd(year_, month_, day_, hour_, minute_, second_);
        }

        // Getter functions for each date and time component.
        constexpr year_t year() const { return year_; }
        constexpr month_t month() const { return month_; }
        constexpr day_t day() const { return day_; }
        constexpr hour_t hour() const { return hour_; }
        constexpr minute_t minute() const { return minute_; }
        constexpr second_t second() const { return second_; }

        // Conversion operator to Julian Day.
        operator Jd() const;
    };

    // Determines if the given Islamic year is a leap year.
    constexpr bool is_islamic_leapyear(year_t year) {
        return ((11 * year + 14) % 30) < 11;
    }

    // Returns the number of days in the given Islamic month.
    constexpr unsigned islamic_days_in_month(month_t month_, bool isLeapYear) {
        if (month_ == DhulQadah) {
            return 30; // DhulQadah always has 30 days.
        }
        if (month_ == DhulHijja) {
            return isLeapYear ? 30 : 29; // DhulHijja varies based on leap year.
        }
        return (month_ % 2 == 1) ? 30 : 29; // Other months alternate between 29 and 30 days.
    }

    // Function prototypes for adding months or days to an Islamic date.
    Islamic add_months(Islamic const& date, int n);
    Islamic add_days(Islamic const& date, int n);

    // Comparison operators between Islamic and Gregorian dates.
    bool operator==(const Islamic& islamic, const Gregorian& gregorian);
    bool operator==(const Gregorian& gregorian, const Islamic& islamic);

} // end of namespace khronos
