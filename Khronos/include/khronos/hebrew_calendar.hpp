#pragma once

/**	@file khronos/hebrew_calendar.hpp
    @author Garth Santor
    @date 2013-09-26

    Hebrew calendar class.
    */



#include <iostream>
#include <cmath>
#include <string>
#include "khronos/def.hpp"  
#include <khronos/julian_day.hpp>
#include<khronos/gregorian_calendar.hpp>
namespace khronos {

    // The epoch for the Hebrew calendar 
    constexpr double HEBREW_EPOCH = 347'995.5;

    // Enumeration for the months of the Hebrew calendar.
    enum HebrewMonth {
        Nisan = 1, Iyyar, Sivan, Tammuz, Av, Elul,
        Tishri, Heshvan, Kislev, Teveth, Shevat, Adar, Veadar
    };

    // Determines if the given Hebrew year is a leap year.
    constexpr bool is_hebrew_leapyear(year_t year) {
        return (year * 7 + 1) % 19 < 7;
    }

    // Returns the number of months in the given Hebrew year.
    constexpr month_t hebrew_months_in_year(year_t year) {
        return is_hebrew_leapyear(year) ? 13 : 12;
    }

    // Retrieves the name of the given Hebrew month as a string.
    inline std::string hebrew_month_name(HebrewMonth month) {
        static const std::string names[] = {
            "", "Nisan", "Iyyar", "Sivan", "Tammuz",
            "Av", "Elul", "Tishri", "Heshvan",
            "Kislev", "Teveth", "Shevat", "Adar", "Veadar"
        };
        return (month >= Nisan && month <= Veadar) ? names[month] : "Invalid month";
    }

    // Converts a generic month_t to a Hebrew month name.
    inline std::string hebrew_month_name(month_t month) {
        return hebrew_month_name(static_cast<HebrewMonth>(month));
    }

    // Function prototypes for Hebrew calendar calculations.
    double hebrew_delay_of_week(year_t year);
    double hebrew_delay_adjacent_year(year_t year);
    double hebrew_days_in_year(year_t year);
    day_t hebrew_days_in_month(year_t year, month_t month);

    // Converts a Hebrew date to Julian Day Number (JDN).
    double hebrew_to_jd(year_t year, month_t month, day_t day);
    double hebrew_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second);

    // Converts a Julian Day Number (JDN) to a Hebrew date.
    void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day);
    void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& second);

    // Class representing a Hebrew calendar date.
    class Hebrew {
        year_t year_;     
        month_t month_;   
        day_t day_;       
        hour_t hour_;     
        minute_t minute_; 
        second_t second_; 

        // Compares two Hebrew dates for equality.
        bool operator==(const Hebrew& other) const {
            return (year_ == other.year_) &&
                (month_ == other.month_) &&
                (day_ == other.day_);
        }

        // Adds a number of days to the Hebrew date.
        Hebrew operator+(day_t days) const {
            Hebrew result = *this;
            result.day_ += days;
            return result;
        }

        // Subtracts a number of days from the Hebrew date.
        Hebrew operator-(day_t days) const {
            Hebrew result = *this;
            result.day_ -= days;
            return result;
        }

    public:
        // Constructor for a Hebrew date with optional time components.
        Hebrew(year_t year, month_t month, day_t day, hour_t hour = 0, minute_t minute = 0, second_t second = 0);

        Hebrew(); // Default constructor.

        std::string to_string() const; // Converts the date to a string.
        double to_jd() const;          // Converts the date to a Julian Day Number.
        void from_jd(double jd);       // Initializes the date from a Julian Day Number.
    };

    // Overload of << operator to print a Hebrew date.
    inline std::ostream& operator << (std::ostream& os, Hebrew const& g) {
        return os << g.to_string();
    }

} // end of namespace khronos
