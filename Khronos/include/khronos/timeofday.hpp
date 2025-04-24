#pragma once

/*
khronos\timeofday.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'time-of-day' declarations.
*/


#include <khronos/def.hpp>


namespace khronos {
	// LITERALS
	/* UDL - converts an 'AM' hour to 'military' hour. */
	inline int operator"" _am(unsigned long long hour) {
		return static_cast<int>((hour == 12) ? 0 : hour);
	}


	/* UDL - converts an 'PM' hour to 'military' hour. */

	inline int operator"" _pm(unsigned long long hour) {
		return static_cast<int>((hour == 12) ? 12 : hour + 12);
	}


	/* Seconds per day. */

	constexpr int SECONDS_PER_DAY = 24 * 60 * 60;


	double tod(hour_t hour, minute_t minute, second_t second);

	/**	Convert Time-of-day (TOD) to hours, minutes, seconds.  Civil days run from midnight to midnight.
		@param tod [in] a time-of-day [0..1)
		@param hours [out] 0-23
		@param minutes [out] 0-59
		@param seconds [out] [0.0..60.0)
		*/

} // end-of-namespace khronos
