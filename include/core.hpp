#pragma once 
#ifndef __CORE__
#define __CORE__

#if defined(_MSC_VER)
	// MSVC provides __debugbreak() as an intrinsic.
#elif defined(__GNUC__) || defined(__clang__)
	inline void __debugbreak() {
		__builtin_trap();
	}
#else
	#include <cstdlib>
	inline void __debugbreak() {
		std::abort();
	}
#endif

constexpr const unsigned int uint32_max = -1;
constexpr const unsigned long long uint64_max = -1;

#if _DEBUG
	constexpr auto const _debug = true;
#else
	constexpr auto const _debug = false;
#endif
using _SizeType = unsigned long long;
#endif