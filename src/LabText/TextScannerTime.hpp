#pragma once

#include "TextScanner.hpp"
#include <chrono>

namespace TextScanner
{
	// note: 29.97 = 30 / 1.001
	// hours:minutes:seconds:frames
    // returns time relative to epoch
	std::pair<bool, std::chrono::steady_clock::time_point> ParseSMPTE(const std::string&, int fps_num, int fps_den) noexcept;

    // returns string representation of time relative to epoch
	std::string SMPTE_ToString(std::chrono::steady_clock::time_point) noexcept;

#ifdef IMPL_TEXTSCANNER

	std::pair<bool, std::chrono::steady_clock::time_point> ParseSMPTE(const std::string& s, int fps_num, int fps_den) noexcept
	{
		auto parts = Split(s, ':', false, false);
		if (parts.size() != 4)
            return { false, {} };

        float hours, minutes, seconds, frames;
		Parsef(parts[0], hours);
		Parsef(parts[0], minutes);
		Parsef(parts[0], seconds);
		Parsef(parts[0], frames);
        return { true,
            std::chrono::steady_clock::time_point() + 
               std::chrono::hours{ static_cast<int>(hours) } +
               std::chrono::minutes{ static_cast<int>(minutes) } +
               std::chrono::seconds{ static_cast<int>(seconds) } +
               std::chrono::seconds{ static_cast<int>(frames) * fps_num / fps_den }};
	}

	std::string SMPTE_ToString(std::chrono::steady_clock::time_point t, int fps_num, int fps_den) noexcept
	{
		auto microSec = std::chrono::time_point_cast<std::chrono::microseconds>(t);
		uint64_t n = microSec.time_since_epoch().count();
		uint64_t residual = n % 1000000;
		n /= 1000000;
		uint64_t seconds = n % 60;
		n /= 60;
		uint64_t minutes = n % 60;
		n /= 60;
		uint64_t hours = n;
		char buff[128];
		sprintf_s(buff, 128, "%2lld:%2lld:%2lld:%2lld", hours, minutes, seconds, (residual * fps_num) / fps_den);
		return buff;
	}

#endif

}
