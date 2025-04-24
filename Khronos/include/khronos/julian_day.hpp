#pragma once
/*	julian_day.hpp

    Khronos library Julian Day class declaration.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.09.25
    Repaced relational operators with <=>

Version 2016.09.30
    Added: Phase List

Version 2015.09.21
    Initial Release


=============================================================

Copyright Garth Santor/Trinh Han

The copyright to the computer program(s) herein
is the property of Garth Santor/Trinh Han, Canada.
The program(s) may be used and/or copied only with
the written permission of Garth Santor/Trinh Han
or in accordance with the terms and conditions
stipulated in the agreement/contract under which
the program(s) have been supplied.
=============================================================*/


#include <khronos/def.hpp>  
#include <khronos/timeofday.hpp>
#include <khronos/utility.hpp>

#include <iostream>
#include <string>
#include <ctime>
#include <cmath>


namespace khronos {

    using jd_t = double;  // Alias for Julian Day type.
    constexpr jd_t GREGORIAN_EPOCH = 1721425.5;  // Epoch for the Gregorian calendar.

    // Structure representing a number of days.
    struct days {
        jd_t value;  // Number of days.
        constexpr explicit days(jd_t d) : value(d) {}
    };

    // Structure representing a number of weeks, converted to days.
    struct weeks {
        jd_t value;  // Number of days in weeks.
        constexpr explicit weeks(jd_t w) : value(w * 7) {}
    };

    // Structure representing a number of months.
    struct months {
        month_t value;  // Number of months.
        constexpr explicit months(month_t m) : value(m) {}
    };

    // Structure representing a number of years, converted to days.
    struct years {
        jd_t value;  // Number of days in years.
        constexpr explicit years(jd_t y) : value(y* EARTH_ORBITAL_PERIOD_DAYS) {}
    };

    // Class representing a Julian Day (Jd).
    class Jd {
    private:
        jd_t jd_;  // Julian Day value.

    public:
        Jd();  // Default constructor.

        // Constructor initializing the Julian Day.
        constexpr Jd(jd_t jd) : jd_(jd) {}

        // Returns the Julian Day value.
        constexpr jd_t jd() const { return jd_; }

        std::string to_string() const;  // Converts the Julian Day to a string.

        Jd(now_t n);  // Constructor to initialize with current time.

        // Returns the Julian Day value.
        constexpr jd_t value() const { return jd_; }
    };

    // Overload of << operator to print a Julian Day.
    std::ostream& operator << (std::ostream& os, const Jd& jd);

    // Three-way comparison operator for Julian Days.
    constexpr auto operator <=> (const Jd& lhs, const Jd& rhs) { return lhs.jd() <=> rhs.jd(); }

    // Equality operator for Julian Days.
    constexpr bool operator == (const Jd& lhs, const Jd& rhs) { return lhs.jd() == rhs.jd(); }

    // Subtract two Julian Days to get the difference in days.
    constexpr jd_t operator - (const Jd& lhs, const Jd& rhs) { return lhs.jd() - rhs.jd(); }

    // Add days to a Julian Day.
    constexpr Jd operator+(const Jd& jd, const days& d) {
        return Jd(jd.jd() + d.value);
    }

    // Subtract days from a Julian Day.
    constexpr Jd operator-(const Jd& jd, const days& d) {
        return Jd(jd.jd() - d.value);
    }

    // Add days to a Julian Day (commutative).
    constexpr Jd operator+(const days& d, const Jd& jd) {
        return Jd(jd.jd() + d.value);
    }

    // Add weeks to a Julian Day.
    constexpr Jd operator+(const Jd& jd, const weeks& w) {
        return Jd(jd.jd() + w.value);
    }

    // Subtract weeks from a Julian Day.
    constexpr Jd operator-(const Jd& jd, const weeks& w) {
        return Jd(jd.jd() - w.value);
    }

    // Add weeks to a Julian Day (commutative).
    constexpr Jd operator+(const weeks& w, const Jd& jd) {
        return Jd(jd.jd() + w.value);
    }

    // Add years to a Julian Day.
    constexpr Jd operator+(const Jd& jd, const years& y) {
        return Jd(jd.jd() + y.value);
    }

    // Subtract years from a Julian Day.
    constexpr Jd operator-(const Jd& jd, const years& y) {
        return Jd(jd.jd() - y.value);
    }

    // Add years to a Julian Day (commutative).
    constexpr Jd operator+(const years& y, const Jd& jd) {
        return Jd(jd.jd() + y.value);
    }

} // end-of-namespace khronos