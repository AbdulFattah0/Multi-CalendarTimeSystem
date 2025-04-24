#pragma once
/*
khronos\gregorian_calendar.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'Gregorian calendar' declarations.
*/



#pragma once
#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>
#include <ctime>


enum Month {
    MONTH_JAN = 1, MONTH_FEB, MONTH_MAR, MONTH_APR, MONTH_MAY,
    MONTH_JUN, MONTH_JUL, MONTH_AUG, MONTH_SEP, MONTH_OCT,
    MONTH_NOV, MONTH_DEC
};

enum DayOfWeek { SUN = 0, MON, TUE, WED, THU, FRI, SAT };

namespace khronos {

    /** Provide the number of days in the month of the Gregorian Calendar. */
    constexpr day_t gregorian_days_in_month(month_t month, bool isLeapYear) {
        return civil::days_in_month(month, isLeapYear);
    }

    /** Provide the name of the given month in the Gregorian calendar. */
    constexpr char const* gregorian_month_name(month_t month) {
        return civil::month_name_long(month);
    }

    /** Provide the short name of the given month in the Gregorian calendar. */
    constexpr char const* gregorian_short_month_name(month_t month) {
        return civil::month_name_short(month);
    }



    

    // --------------------------------------------------------------------------------------
    // CLASSES
    // --------------------------------------------------------------------------------------



    // Class representing a Gregorian calendar date.
    class Gregorian {
    public:
        year_t year_;   
        Month month_;
        day_t day_;
        hour_t hour_{ 0 };
        minute_t minute_{ 0 };
        second_t second_{ 0.0 };

        // Constructor: Initializes date without time.
        constexpr Gregorian(year_t y, month_t m, day_t d)
            : year_(y), month_(static_cast<Month>(m)), day_(d) {}

        // Constructor: Initializes date with time components.
        constexpr Gregorian(year_t y, month_t m, day_t d,
            hour_t h, minute_t min, second_t sec)
            : year_(y), month_(static_cast<Month>(m)), day_(d),
            hour_(h), minute_(min), second_(sec) {}

        std::string to_string() const; // Converts date to a string.

        Gregorian(); // Default constructor.
        Gregorian(Jd jd); // Constructor from Julian Day.
        operator Jd() const; // Conversion to Julian Day.

        // Initializes the current date, optionally including time of day.
        explicit Gregorian(now_t option) {
            time_t utcTime = time(NULL);
            struct tm tmLocal;
            localtime_s(&tmLocal, &utcTime);

            year_ = year_t(tmLocal.tm_year) + 1900;
            month_ = static_cast<Month>(tmLocal.tm_mon + 1);
            day_ = day_t(tmLocal.tm_mday);

            if (option == WTIMEOFDAY) {
                hour_ = hour_t(tmLocal.tm_hour);
                minute_ = minute_t(tmLocal.tm_min);
                second_ = second_t(tmLocal.tm_sec);
            }
        }

        


        // Getter functions
        constexpr year_t year() const { return year_; }
        constexpr Month month() const { return month_; }
        constexpr day_t day() const { return day_; }
        constexpr hour_t hour() const { return hour_; }
        constexpr minute_t minute() const { return minute_; }
        constexpr second_t second() const { return second_; }


    };




    // Checks if a given year is a leap year.
    constexpr bool is_gregorian_leapyear(year_t year) {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }

    // Literal for specifying a Common Era (CE) year.
    constexpr year_t operator"" _CE(unsigned long long year) {
        return (year);
    }

    // Literal for specifying a Before Common Era (BCE) year.
    constexpr year_t operator"" _BCE(unsigned long long year) {
        return year_t(-(static_cast<long long>(year) - 1));
    }



    // Computes the day of the week for a given Gregorian date.
    constexpr DayOfWeek day_of_week(const Gregorian& date) {
        auto y = date.year();
        auto m = static_cast<int>(date.month());
        auto d = date.day();

        if (m < 3) {
            m += 12;
            y -= 1;
        }

        if (y < 0) {
            y += 1;
        }

        auto k = y % 100;   // Year within the century.
        auto j = y / 100;    // Zero-based century.
        auto dow = (d + (13 * (m + 1)) / 5 + k + (k / 4) + (j / 4) - (2 * j)) % 7;
        dow = (dow + 5) % 7;

        return static_cast<DayOfWeek>(dow);
    }


