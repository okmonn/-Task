#pragma once
#include <string>

namespace std {
#ifdef UNICODE
	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> tstring;
#else
	typedef std::basic_string<char, std::char_traits<char>, std::allocator<char>> tstring;
#endif
};