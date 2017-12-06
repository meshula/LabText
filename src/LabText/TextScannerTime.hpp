#pragma once

#include "TextScanner.hpp"
#include <chrono>

namespace TextScanner
{
	// note: 29.97 = 30 / 1.001
	// hours:minutes:seconds:frames
	std::chrono::steady_clock::time_point ParseSMPTE(const std::string&, int fps_num, int fps_den);

#ifdef IMPL_TEXTSCANNER

	std::chrono::steady_clock::time_point ParseSMPTE(const std::string& s, int fps_num, int fps_den)
	{
		auto parts = Split(s, ':', false, false);
		if (parts.size() != 4)
			throw std::runtime_error("Incorrectly formatted time code");
		float hours, minutes, seconds, frames;
		Parsef(parts[0], hours);
		Parsef(parts[0], minutes);
		Parsef(parts[0], seconds);
		Parsef(parts[0], frames);
		return std::chrono::hours(hours) + std::chrono::minutes(minutes) + std::chrono::seconds(seconds) + std::chrono::seconds(frames * fps_num / fps_den);
	}

#endif

}