    // Computes the day of the week from a Julian Day.
    constexpr DayOfWeek day_of_week(jd_t jd) {
     
        auto jdn = static_cast<int>(jd + 0.5);

        auto dow = (jdn + 1) % 7;

        return static_cast<DayOfWeek>((dow + 6) % 7);
    }
  

    
    constexpr unsigned gregorian_days_in_month(unsigned month, bool isLeapYear) {
        constexpr int daysOfMonth[2][13] = {
            { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
            { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
        };
        return daysOfMonth[isLeapYear][month];
    }

    // Adds a number of months to a given Gregorian date.
    constexpr Gregorian add_months(Gregorian const& date, int n) {
        auto total_month = static_cast<int>(date.month()) + n;  // Calculate total months.
        auto new_year = date.year() + (total_month - 1) / 12;   // Adjust the year.
        auto new_month = (total_month - 1) % 12 + 1;            // Get the new month.

        if (new_month < 1) {  // Handle negative month values.
            new_month += 12;
            new_year -= 1;
        }

        auto day = date.day();  // Get the original day.
        auto max_days = gregorian_days_in_month(new_month, is_gregorian_leapyear(new_year));

        if (day > max_days) {  // Adjust the day if it exceeds the new month’s days.
            day = max_days;
        }

        // Return the updated Gregorian date with the adjusted values.
        return Gregorian(
            new_year,
            static_cast<Month>(new_month),
            day,
            date.hour(),
            date.minute(),
            date.second()
        );
    }

    // Overloads the + operator to add months to a date.
    constexpr Gregorian operator+(const Gregorian& date, const months& m) {
        return add_months(date, m.value);
    }

    // Overloads the - operator to subtract months from a date.
    constexpr Gregorian operator-(const Gregorian& date, const months& m) {
        return add_months(date, -month_t(m.value));
    }

    // Adds a number of days to a given Gregorian date.
    constexpr Gregorian add_days(Gregorian const& date, unsigned n) {
        auto y = date.year();
        unsigned m = date.month();
        auto d = date.day() + n;

        auto dom = gregorian_days_in_month(m, is_gregorian_leapyear(y));
        while (d > static_cast<int>(dom)) {
            d -= dom;
            if (++m > 12) {
                ++y;
                m = 1;
            }
            dom = gregorian_days_in_month(m, is_gregorian_leapyear(y));
        }

        return Gregorian(y, Month(m), d);
    }

    // Adds years to a Gregorian date, adjusting for leap years.
    constexpr Gregorian addYears(const Gregorian& date, int n) {
        auto new_year = date.year() + n;  // Calculate the new year.
        Month new_month = date.month();   // Keep the same month.
        auto new_day = date.day();        // Keep the same day.

        // Handle leap year adjustment for February 29.
        if (new_month == MONTH_FEB && new_day == 29 && !is_gregorian_leapyear(new_year)) {
            new_day = 28;
        }

        // Ensure the day is valid for the new month and year.
        auto max_days = gregorian_days_in_month(new_month, is_gregorian_leapyear(new_year));
        if (new_day > max_days) {
            new_day = max_days;
        }

        // Return the new Gregorian date with the adjusted values.
        return Gregorian(new_year, new_month, new_day, date.hour(), date.minute(), date.second());
    }


    

    constexpr year_t year(const Gregorian& g) { return g.year(); }
    constexpr Month month(const Gregorian& g) { return g.month(); }
    constexpr day_t day(const Gregorian& g) { return g.day(); }
    constexpr hour_t hour(const Gregorian& g) { return g.hour(); }
    constexpr minute_t minute(const Gregorian& g) { return g.minute(); }
    constexpr second_t second(const Gregorian& g) { return g.second(); }

    
    jd_t gregorian_to_jd(year_t year, month_t month, day_t day);
    jd_t gregorian_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second);
    void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day);
    void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& second);
    jd_t jd_to_jdn(jd_t jd);

    // Overloads the - operator to compute the difference in days between two dates.
    inline int operator-(const Gregorian& lhs, const Gregorian& rhs) {
        jd_t lhs_jd = gregorian_to_jd(lhs.year(), lhs.month(), lhs.day());
        jd_t rhs_jd = gregorian_to_jd(rhs.year(), rhs.month(), rhs.day());
        return static_cast<int>(lhs_jd - rhs_jd);
    }

   
    




}  // namespace khronos