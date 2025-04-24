/**	@file Islamic.cpp
    @author Garth Santor
    @date 2013-09-20

    Islamic calendar class implementation.
    */

#include <khronos/islamic_calendar.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <khronos/julian_day.hpp>
  

namespace khronos {

    // Adds 'n' months to the given Islamic date.
    Islamic add_months(Islamic const& date, int n) {
        month_t months = date.month_ + n;
        year_t years = date.year_ + (months - 1) / 12;
        months = (months - 1) % 12 + 1;

        // Adjust the day to fit within the new month.
        int days_in_new_month = islamic_days_in_month(months, is_islamic_leapyear(years));
        int new_day = std::min(date.day_, days_in_new_month);

        return Islamic(years, static_cast<month_t>(months), new_day);
    }

    // Adds 'n' days to the given Islamic date.
    Islamic add_days(Islamic const& date, int n) {
        year_t year = date.year_;
        month_t month = date.month_;
        day_t day = date.day_ + n;

        // Adjust the day, month, and year as needed.
        while (day > static_cast<int>(islamic_days_in_month(month, is_islamic_leapyear(year)))) {
            day -= islamic_days_in_month(month, is_islamic_leapyear(year));
            if (++month > 12) {
                ++year;
                month = 1;
            }
        }

        return Islamic(year, month, day);
    }

    // Returns the name of the specified Islamic month.
    std::string islamic_month_name(month_t month) {
        static const std::string monthNames[12] = {
            "Muharram", "Safar", "Rabi'al-Awwal", "Rabi'ath-Thani",
            "Jumada I-Ula", "Jumada t-Tania", "Rajab", "Sha'ban",
            "Ramadan", "Shawwal", "Dhu I-Qa'da", "Dhu I-Hijja"
        };
        return monthNames[static_cast<int>(month) - 1];
    }

    // Converts a Julian Day (JDN) to hours, minutes, and seconds.
    inline void hms(double jd, hour_t& hour, minute_t& minute, second_t& second) {
        double tod = (jd + 0.5) - floor(jd + 0.5);  // Time of day fraction.
        int secondsInDay = static_cast<int>(floor(tod * SECONDS_PER_DAY + 0.5));

        hour = secondsInDay / 3600;
        minute = (secondsInDay / 60) % 60;
        second = secondsInDay % 60;
    }

    // Converts Julian Day (JDN) to an Islamic date.
    void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day) {
        double a = floor(jd) + 0.5;

        year = static_cast<year_t>(floor((30 * (jd - ISLAMIC_EPOCH) + 10646) / 10631.0));
        month = static_cast<month_t>(std::min(12, static_cast<int>(
            ceil((jd - (29 + islamic_to_jd(year, 1, 1))) / 29.5) + 1)));
        day = static_cast<day_t>(a - islamic_to_jd(year, month, 1) + 1);
    }

    // Converts Julian Day (JDN) to an Islamic date with time components.
    void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day,
        hour_t& hour, minute_t& minute, second_t& second) {
        jd_to_islamic(jd, year, month, day);  // Convert date part.
        hms(jd, hour, minute, second);  // Extract time part.
    }

    // Default constructor for Islamic date, initializes to the current local time.
    Islamic::Islamic() {
        time_t utcTime = time(NULL);
        struct tm tmLocal;
        localtime_s(&tmLocal, &utcTime);

        // Convert current Gregorian date to Julian Day and then to Islamic.
        jd_t jd = gregorian_to_jd(
            year_t(tmLocal.tm_year) + 1900, month_t(tmLocal.tm_mon) + 1, day_t(tmLocal.tm_mday),
            hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)
        );
        jd_to_islamic(jd, year_, month_, day_, hour_, minute_, second_);
    }

    // Constructor for Islamic date, optionally ignoring the time of day.
    Islamic::Islamic(now_t option) {
        time_t utcTime = time(NULL);
        struct tm tmLocal;
        localtime_s(&tmLocal, &utcTime);

        jd_t jd = gregorian_to_jd(
            year_t(tmLocal.tm_year) + 1900, month_t(tmLocal.tm_mon) + 1, day_t(tmLocal.tm_mday)
        );
        jd_to_islamic(jd, year_, month_, day_);

        if (option == NOTIMEOFDAY) {
            hour_ = 0;
            minute_ = 0;
            second_ = 0;
        }
    }

    // Equality operator to compare Islamic and Gregorian dates.
    bool operator==(const Islamic& islamic, const Gregorian& gregorian) {
        jd_t islamic_jd = islamic.to_jd();
        jd_t gregorian_jd = gregorian_to_jd(
            gregorian.year(), gregorian.month(), gregorian.day(),
            gregorian.hour(), gregorian.minute(), gregorian.second()
        );
        return std::abs(islamic_jd - gregorian_jd) < 1e-9;  // Precision check.
    }

    // Equality operator to compare Gregorian and Islamic dates.
    bool operator==(const Gregorian& gregorian, const Islamic& islamic) {
        return islamic == gregorian;  // Reuse the previous operator.
    }

}  // end of namespace khronos